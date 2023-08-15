/* 
 * querier.c - CS50 'querier' method
 *
 * The TSE Querier is a standalone program that reads the index file 
 * produced by the TSE Indexer, and page files produced by the TSE 
 * Querier, and answers search queries submitted via stdin.
 * usage:
 *   2 command-line arguments for the pageDirectory produced by crawler 
 *   and indexFilename produced by the Indexer
 *   Then reads searches from stdin, one per line, until EOF
 *
 * output:
 *   print a ordered query, nothing, or an error based on input
 *
 * Charlie Childress, February 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h> 
#include <ctype.h>
#include "counters.h"
#include "file.h"
#include "hashtable.h"
#include "mem.h"
#include "webpage.h"
#include "pagedir.h"
#include "index.h"
#include "word.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
// struct for two counters to 
struct allcounters {
  counters_t* ctrs1;
  counters_t* ctrs2;
};

// struct for the array of counters 
struct array {
  counters_t** countersarray;
  int* size;
};

/**************** global types ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
int main(const int argc, char* argv[]);
bool querier(index_t* index, char* pageDirectory);
bool verifyString(char* query);
char** tokenize(char* query, int* size);
bool verifyArray(char** wordArray, int size);
counters_t* querySequence(char** wordArray, int size, hashtable_t* ht);
counters_t* andSequence(char** wordArray, int firstIndex, int lastIndex, hashtable_t* ht);
counters_t* intersection(counters_t* ctrs1, counters_t* ctrs2);
counters_t* combination(counters_t* ctrs1, counters_t* ctrs2, bool free);
int fileno(FILE *stream);

/**************** local functions ****************/
/* not visible outside this file */
static void docsIterate(void *arg, const int key, const int count);
static void countersArrayIterate(void* arg, const int key, const int count);
static void countersCountersIterate(void *arg, const int key, const int count);
static void combinationIterate(void* arg, const int key, const int count);
static void intersectionIterate(void* arg, const int key, const int count);
static void printArray(struct array* arrayPrint, char* pageDirectory);
static void printIterate(void* arg, const int key, const int count);
static void deleteArray(struct array* arrayDel);

/*********************** main() ***********************/
/* Takes int argc for the number of command-line arguments and 
 * char* argv[] as a list of the command-line arguments
 *
 * parses arguments and make sure they are valid and then loads
 * index from indexFilename into an internal data structure
 * calls querier, once done, the memory is freed;
 *
 * Return 0 if everything in the program runs successfully, 
 * otherwise exit at non-zero integer to show error in the program
 */
int main(const int argc, char* argv[]) {
  // check argc to make sure the only arguments are pageDirectory and indexFilename
  if (argc < 3) {
    // too few arguments, print error message to stderr
    fprintf(stderr, "usage: %s [querier]: too few arguments\n", argv[0]);
    exit(1);  // non-zero exit to represent unsuccessful exit status
  } else if (argc == 3) {   // correct number of arguments
    // first make sure the directory exists
    DIR* directory = opendir(argv[1]);
    if (directory) { // directory exists 
      closedir(directory);
    } else {  // directory does not exist or read error
      fprintf(stderr, "usage: %s [pageDirectory]; directory does not exist or there was an error reading\n", argv[1]);
      closedir(directory);
      exit(2);
    }
    // check that the directory is a crawler directory and that it contains a .crawler file
    if (!pagedir_validate(argv[1])) {  // if not, print error to stderr and exit
      fprintf(stderr, "usage: %s [pageDirectory]: directory is not a crawler directory\n", argv[1]);
      exit(3);
    }
    // check that the index filename can be opened and read
    FILE* file;
    if ((file = fopen(argv[2], "r")) == NULL) { // if it cannot be, print error to stderr, close file, and exit
      fprintf(stderr, "usage: %s [indexFilename]: index file does not exist or there was an error reading\n", argv[2]);
      exit(4);
    }
    fclose(file);

    // now see if we have memory to allocate for the arguments using mem_malloc_assert
    char* pageDirectory = mem_malloc_assert(strlen(argv[1]) + 1, "unable to allocate memory for pageDirectory");
    char* indexFilename = mem_malloc_assert(strlen(argv[2]) + 1, "unable to allocate memory for indexFilename");

    strcpy(pageDirectory, argv[1]);
    strcpy(indexFilename, argv[2]);

    // loads index from indexFilename into an internal data structure
    index_t* index = index_load(indexFilename);
    if (index == NULL) {
      fprintf(stderr, "usage: [index_load]: error loading the index\n");
      index_delete(index);
      exit(5);
    }

    // run querier method, if it does not work at some point and returns false, delete the index and return an error
    if(!querier(index, pageDirectory)) {
      fprintf(stderr, "usage: [querier]: error with querier\n");
      index_delete(index);
      exit(6);
    }
    index_delete(index);
    free(pageDirectory);
    free(indexFilename);
  } else {
    // too many arguments
    fprintf(stderr, "usage: %s [querier]; too many arguments\n", argv[0]);
    exit(7); // we continue to increase the exit number to better know where the error occurs
  }

  exit(0); //successful exit status
}

