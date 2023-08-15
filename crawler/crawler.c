/* 
 * crawler.c - CS50 'crawler' method
 *
 * The TSE crawler crawls the web and retrieves webpages 
 * recursively, limiting its exploration to a given "depth".
 * usage:
 *   3 command-line arguments for the seedURL, pageDirectory, and maxDepth
 *
 * output:
 *   directory with webpage content from all webpages that are a given depth from the seedURL
 *   print statements put in stdout
 *
 * Charlie Childress, February 2022
 * Used pseudocode from cs50 webpage
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bag.h"
#include "hashtable.h"
#include "mem.h"
#include "webpage.h"
#include "pagedir.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* none */

/**************** function prototypes ****************/
static void logr(const char *word, const int depth, const char *url);
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/* *************************************************************************************************
 * code taken from Dartmouth cs50 webpage 
 * Takes char* that varies based on what is being ran ("Found", "Added", "Fetched", etc.),
 * an int for current depth of the crawl, and another char* for the url being acted on
 *
 * Prints the depth of the current crawl at left, then indented slightly based on the current 
 * depth, then printed a single word meant to indicate what is being done, then printed the URL. 
 *
 * Ensures a consistent format that can easily be read or add functions to in the future
 */                                 
static void logr(const char *word, const int depth, const char *url) {
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

/* *************************************************************************************************
 * Takes int argc for the number of command-line arguments and char* argv[] as a list of the 
 * command-line arguments
 *
 * Check if there are the right number of arguments, non-zero exit if not, otherwise send the 
 * arguments to parseArgs() and then crawler()
 *
 * Return 0 if everything in the program runs successfully, otherwise exit at non-zero integer to 
 * show error in the program
 */ 
int main(const int argc, char* argv[]) {
  // check argc to make sure the only arguments are seedURL, pageDirectory, and maxDepth
  if (argc < 4) {
    // too few arguments, print error message to stderr
    fprintf(stderr, "usage: %s [crawler]: too few arguments\n", argv[0]);
    exit(1);  // non-zero exit to represent unsuccessful exit status
  } else if (argc == 4) {   // correct number of arguments
    char** seedURLPointer = &argv[1];
    char** pageDirectoryPointer = &argv[2];
    int* maxDepthPointer = (int*) argv[3];

    // parse the arguments to make sure they are correct
    parseArgs(argc, argv, seedURLPointer, pageDirectoryPointer, maxDepthPointer);

    // if the arguments are correct, assign variables to their value and run them through crawler
    char* seedURL = argv[1];
    char* pageDirectory = argv[2];
    int maxDepth;
    sscanf(argv[3], "%d", &maxDepth);

    crawl(seedURL, pageDirectory, maxDepth);
  } else {
    // too many arguments
    fprintf(stderr, "usage: %s [crawler]; too many arguments\n", argv[0]);
    exit(2); // we continue to increase the exit number to better know where the error occurs
  }
  exit(0); //successful exit status
}

/* *************************************************************************************************
 * Takes int argc for the number of command-line arguments, char* argv[] as a list of the 
 * command-line arguments, a pointer to the seedURL, a pointer to the pageDirectory, and a pointer
 * to the maxDepth
 * 
 * Goes through each command-line argument to make sure they are of valid format
 *
 * If an error occurs, free any memory, send an error message to stderr, and exit the program
 */ 
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {
  // first check the seedURL
  char* normalURL = normalizeURL(*seedURL); // normalize the seedURL
  // validate that it is an internal URL
  if (!isInternalURL(normalURL)) {  // if not, send an error message, free the URL, and non-zero exit the program
    fprintf(stderr, "usage: %s [seedURL]; seedURL is not an internal URL\n", argv[1]);
    free(normalURL);
    exit(3);
  }
  free(normalURL);  // free successful URL

  // check the pageDirectory
  if (*pageDirectory == NULL) {
    fprintf(stderr, "usage: %s [pageDirectory]; directory does not exist\n", argv[2]);
    exit(4);
  }
  if (!pagedir_init(*pageDirectory)) {  // if unable to initialize, there is a problem with the pageDirectory, exit program
    fprintf(stderr, "usage: %s [pageDirectory]; error in initializing directory\n", argv[2]);
    exit(5);
  }

  int depth;
  char ignore;
  // check the depth
  if (sscanf(argv[3], "%d%c", &depth, &ignore) != 1) { // check whether the depth is of the right int format
    fprintf(stderr, "usage: %s [maxDepth]; improper maxDepth\n", argv[3]);
    exit(6);
    // make sure the depth is within bounds [0, 10]
  } else if (depth > 10) {
    fprintf(stderr, "usage: %s [maxDepth bounds [0, 10]]; maxDepth is too high\n", argv[3]);
    exit(7);
  } else if(depth < 0) {
    fprintf(stderr, "usage: %s [maxDepth bounds [0, 10]]; maxDepth is too low \n", argv[3]);
    exit(8);
  }
}


/* *************************************************************************************************
 * Takes the seedURL that we want to explore, pageDirectory that we want to insert to, and maxDepth
 * that is our max depth we can crawl
 *
 * Loops over pages to explore, until the list is exhausted
 *
 * exits and frees memory if error occurs, otherwise return nothing
 */ 
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) {
  webpage_t* webpage;
  // allocate memory for the seedURL so that you can put it in the bag and hashtable
  char* url = mem_malloc(strlen(seedURL)*sizeof(char) + 1);
  strcpy(url, seedURL);
  // the size of the slots is impossible to determine in advance, so we use 200
  hashtable_t* ht = hashtable_new(200);
  // check if hashtable is null, if it is: send an error message, free the memory, and exit
  if (ht == NULL) {
    fprintf(stderr, "error when creating hashtable\n");
    mem_free(url);
    hashtable_delete(ht, NULL);
    exit(9);
  } else {  // if it's not empty, try to insert the the seedURL into the hashtable
      if (!hashtable_insert(ht, seedURL, "item")) { // check if there is an error in inserting
      fprintf(stderr, "error when inserting seedURL into hashtable\n");
      mem_free(url);
      hashtable_delete(ht, NULL);
      exit(10);
    }
  }

  bag_t* bag = bag_new();   // initialize new bag
  if (bag == NULL) {
    // check if hashtable is null, if it is: send an error message, free the memory, and exit
    fprintf(stderr, "error when creating bag\n");
    hashtable_delete(ht, NULL); // delete the hashtable
    bag_delete(bag, NULL);    // delete the bag
    exit(11);
  } else {  // if bag is not empty
    webpage = webpage_new(url, 0, NULL);  // create a new webpage from the url
    if (webpage == NULL) {  // make sure the webpage is not null
      fprintf(stderr, "error when creating webpage\n");
      webpage_delete(webpage); // delete the webpage
      hashtable_delete(ht, NULL); // delete the hashtable
      bag_delete(bag, NULL);    // delete the bag
      exit(12);
    } else {
      bag_insert(bag, webpage); // then insert the webpage representing the seedURL into the bag
      if (bag == NULL) { // check for errors in inserting the webpage into the bag
        fprintf(stderr, "error when inserting webpage into bag\n");
        webpage_delete(webpage);  // delete the webpage
        hashtable_delete(ht, NULL); // delete the hashtable
        bag_delete(bag, NULL);    // delete the bag
        exit(13);
      }
    }
  }
  int counter = 1; // counter to produce unique id code
  // while bag is not empty, pull a webpage from the bag one at a time
  while ((webpage = bag_extract(bag)) != NULL) {
    // fetch the HTML for that webpage
    if (webpage_fetch(webpage)) {  // if fetch was successful
      logr("Fetched", webpage_getDepth(webpage), webpage_getURL(webpage));  // print that the webpage is being fetched
      pagedir_save(webpage, pageDirectory, counter);  // save the webpage to pageDirectory
      counter++; // increase counter by one
      if (webpage_getDepth(webpage) < maxDepth) { // if the webpage is not at maxDepth
        pageScan(webpage, bag, ht);   // pageScan the HTML
      }
    }
    webpage_delete(webpage);    // delete the webpage
  }
  hashtable_delete(ht, NULL); // delete the hashtable
  bag_delete(bag, NULL);    // delete the bag
}


