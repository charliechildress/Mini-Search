# CS50 Tiny Search Engine (TSE) Lab 4 Crawler

The TSE `crawler` is a standalone program that crawls the web and retrieves webpages starting from a _"seed" URL_. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given _"depth"_.

We use two data structures: a 'bag' of pages that need to be crawled, and a 'hashtable' of URLs that we have seen during our crawl. Both start empty. 

## Assumptions
The size of the hashtable (slots) is impossible to determine in advance, so we use 200

No other assumptions or implementations beyond what the specs provide

## Usage

To compile, simply `make`.

To test, simply `make test`.

To clean up, run `make clean`.

## Overview

 * `crawler.c` - crawler program
 * `testing.sh` - tests for crawler program
 * `testing.out` - result of testing.sh
 * `Makefile` - compilation procedure

 ## Testing
We write a script `testing.sh` that invokes the crawler several times, with a variety of command-line arguments. First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made. Second, a run with _valgrind_ over a moderate-sized test case (_toscrape_ at _depth_ 1). Third, runs over all three CS50 websites (_letters_ at _depths_ 0,1,2,10, _toscrape_ at _depths_ 0,1,2,3, _wikipedia_ at depths 0,1,2). Run that script with `bash -v testing.sh` so the output of crawler is intermixed with the commands used to invoke the crawler. Verify correct behavior by studying the output, and by sampling the files created in the respective pageDirectories.