/*********************** querier() ***********************/
/* takes the index_t* index that we will use to find out 
 * the documents that match the query and char* pageDirectory
 * which is the crawler directory that we get our documents from
 *
 * load the hashtable from the index. Then make sure the query
 * is good. Once checked, turn the query into an array of words
 * then varify the array and print out a clean query. Finally
 * send it to other functions to calculate and then print
 *
 * returns a true that everything in the method and the subsequent
 * methods was successful, return false otherwise
 */
bool querier(index_t* index, char* pageDirectory) {
  // make sure all of the parameters are good
  if (index == NULL || pageDirectory == NULL) {
    return false;
  }

  // load in the hashtable from the index
  hashtable_t* ht = index_get_hashtable(index);
  if (ht == NULL) {
    return false;
  }

  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }

  char* query;
  int size;
  char** wordArray;
  counters_t* ctrs1;
  int countersSize;
  counters_t** countersArray;
  int arraySize;

  // keep reading each line of stdin until EOF is reached
  while ((query = file_readLine(stdin)) != NULL) {
    // verify that the string fits according to REQUIREMENTS specs
    if (!verifyString(query)) {
      // if not, reprint the query prompty and go to the next query
      if (isatty(fileno(stdin))) {
      printf("Query? ");
      }
      free(query);
      continue;
    } else {
      // use a int* for the size of the array
      size = 0;
      // tokenize the query and create an array out of it
      wordArray = tokenize(query, &size);
      // if wordArray is NULL, free the memory and move on to the next query
      if (wordArray == NULL) {
        fprintf(stderr, "Error: issue when tokenizing query\n");
        if (isatty(fileno(stdin))) {
        printf("Query? ");
        }
        free(wordArray);
        free(query);
        continue;
      } else {
        // if not, reprint the query prompty and go to the next query
        printf("Query: ");
        for (int i = 0; i < size; i++) {
          printf("%s ", wordArray[i]);
        }
        printf("\n");
      }
      // now verify that the array is good
      if (!verifyArray(wordArray, size)) {
        // if not, reprint the query prompty and go to the next query and free the memory
        if (isatty(fileno(stdin))) {
        printf("Query? ");
        }
        free(wordArray);
        free(query);
        continue;
      } else {
        ctrs1 = querySequence(wordArray, size, ht);
        if (ctrs1 == NULL) {
          // if NULL, reprint the query prompty and go to the next query
          if (isatty(fileno(stdin))) {
          printf("Query? ");
          }
          counters_delete(ctrs1);
          free(wordArray);
          free(query);
          continue;
        }
        countersSize = 0;
        counters_iterate(ctrs1, &countersSize, docsIterate); // get total docIDS
        countersArray = mem_malloc(countersSize * sizeof(counters_t*));
        arraySize = 0;
        struct array array = { countersArray, &arraySize };
        counters_iterate(ctrs1, &array, countersArrayIterate);  // order the counters
        printArray(&array, pageDirectory);
        counters_delete(ctrs1);
        deleteArray(&array);
        free(countersArray);
      }
    }
    // free the memory and move on to the next query
    free(wordArray);
    free(query);
    if (isatty(fileno(stdin))) {
      printf("Query? ");
    }
  }
  return true;
}