/* *************************************************************************************************
 * Takes webpage_t* that we want to scan, the bag_t* that we are checking from, and the hashtable_t* 
 * that we are inserting the webpage into
 *
 * Given a webpage, scan the given page to extract any links (URLs), ignoring non-internal URLs; 
 * for any URL not already seen before (i.e., not in the hashtable), add the URL to both the hashtable 
 * pages_seen and to the bag pages_to_crawl
 */ 
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {
  // check that the page, bag, and hashtable are not empty; otherwise delete the webpage and return
  if (pagesToCrawl == NULL || pagesSeen == NULL || page == NULL) {
    webpage_delete(page);
    return;  
  }

  // print the webpage url that is being scanned so we can track if needed
  int depth = webpage_getDepth(page);
  logr("Scanning", depth, webpage_getURL(page));

  int count = 0;
  char* next;
  // look through each URL that the webpage has
  while ((next = webpage_getNextURL(page, &count)) != NULL) {
    logr("Found", depth, next);   // print found for all urls in the webpage
    if (!isInternalURL(next)) {   // check if the urls are internal
      logr("IgnExtrn", depth, next);  // if not, print they are external and continue
      mem_free(next);
      continue;
    } else {                      // if they are internal, try to insert them into hashtable
      if (hashtable_insert(pagesSeen, next, "item")) {  // if the url is new and can be inserted
        webpage_t* nextPage = webpage_new(next, webpage_getDepth(page) + 1, NULL);  // give it memory
        bag_insert(pagesToCrawl, nextPage);             // and insert it in the bag of pages that need to be crawled
        logr("Added", depth, next);     // print that the url has been added
      } else {
        logr("IgnDupl", depth, next);   // print that this is a duplicate url and continue
        mem_free(next);
        continue;
      }
    }
  }
  mem_free(next); // free the char*
}
