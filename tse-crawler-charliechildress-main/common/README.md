# CS50 Tiny Search Engine (TSE) utility library

We create a re-usable module `pagedir.c` to handles the _pagesaver_ mentioned in the design (writing a page to the pageDirectory), and marking it as a Crawler-produced pageDirectory (as required in the spec). We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place.

For the `word.c` module, we normalize words given by
indexer. _Normalize_ just means that we change the 
word to all lowercase.

In the `index.c` module implement an abstract index_t type that represents an index in memory, and supports functions like `index_new()`, `index_delete()`, `index_write()`.

...anticipating future use by the Querier.

## Usage

To build `common.a`, run `make`. 

To clean up, run `make clean`.

## Overview

 * `pagedir.c` - pagedir program to intial and save page directories
 * `pagedir.h` - pagedir.c interface
 * `word.c` - normalizes words to lowercase
 * `word.h` - word.c interface
 * `index.c` - represent an index and support functions for it
 * `index.h` - index.c interface
 * `Makefile` - compilation procedure