/*********************** verifyString() ***********************/
/* Takes a char* which represents a query
 *
 * loop through every character in the query to make sure that 
 * it consists of only letters and spaces with no other erroneous
 * characters
 *
 * Return false if any character is not a space or letter. If 
 * every character is checked and there are no errors, return 
 * true
 */
bool verifyString(char* query) {
  // first make sure that the query is not NULL
  if (query == NULL) {
    fprintf(stderr, "Error: NULL query\n");
    return false;
  } else {
    // loop through every character in the query
    for (int i = 0; i < strlen(query); i++) {
      if (isalpha(query[i])) {  // if the character is a letter...
        continue;               // check the next character
      } else if (isspace(query[i])) {  // if the charcater is a space...
        continue;                      // check the next letter
      } else {
        fprintf(stderr, "Error: bad character '%c' in query\n", __toascii(query[i])); // give error message to stdout if a character is bad
        return false;   // if there is a bad character, the query is bad, return false
      }
    }
  }
  return true;  // if no errors, return true
}

/*********************** tokenize() ***********************/
/* Takes a char* for the query that we want to tokenize and
 * int* which is a pointer that will track how large the 
 * wordArray is for later methods
 *
 * 
 */
char** tokenize(char* query, int* size) {
  if (query == NULL) {
    fprintf(stderr, "Error: NULL query\n");
    return NULL;
  }
  char* word;
  bool wordBuild = false;
  char** wordArray = mem_malloc_assert(sizeof(query) * 100, "Error: cannot allocate memory for query");
  int wordIndex = 0;
  int length = strlen(query);
  for (int i = 0; i < length; i++) {
    if (!wordBuild && isalpha(query[i])) {
      word = &query[i];
      wordBuild = true;
    } else if (wordBuild && isspace(query[i])) {
      query[i] = '\0';
      wordArray[wordIndex] = normalizeWord(word);
      wordIndex++;
      wordBuild = false;
    }
  }
  if (wordBuild) {
    wordArray[wordIndex] = normalizeWord(word);
    wordIndex++;
  }
  *size = wordIndex;

  return wordArray;
}

/*********************** verifyArray() ***********************/
/* Takes a char** wordArray which is a string to represent the query
 * and int size for the size of the string
 *
 * make sure the literal 'and' comes between words, and the literal 
 * 'or' must come between andsequences, so they cannot appear at the 
 * beginning or end of a query. Also check that the literals ('and' & 'or') 
 * are not  adjacent
 *
 * if there are any errors or improper formatting, print an error
 * message to stdout and return false. If no errors are found, return
 * true
 */
bool verifyArray(char** wordArray, int size) {
  // make sure the wordArray is not NULL and there is a positive size
  if(wordArray == NULL || size <= 0) {
    fprintf(stderr, "Error: NULL query\n");
    return false;
  }

  if (strcmp(wordArray[0], "and") == 0) {
    fprintf(stderr, "Error: 'and' cannot be first\n");
    return false;
  } else if (strcmp(wordArray[0], "or") == 0) {
    fprintf(stderr, "Error: 'or' cannot be first\n");
    return false;
  } else if (strcmp(wordArray[size - 1], "and") == 0) {
    fprintf(stderr, "Error: 'and' cannot be last\n");
    return false;
  } else if (strcmp(wordArray[size - 1], "or") == 0) {
    fprintf(stderr, "Error: 'or' cannot be last\n");
    return false;
  } else {
    for(int i = 1; i < size - 1; i++) {
      if(strcmp(wordArray[i], "and") == 0 || strcmp(wordArray[i], "or") == 0) {
        if(strcmp(wordArray[i + 1], "and") == 0 || strcmp(wordArray[i + 1], "or") == 0) {
          fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", wordArray[i], wordArray[i + 1]);
          return false;
        }
      }
    }
  }
  return true;
}

/*********************** querySequence() ***********************/
/* Takes the char** wordArray for the query that is being evaluated,
 * the int size for the number of words or indices in the array, and
 * the hashtable_t* ht from indexer
 *
 * Go through each word, using the combination method to add it to the
 * total counter until there is an "or" then create a separate counter
 * for the words after the "or" so we can compare them
 *
 * return the totalCounter that contains all documents that match the 
 * query and their counters
 */
