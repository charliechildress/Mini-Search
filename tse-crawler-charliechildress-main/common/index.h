/* 
 * index.h - header file for CS50 index program
 *
 * A module providing the data structure to represent 
 * the in-memory index, and functions to read and write 
 * index files.
 *
 * Charlie Childress, February 2022, cs50
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"

/**************** global types ****************/
typedef struct index index_t;

/**************** global types ****************/
hashtable_t* index_get_hashtable(index_t* index);

/**************** index_new ****************/
/* Parameters:
 *  none
 *
 * create a new index and instantiate it with
 * a hashtable of size 200. This size is arbitrary
 * as it is difficult to know the size of the 
 * hashtable. Could be any size from 200-900
 * according to the specs
 *
 * Return the new index if everything is successful
 * Return NULL if there are any errors
 */
index_t* index_new();

/**************** index_insert ****************/
/* Parameters:
 *  index struct that has a hashtable item, a char*
 *  that we want to put into the hashtable, and a
 *  int for the docID for the counter
 *
 * Make sure all of the paramters are valid and 
 * then use the word as a key for the hashtable 
 * and the docID as the item/counter for the index.
 *
 * Return true if the process runs successfully
 * Return false if any errors occur
 */
bool index_insert(index_t* index, const char* word, int docID);

/**************** index_load() ****************/
/* Parameter:
 *  oldIndexFilename in which an index has already
 *  been printed on
 *
 * load the index from the oldIndexFilename into 
 * an inverted-index data structurem thus allowing
 * us to create a file newIndexFilename and write 
 * the index to that file, used in indextester.c
 *
 * Return the index_t* that we will use to write
 * onto the new file
 * If there is an error, exits program
 */
index_t* index_load(const char* oldIndexFilename);

/**************** index_write ****************/
/* Parameters:
 *  char* newIndexFilename as our new file and 
 *  index_t* index as the index we want to put 
 *  into the file
 *
 * Check if the file is able to be opened/created
 * and if it is, go through the contents of
 * the index and print it into the new file
 *
 * Return true if the process runs successfully
 * Return false if any errors occur
 */
bool index_write(index_t* index, char* newIndexFilename);

/**************** index_delete ****************/
/* Parameters:
 *  index struct that we want to delete
 *
 * If the index is not NULL, call hashtable_delete
 * to delete the contents of the hashtable and then
 * free the index.
 * If the index is NULL, simply do nothing as there
 * is no memory to free
 */
void index_delete(index_t* index);

#endif // __INDEX_H