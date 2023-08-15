/* 
 * pagedir.h - header file for CS50 page directory program
 *
 * We create a re-usable module pagedir.c to handles the 
 * pagesaver mentioned in the design (writing a page to the pageDirectory), 
 * and marking it as a Crawler-produced pageDirectory (as required in the spec). 
 * We chose to write this as a separate module, in ../common, to encapsulate all 
 * the knowledge about how to initialize and validate a pageDirectory, and how 
 * to write and read page files, in one place
 *
 * Charlie Childress, February 2022, cs50
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "webpage.h"

/**************** pagedir_init ****************/
/* Initialize a new page directory
 * Parameters:
 *   char* for the name of a existing directory
 * Construct a pathname and allocate it enough memory
 *    to crawl to a depth of 10 (the max depth)
 * Opens the file for writing
 * Closes the file and returns false on any error.
 * Otherwise, closes the file and returns true to show page was initialized
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Parameters:
 *  webpage_t* we are trying to print
 *  pageDirectory we are trying to create filename to
 *  const int docID for the unique filename number
 * Save the directory and url to a filename with
 *    a unique docID. Construct a file name and allocate 
 *    it enough memory to crawl to a depth of 10 
 *    (the max depth). Then it prints the contents of 
 *    the webpage. Lastly, it closes the file and returns 
 *    its memory
 * Only runs if the webpage and directory are not empty
 * Returns nothing
 * Prints webpage contents
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_validate ****************/
/* Parameters:
 *  pageDirectory
 *
 * Check the directory to see that it is a directory
 * created by crawler. First make sure the directory 
 * not NULL. Next open a filename of the directory 
 * with a ./crawler tag at the end. If we get NULL, 
 * the file does not exist, else it does.
 * 
 * Returns true if the directory exists as a crawler
 * directory
 * Return false otherwise
 */
bool pagedir_validate(const char* pageDirectory);

/**************** pagedir_load ****************/
/* Parameters:
 *  pageDirectory and docID that can be used for
 *  different files in the page directory
 *
 * Load webpages from files in the pageDirectory.
 * Use unique docID to refer to different files
 * in the page directory
 *
 * Returns the webpage if successful
 * Returns NULL if any errors occur
 */
webpage_t* pagedir_load(const char* pageDirectory, int docID);

#endif // __PAGEDIR_H