counters_t* querySequence(char** wordArray, int size, hashtable_t* ht) {
  // make sure none of the parameters are empty
  if (wordArray == NULL || size < 0 || ht == NULL) {
    return NULL;
  }
  counters_t* totalCounter = mem_assert(counters_new(), "totalCounter");
  int orAppearance = -1; //start out not in the array
  for(int i = 0; i < size; i++) {
    if(strcmp(wordArray[i], "or") == 0) {
      // have total counter be the sequence of words before the current "or"
      totalCounter = combination(totalCounter, andSequence(wordArray, orAppearance+1, i, ht), true);
      orAppearance = i;
    }
  }
  // then compare this sequence before the or with the sequence after the or
  // if no or appears, this is just the entire sequence
  totalCounter = combination(totalCounter, andSequence(wordArray, orAppearance+1, size, ht), true);
  return totalCounter;

}

/*********************** andSequence() ***********************/
/* Takes the char** wordArray for the query that is being evaluated,
 * the int firstIndex for the first index of the sequence and 
 * int lastIndex for the last index of the sequence, and the 
 * hashtable_t* ht from indexer
 *
 * Go through each word from the first index to the last index, 
 * using the combination method to add all words with an intersection
 *
 * return the totalCounter that contains all documents in the sequence
 * that share a word in the query
 */
counters_t* andSequence(char** wordArray, int firstIndex, int lastIndex, hashtable_t* ht) {
  // make sure all the parameters are valid and that the first index is before the last index
  if (wordArray == NULL || firstIndex < 0 || lastIndex < 0 || firstIndex > lastIndex || ht == NULL) {
    return NULL;
  }

  counters_t* totalCounter = mem_assert(counters_new(), "totalCounter");
  // have total counter be the sequence of words before the current word
  totalCounter = combination(totalCounter, hashtable_find(ht, wordArray[firstIndex]), false);
  for (int i = firstIndex + 1; i < lastIndex; i++) {
    if(strcmp(wordArray[i], "and") != 0) {
      // as long as the word is not "and", find the intersection the array and the current word
      totalCounter = intersection(totalCounter, hashtable_find(ht, wordArray[i]));
    }
  }
  return totalCounter;
}

/*********************** intersection() ***********************/
/* Takes the two counters that want to find the intersection of
 *
 * Creates an allcounters struct from the intersection counter and
 * use it to compare the two counters while putting their intersection
 * into a third counter. Must iterate through both counters to do it
 *
 * return the counter that contains the counters that intersect
 */
counters_t* intersection(counters_t* ctrs1, counters_t* ctrs2) {
  counters_t* intersection = mem_assert(counters_new(), "intersection");
  struct allcounters counters = { intersection, ctrs2 };
  counters_iterate(ctrs1, &counters, intersectionIterate);
  counters_delete(ctrs1);
  return intersection;
}

/*********************** combination() ***********************/
/* Takes the two counters that want to find the combination of
 *
 * Creates an a counters struct for the combination of the two
 * other counters, then iterate through both counters and put 
 * them into the combination counter. After that delete the first
 * counter as it is no longer useful and delete the second counter
 * if specified.
 *
 * return the combination counter
 */
counters_t* combination(counters_t* ctrs1, counters_t* ctrs2, bool free) {
  counters_t* combination = mem_assert(counters_new(), "combination");
  counters_iterate(ctrs1, combination, combinationIterate);
  counters_iterate(ctrs2, combination, combinationIterate);
  counters_delete(ctrs1);
  if (free) {
    counters_delete(ctrs2);
  }
  return combination;
}

/*********************** docsIterate() ***********************/
/* itemfunc for the iteration in the querier method
 */
static void docsIterate(void *arg, const int key, const int count) {
  int* num;
  if ((num = arg) != NULL && key >=0 && count >=0) {
    (*num)++;
  }
}

/*********************** countersArrayIterate() ***********************/
/* itemfunc for the iteration in the querier method
 */
static void countersArrayIterate(void* arg, const int key, const int count) {
  struct array* array = arg;
  counters_t** countersarray = array->countersarray;
  int* size = array->size;

  countersarray[*size] = counters_new();
  counters_set(countersarray[*size], key, count);
  counters_t* add = countersarray[*size];

  int index = *size - 1;
  while(index >= 0) {
    int count2 = 0;
    counters_iterate(countersarray[index], &count2, countersCountersIterate);
    if(count2 > count) {
      break;
    } else {
      countersarray[index+1] = countersarray[index];
      index--;
    }
  }
  (*size)++;
  countersarray[index + 1] = add;
}

