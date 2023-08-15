/* 
 * word.c - CS50 program for normalizing words
 *
 * see word.h for more information.
 *
 * Charlie Childress, cs50, February 2022
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**************** normalizeWord ****************/
/* see word.h for documentation */
char* normalizeWord(char* word) {
  // make sure that the word is not empty or erroneous
  if (word == NULL) {
    return NULL;    // if it is, cannot normalize it, return NULL
  }

  // go through each index of the char* and change it to lowercase
  for(int i = 0; i < strlen(word); i++) {
    word[i] = tolower(word[i]);   // call the tolower() method to covert each letter
  }

  return word;  // return the normalized word
}