# CS50 Tiny Search Engine (TSE) Lab 5 Indexer

The TSE _indexer_ is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the _index tester_, loads an index file produced by the indexer and saves it to another file.

The indexer writes the inverted index to a file, and both the index tester and the querier read the inverted index from a file; the file shall be in the following format. 

## Assumptions
The index tester assumes that the content of the index file follows the format specified below; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.

The size of the hashtable (slots) is impossible to determine in advance, so we use 200

No other assumptions or implementations beyond what the specs provide

## Usage

To compile, simply `make`.

To test, simply `make test`.

To clean up, run `make clean`.

## Overview

 * `indexer.c` - indexer program
 * `indextest.c` - test for indexer program
 * `testing.sh` - tests for indexer and indextest programs
 * `testing.out` - result of testing.sh
 * `Makefile` - compilation procedure
 * `IMPLEMENTATION` - implementation guide for indexer program

 ## Testing
_Unit testing_. A program _indextest_ will serve as a unit test for the _index_ module; it reads an index file into the internal index data structure, then writes the index out to a new index file.

_Integration testing_. The _indexer_, as a complete program, will be tested by building an index from a pageDirectory, and then the resulting index will be validated by running it through the _indextest_ to ensure it can be loaded.

We will test _indexer_ with various invalid arguments. 2. no arguments 3. one argument 4. three or more arguments 5. invalid `pageDirectory` (non-existent path) 6. invalid `pageDirectory` (not a crawler directory) 7. invalid `indexFile` (non-existent path)
We will also run _indexer_ on a variety of pageDirectories and use _indextest_ as one means of validating the resulting index.
Finally, we will run _valgrind_ on both _indexer_ and _indextest_ to ensure no memory leaks or errors.