/*********************** countersCountersIterate() ***********************/
/* itemfunc for the iteration in the countersArrayIterate method
 */
static void countersCountersIterate(void* arg, const int key, const int count) {
  int* num = arg;
  if (num != NULL && key >=0 && count >=0) {
    *num += count;
  }
}

/*********************** combinationIterate() ***********************/
/* itemfunc for the iteration in the combination method
 */
static void combinationIterate(void* arg, const int key, const int count) {
  counters_t* ctrs = arg;
  int count2;
  if ((count2 = counters_get(ctrs, key)) != 0) {
    counters_set(ctrs, key, count + count2);
  } else {
    counters_set(ctrs, key, count);
  }
}

/*********************** intersectionIterate() ***********************/
/* itemfunc for the iteration in the intersection method
 */
static void intersectionIterate(void* arg, const int key, const int count) {
  struct allcounters* allCounters= arg;
  counters_t* totalCounter = allCounters->ctrs1;
  counters_t* ctr2 = allCounters->ctrs2;
  int count2;
  if ((count2 = counters_get(ctr2, key)) == 0) {
    counters_set(totalCounter, key, count2);
  } else {
    if (count > count2) {
      counters_set(totalCounter, key, count2);
    } else if (count <= count2) {
      counters_set(totalCounter, key, count);
    }
  }
}

/*********************** printArray() ***********************/
/* Takes the array* array of pointers to the (docID, count) 
 * counters that match the query and the char* pageDirectory
 * where the documents with these docIDs exist
 *
 * Iterate through the now ordered array and print out each counters
 * information and the URL for the docs that match the query. 
 * If there are no matches, print "No documents match."
 * 
 * Return nothing, just print statements
 */
static void printArray(struct array* arrayPrint, char* pageDirectory) {
  // get the information held by the array
  struct array* array = arrayPrint;
  counters_t** countersArray = array->countersarray;
  int* num = array->size;

  // if there are no counters meaning that there are no documents that represent the query...
  if (*num <= 0) {
    printf("No documents match.\n");  // print no matches
  } else {
    printf("Matches %d documents (ranked):\n", *num);   // else print the ranked list of doc
    for (int i = 0; i < *num; i++) {    // do this by iterating through each counter 
      counters_iterate(countersArray[i], pageDirectory, printIterate);
    }
  }
}

/*********************** printIterate() ***********************/
/* Takes arg which is a pageDirectory, key which is a docID, and 
 * count which is used to keep track of the count of each counter
 *
 * Load files from pageDirectory and unique docID.
 * Then print all query documents and their scores and information
 * 
 * Return nothing, only print
 */
static void printIterate(void* arg, const int key, const int count) {
  if (arg != NULL && key >= 0 && count > 0) {
    FILE* fp;
    char* pageDirectory = arg;
    char* filename = malloc((strlen(arg) + 10));  // alloate memory for the filename
    sprintf(filename, "%s/%d", pageDirectory, key);  //the filename is of form pageDirectory/id
    fp = fopen(filename, "r");
    if (fp == NULL) { // if there is an error opening the file, free the memory and return null
      free(filename);
    } else {
      char* URL = file_readLine(fp); // get URL is the top line of the file
      printf("score   %d doc   %d: %s\n", count, key, URL);
      fclose(fp);
      free(filename);
      free(URL);
    }
  }
}

/*********************** deleteArray() ***********************/
/* Parameters:
 *  array struct that we want to delete
 *
 * If the array is not NULL, call counters_delete on each
 * counter in the array of counters in the array
 * to delete the counters and then free the array.
 * If the array is NULL, simply do nothing as there
 * is no memory to free
 *
 * Return nothing
 */
static void deleteArray(struct array* arrayDel) {
  if (arrayDel != NULL) {
    struct array* array = arrayDel;
    counters_t** countersArray = array->countersarray;
    int* size = array->size;
    // loop through each counter in the counterArray and delete it
    for(int i = 0; i < *size; i++) {
      counters_delete(countersArray[i]);
    }
  }
}