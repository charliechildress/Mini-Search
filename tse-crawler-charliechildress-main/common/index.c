/* 
 * index.c - CS50 program for providing index data structures
 *
 * see index.h for more information.
 *
 * Charlie Childress, cs50, February 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "index.h"
#include "hashtable.h"
#include "file.h"
#include "mem.h"
#include "counters.h"


/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct index {
  hashtable_t* hashtable;
} index_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see hashtable.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
void word_write(void* arg, const char* word, void* item);
void counter_write(void* arg, const int docID, const int counter);
void hashtable_indexDelete(void* item);

/**************** index_get_hashtable() ****************/
/* see index.h for description */
hashtable_t* index_get_hashtable(index_t* index) {
  return index ? index->hashtable : 0;
}

/**************** index_new() ****************/
/* see index.h for description */
index_t* index_new() {
  index_t* index = mem_malloc(sizeof(index_t));  
  if (index == NULL) {  // error allocating index
    index_delete(index);  // free the memory allocated
    return NULL;  // return nothing
  } else {
    index->hashtable = hashtable_new(200);  // create a hashtable for the index (size could be 200-900 according to specs)
    if (index->hashtable == NULL) { // if there is an error creating hashtable...
      index_delete(index);  // free the memory allocated
      return NULL;   // and return nothing
    }
    return index;   // otherwise return the index with the new hashtable
  }
}

/**************** index_insert() ****************/
/* see index.h for description */
bool index_insert(index_t* index, const char* word, int docID) {
  counters_t* counter;
  if (index != NULL && word != NULL && docID >= 0) {  // make sure all parameters are valid
    // set counter to the current count of the word in the hashtable
    if ((counter = hashtable_find(index->hashtable, word)) == NULL) {   // if the the word is not in the hashtable
      counter = counters_new();   // create a new counter for the word
      // then insert the word into the hashtable with the new counter
      if (!hashtable_insert(index->hashtable, word, counter)) {  // if there is an error creating a set for the word, return false
        return false;
      }
    }
  } else {  // if there are any faulty parameters
    return false;
  }
  counters_add(counter, docID);   // add the word counter in the hashtable to current counter for the docID
  return true;
}

/**************** index_load() ****************/
/* see index.h for description */
index_t* index_load(const char* oldIndexFilename) {
  // check to see if the filename is NULL
  if (oldIndexFilename == NULL) {
    return NULL;
  }
  // open the filename and make sure it opens correctly
  FILE* fp;
  fp = fopen(oldIndexFilename, "r");
  if (fp == NULL) { // if the file does not open correctly print to stderr, close file, and return NULL
    fprintf(stderr, "usage: %s [index_load]; error when trying to open oldIndexFilename\n", oldIndexFilename);
    fclose(fp);
    return NULL;
  }
  // to get the size of the hashtable, find how many lines as each one represents a slot
  int indexSize = file_numLines(fp);
  index_t* index = index_new(indexSize + 1);
  char* word;
  while ((word = file_readWord(fp)) != NULL) { // go through each word of the file as long as words exists
    int docID;
    int counter;
    // scan the rest of the line for pairs of ints for the docID and counter
    while(fscanf(fp, " %d %d ", &docID, &counter) == 2) {
      for (int i = 0; i < counter; i++) {
        // insert for each counter
        index_insert(index, word, docID);
      }
    }
    free(word);
  }
  fclose(fp);
  return index;
}

/**************** index_write() ****************/
/* see index.h for description */
bool index_write(index_t* index, char* newIndexFilename) {
  // if index if empty return false as there is nothing to write
  if (index == NULL) {
    return false;
  }
  FILE* fp;
  // check to see if the file is openable
  if ((fp = fopen(newIndexFilename, "w")) == NULL) {
    fprintf(stderr, "usage: %s [index_write]; error when trying to open or create newIndexFilename\n", newIndexFilename);
    return false;
  } else {
    hashtable_iterate(index->hashtable, fp, word_write);  // print out the contents of the index into the newIndexFilename
  }
  fclose(fp);
  return true;
}

/**************** word_write() ****************/
/* Parameters:
 *  arg which is the file we want to print the index
 *  into, docID for specific file that is a key of 
 *  the hashtable, and a counter which is the item 
 *  of the key.
 *
 * print the contents of the index
 *
 */
void word_write(void* arg, const char* word, void* item) {
  FILE* fp = arg;
  fprintf(fp, "%s", word);  // print the words into the index file
  if (item != NULL) {  // then if there are counters
    counters_iterate(item, fp, counter_write); // go through the counters and print the, too
  }
  fprintf(fp, "\n");
}

/**************** counter_write() ****************/
/* Parameters:
 *  arg which is the file we want to print the index
 *  into, docID for specific file that is a key of 
 *  the hashtable, and a counter which is the item 
 *  of the key.
 *
 * print the contents of the counter
 *
 */
void counter_write(void* arg, const int docID, const int counter) {
  FILE* fp = arg;
  fprintf(fp, " %d %d ", docID, counter);  // print the counter into the index file
}

/**************** index_delete() ****************/
/* see index.h for description */
void index_delete(index_t* index) {
  if (index != NULL) {  // if there is a hashtable in the index...
    hashtable_delete(index->hashtable, hashtable_indexDelete);  // delete the hashtable and its items
    free(index);   // and free the index
  }
}

/**************** hashtable_indexDelete() ****************/
/* Parameter:
 *  item/counter in the hashtable that we want to be deleted
 *
 * if the counter is not NULL, delete it
 * if the counter is NULL, do nothing as there is nothing to
 * delete.
 */
void hashtable_indexDelete(void* item) { 
  if (item != NULL) { // if there are counters in the hashtable
    counters_delete(item);  // delete the counters
  }
}

