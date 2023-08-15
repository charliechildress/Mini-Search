# CS50 Tiny Search Engine (TSE) Lab 6 Querier

The TSE *Querier* is a standalone program that reads the index file produced by the TSE *Indexer*, and page files produced by the TSE *Querier*, and answers search queries submitted via stdin.

## Assumptions

No assumptions or implementations were made beyond what the specs provide

## Usage

To compile, simply `make`.

To test, simply `make test`.

To clean up, run `make clean`.

## Overview

 * `querier.c` - querier program
 * `fuzzquery.c` - fuzz-testing program, for testing querier
 * `testing.sh` - tests for querier
 * `testing.out` - result of testing.sh
 * `Makefile` - compilation procedure
 * `DESIGN` - design specs for querier program
 * `IMPLEMENTATION` - implementation specs for querier program

## Testing

*Unit testing*. Use a program called `fuzzquery.c` for testing *querier*. `fuzzquery.c`, a fuzz-testing program, generates a series of random queries on stdout, which it then pipes to the querier on stdin.

*Integration testing*. *Querier* reads queries from stdin, one per line. For thorough and repeated testing a collection of little files is used, each of which contains one or more queries for the *querier*, and run commands like `./querier ... < testquery`. For this, there is a short bash script to run the *querier* through several such test files.

Will be using provided *indexer* and *crawler* outputs from a shared directory `~/cs50-dev/shared/tse/output/` to test *querier*. This will help to ensure that any problems in testing come from my `querier.c` rather than any other source of code.
