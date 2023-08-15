/* 
 * indextest.c - unit test program for the CS50 'indexer' method
 *
 * usage:
 *   2 command-line arguments for the oldIndexFilename that it reads from
 *      and the newIndexFilename which it writes the index into
 *   
 * Charlie Childress, February 2022
 * Used pseudocode from cs50 webpage
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file.h"
#include "memory.h"
#include "webpage.h"
#include "pagedir.h"
#include "index.h"
#include "hashtable.h"
#include "word.h"

int main(const int argc, char* argv[]);

/*********************** main() ***********************/
/* Takes int argc for the number of command-line arguments and 
 * char* argv[] as a list of the command-line arguments
 *
 * load the index from the oldIndexFilename into an inverted-
 * index data structure. Then create a file newIndexFilename 
 * and write the index to that file
 *
 * Return 0 if everything in the program runs successfully, 
 * otherwise exit at non-zero integer to show error in the program
 */
int main(const int argc, char* argv[]) {
  // check argc to make sure the only arguments are oldIndexFilename and newIndexFilename
  if (argc < 3) {
    // too few arguments, print error message to stderr
    fprintf(stderr, "usage: %s [indextest]: too few arguments\n", argv[0]);
    exit(1);  // non-zero exit to represent unsuccessful exit status
  } else if (argc == 3) {   // correct number of arguments
    // load an index from the given parameters
    index_t* index = index_load(argv[1]);
    if (index == NULL) {  // if something went wrong with loading, print to stderr and exit
      fprintf(stderr, "usage: %s [index_load]: error when loading index", argv[1]);
      exit(2);
    }
    // write the index onto the new filename
    if (!index_write(index, argv[2])) { // if something went wrong with loading, print to stderr and exit
      fprintf(stderr, "usage: [index_write]: error when writing index onto newIndexFilename");
      exit(3);
    }
    index_delete(index); // free index memory when done
  } else {
    // too many arguments
    fprintf(stderr, "usage: %s [indextest]; too many arguments\n", argv[0]);
    exit(4); // we continue to increase the exit number to better know where the error occurs
  }
  exit(0); //successful exit status
}