# CS50 TSE Querier
## Design Spec

In this document we reference the Requirements Specification and focus on the implementation-independent design decisions. The knowledge unit noted that an design spec may include many topics; not all are relevant to the TSE or the Querier. Here we focus on the core subset:

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan

## User interface

As described in the Requirements Spec, the querier's interface is the querier reads queriers from stdin, one per line. These queries must have two arguments:     
  1. pageDirectory - the pathname of a directory produced by the Crawler
  2. indexFilename - the pathname of a file produced by the Indexer

```bash
$ querier pageDirectory indexFilename
```

## Inputs and outputs

*Input*: There are no file inputs; there are command-line parameters described above.

*Output*: Per the requirements spec, Querier reads search queriers from stdin, one per line, until EOF first printing an error message if the query is invalid, nothing if the query is empty, or a _'clean' query_ for the user to see after each input. If the query is valid, Querier prints the set of documents according to its score or `No documents match.` if no documents satisfy the query

## Functional decomposition into modules

We anticipate the following module of functions:

1. main, which parses arguments and initializes other modules

2. querier, main function that calls the other functions and makes sure they all work properly

3. verifyString, checks that there are only letters and spaces in the query

4. tokenize, normalizes the words in the query and creates an array out of them

5. verifyArray, checks that the literals "and" and "or" do not appear at the beginning or end of the query and are not adjacent

6. printQuery, print out the each document, its score, and url to stdout in order of score

7. querySequence, creates counters that contains all documents that match the query and their counters

8. andSequence, creats counter for all documents in the sequence that share a word in the query and their counters

9. fileno, prints a prompt to stdout iff stdin is a terminal

## Pseudo code for logic/algorithmic flow

The querier will run as follows:

```
parse the command line, validate parameters, intialize other modules
load an already created index from indexFilename
get hashtable from index
while there are lines in stdin
  each line is a query
  verify if that the query consists of only strings and letters
  if not
    go to the next query
  now tokenize the query, creating an array of words from it
  print out the tokenized query
  very the array of words has "and" or "or" in the right place
  if not
    print error message
    go to the next query
  calculate the query sequence for the score of each document that matches the query
  order the query based on score
  print the ordered query
  delete and free all the memory
  go on to the next query
```

## Major data structures

Helper modules provide all the data structures we need:

- *index* which is a *hashtable* of words with the docID to represent the file in the crawler directory that contains each word and a corresponding counter for how many times the word appears in the docID
- *allcounters* a counter that holds two counters
- *array* an array of pointers to counters
- *counters* the base of the data structure in querier holds (docID, count) pairs with the count being the number of matches the doc has with the query

## Testing plan

A sampling of tests that should be run:

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Use a simple query on a closed set of cross-linked web pages like letters, at depths 0, 1, 2, or more. Verify that the query output created matches expectations.

3. Now try a similar query on the same web pages, but now incorporate an array of and/or operators to see how querier handles these.

4. Point the querier at one of the bigger playgrounds. Explore at greater depths on larger indexes such as for toscrape and wikipedia. Use simple queries and verify that the files created match expectations.

5. Finally when confident that the querier runs well, test it at greater depths and larger indexes, and now try an array of and/or operators.
