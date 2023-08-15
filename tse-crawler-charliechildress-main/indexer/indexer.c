/* 
 * indexer.c - CS50 'indexer' method
 *
 * The TSE indexer is a standalone program that reads the document files 
 * produced by the TSE crawler, builds an index, and writes that index to 
 * a file. Its companion, the index tester, loads an index file produced 
 * by the indexer and saves it to another file.
 * usage:
 *   2 command-line arguments for the pageDirectory produced by crawler 
 *   and indexFilename
 *
 * output:
 *   a file (indexFilename) with the formatted index
 *
 * Charlie Childress, February 2022
 * Used pseudocode from cs50 webpage
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include "bag.h"
#include "hashtable.h"
#include "mem.h"
#include "webpage.h"
#include "pagedir.h"
#include "index.h"
#include "word.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* none */

/**************** function prototypes ****************/
int main(const int argc, char* argv[]);
void indexBuild(char* pageDirectory, char* indexFilename);
void indexPage(index_t* index, webpage_t* webpage, int docID);

/*********************** main() ***********************/
/* Takes int argc for the number of command-line arguments and 
 * char* argv[] as a list of the command-line arguments
 *
 * parses arguments and make sure they are valid and then
 * call indexBuild.;
 *
 * Return 0 if everything in the program runs successfully, 
 * otherwise exit at non-zero integer to show error in the program
 */
int main(const int argc, char* argv[]) {
  // check argc to make sure the only arguments are pageDirectory and indexFilename
  if (argc < 3) {
    // too few arguments, print error message to stderr
    fprintf(stderr, "usage: %s [indexer]: too few arguments\n", argv[0]);
    exit(1);  // non-zero exit to represent unsuccessful exit status
  } else if (argc == 3) {   // correct number of arguments
    // now parse the arguments and make sure they are valid
    char* pageDirectory = argv[1];                              // parse the command line
    char* indexFilename = argv[2];
    // first make sure the directory exists
    DIR* directory = opendir(pageDirectory);
    if (directory) { // directory exists 
      closedir(directory);
    } else {  // directory does not exist or read error
      fprintf(stderr, "usage: %s [pageDirectory]; directory does not exist or there was an error reading\n", argv[1]);
      closedir(directory);
      exit(2);
    }
    // check that the directory is a crawler directory and that it contains a .crawler file
    if (!pagedir_validate(pageDirectory)) {  // if not, print error to stderr and exit
      fprintf(stderr, "usage: %s [pageDirectory]: directory is not a crawler directory\n", argv[1]);
      exit(3);
    }
    // check that the index filename can be opened or created
    FILE* file;
    if ((file = fopen(argv[2], "w")) == NULL) { // if it cannot be, print error to stderr, free memory, close file, and exit
      fprintf(stderr, "usage: %s [indexFilename]: problem creating/opening index file\n", argv[2]);
      exit(4);
    }
    fclose(file); 
    indexBuild(pageDirectory, indexFilename);
  } else {
    // too many arguments
    fprintf(stderr, "usage: %s [indexer]; too many arguments\n", argv[0]);
    exit(6); // we continue to increase the exit number to better know where the error occurs
  }
  exit(0); //successful exit status
}

void indexBuild(char* pageDirectory, char* indexFilename) {
  index_t* index = index_new();
  webpage_t* webpage;
  int docID = 1;
  while ((webpage = pagedir_load(pageDirectory, docID)) != NULL) {
    indexPage(index, webpage, docID);
    webpage_delete(webpage);
    docID++;
  }
  index_write(index, indexFilename);
  index_delete(index);
}

void indexPage(index_t* index, webpage_t* webpage, int docID) {
  int pos = 0;
  char* word;
  while ((word = webpage_getNextWord(webpage, &pos)) != NULL) {
    if (strlen(word) < 3) {
      free(word);
      continue;
    }
    normalizeWord(word);
    index_insert(index, word, docID);
    free(word);
  }
}