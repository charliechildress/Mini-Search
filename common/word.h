/* 
 * word.h - header file for CS50 word.c program
 *
 * Takes a given word in the indexer file and calls the
 * normalizeWord method on it. Returns the normalized word
 * back to indexer.
 *
 * Charlie Childress, February 2022, cs50
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdlib.h>

/**************** normalizeWord ****************/
/* 
 * Normalizes words to be used in the indexer method
 * "normalize" means to convert all letters to 
 * lower-case.)
 */
char* normalizeWord(char* word);

#endif // __WORD_H