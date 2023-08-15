/* 
 * pagedir.c - CS50 page directory program
 *
 * see pagedir.h for more information.
 *
 * Charlie Childress, February 2022 
 * Used pseudocode from cs50 webpage
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "webpage.h"
#include "mem.h"
#include "file.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
/* see pagedir.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
/* none */

/**************** pagedir_init() ****************/
/* see pagedir.h for description */
bool pagedir_init(const char* pageDirectory) {
  // make sure the directory is not empty; if it is return false as we cannot initialize anything
  if (pageDirectory == NULL) {
    return false;
  }
  
  // construct a pathname
  FILE* fp;
  // allocate it memory
  char* filename = malloc((strlen(pageDirectory) + 10)); // the 10 is for the maximum depth
  sprintf(filename, "%s/.crawler", pageDirectory);  // inside the .crawler file to mark it as directory produced by the Crawler

  // open the file for writing
  fp = fopen(filename, "w");
  if (fp == NULL) {   // if there is an error in opening the file, return false and free the filename
    fclose(fp);
    free(filename);
    return false;
  }

  // close the file and free the filename memory
  fclose(fp);
  free(filename);
  return true;    // return true for a successful initilialization
}

/**************** pagedir_save() ****************/
/* see pagedir.h for description */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
  // check that the directory and webpage are not empty
  if(pageDirectory != NULL && page != NULL) {
    FILE* fp;
    char* filename = malloc((strlen(pageDirectory) + 10)); // allocate memory for filename

    // assign filename to its unique ID
    sprintf(filename, "%s/%d", pageDirectory, docID);
    // open the file for writing
    fp = fopen(filename, "w");
    // print the contents of the webpage
    if (fp != NULL) {
      fprintf(fp, "%s\n", webpage_getURL(page));
      fprintf(fp, "%d\n", webpage_getDepth(page));
      fprintf(fp, "%s\n", webpage_getHTML(page));
      fclose(fp); // close the file
      free(filename); // free the filename memory
    } else {
      free(filename);
    }
  }
}

/**************** pagedir_validate() ****************/
/* see pagedir.h for description */
bool pagedir_validate(const char* pageDirectory) {
  if (pageDirectory == NULL) {
    return false; // return as false as this filename does not exist as a crawler directory
  } else {
    FILE* fp;
    char* filename = malloc((strlen(pageDirectory) + 10)); // allocate memory for filename
    sprintf(filename, "%s/.crawler", pageDirectory);  // check the pageDirectory with a crawler tag
    fp = fopen(filename, "r");
    if (fp != NULL) {
      fclose(fp); // close the file
      free(filename); // free the filename memory
      return true;  // return true as this filename exists a crawler directory
    } else {
      fclose(fp); // close the file
      free(filename); // free the filename memory
      return false;  // return as false as this filename does not exist as a crawler directory
    }
  }
  return false;  // return false if it never hit true
}

/**************** pagedir_load() ****************/
/* see pagedir.h for description */
webpage_t* pagedir_load(const char* pageDirectory, int docID) {
  if (pageDirectory == NULL || docID < 0) {   //if the directory does not exist or the docID is a bad number, return NULL
    return NULL;
  } else {
    FILE* fp;
    char* filename = malloc((strlen(pageDirectory) + 10));  // alloate memory for the filename
    sprintf(filename, "%s/%d", pageDirectory, docID);  //the filename is of form pageDirectory/id
    fp = fopen(filename, "r");
    if (fp == NULL) { // if there is an error opening the file, free the memory and return null
      free(filename); 
      return NULL;
    } else {
      char* url = file_readLine(fp);  //the first line of the file is the URL
      char* pageDepth = file_readLine(fp);  //the second line of the file is the depth
      int depth;
      sscanf(pageDepth, "%d", &depth);
      free(pageDepth);

      char* html = file_readFile(fp); //the rest of the file is the page content (the HTML, unchanged)
      fclose(fp);
      free(filename); //free the memory and close the file

      // initalize webpage and return it
      return webpage_new(url, depth, html);
    }
    return NULL;
  }
}