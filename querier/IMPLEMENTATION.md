# CS50 TSE Querier
## Implementation Spec

In this document we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions. The knowledge unit noted that an implementation spec may include many topics; not all are relevant to the TSE or the Querier. Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the major functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures
The main data structure of the *indexer* is the _index_ which is a 'hashtable' with words as keys and 'counters' as items. We use the _index_ in the *querier* to create the _allcounters_ data structure which allows us to track the intersection between a *counter_t* for all previous 'counters' and a *counter_t* for the new 'counters'. We also use an _array_ data structure to store the array of 'counters' which represent the set of documents in the query and also store the number of items in the 'counters'.

### index
```c
typedef struct index {
  hashtable_t* hashtable;
} index_t;
```

### hashtable
```c
typedef struct hashtable {
  int num_slots;
  struct set_t** table;
	pointers to sets that make up the hashtable
} hashtable_t;
```

### counters
```c
typedef struct counters {
  struct countersnode* head;
} counters_t;
```

### allcounters
```c
typedef struct allcounters {
  counters_t* ctrs1;
  counters_t* ctrs2;
} allcounters_t;
```

### array
```c
typedef struct array {
  counter_t** countersarray;
  int* docIDs;
} array_t;
```

## Control flow 
The *Querier* is implemented in one file `querier.c` using a total of 18 functions and has a helper methods/modules in `index.c`. The *Querier* is then tested with the program `fuzzquery.c` and `testing.sh` using three different helper test files.

Since most of the methods in *Querier* are helper methods mostly for iterating through counters, we will be giving pseudocode for the major methods.

### Querier

#### main
Pseudocode:
```
parse the command line
if there are too many or too few argument
  print error message to stderr
  exit
validate parameters
if there is an error with the parameters
  print according error message
  exit
if the parameters are valid
  load an index from the indexFilename
  call querier with index and pageDirectory
else 
  exit
free all memory
exit with zero status if not errors occured
```
* Command-line arguments should be of the syntax:
	* `./querier pageDirectory indexFilename`
* Call `pagedir_validate` on _pageDirectory_ to make sure it is a crawler directory
	* exit program if not
* Check to see that _indexFilename_ can be opened or created
	* exit program if not

#### querier
Pseudocode:
```
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
if no errors occured, return true to show that querier was successful
```

#### verifyString
Pseudocode:
```
if query is NULL
  print error to stderr
  return false
else
  for every character in the query
    if it is a character or space
      continue to the next character
    else
      print error to stderr referring to the bad character
      return false
return true
```

#### tokenize
Pseudocode:
```
Takes a char* for the query that we want to tokenize and
int* which is a pointer that will track how large the 
wordArray is for later methods
```

#### verifyArray
Pseudocode:
```
make sure the literal 'and' comes between words, 
and the literal 'or' must come between andsequences, so they cannot appear at the beginning or end of a query. 
Also check that the literals ('and' & 'or') are not  adjacent
```

#### querySequence
Pseudocode:
```
Check that the parameters are not NULL
For each word from the array
  if it is "or" 
    find the intersection between that word and the total counter
    put this value into the total counter
return the total counter
```

#### andSequence
Pseudocode:
```
Check that the parameters are not NULL
For each word from the first index to the last index
  if it is not "and" 
    find the intersection between that word and the total counter
    put this value into the total counter
return the total counter
```

#### intersection
Pseudocode:
```
create memory for a intersection counter
 Creates an allcounters struct from the intersection counter and second counter
 iterate over the two counters comparing them and putting their intersection into the intersection counter
 delete the first counter
 return the intersection counter
```

#### combination
Pseudocode:
```
create memory for a combination counter
iterate over the first counter, putting its data into the combination counter
iterate over the second counter, putting its sata into the combination counter
delete the first counter
if told
  delete the second counter
return the combination counter
```

## Other modules
We only create one small getter method `index_get_hashtable(index_t* index)` inside of `index.c`. Other than this we extensively use methods from other modules like using file.h methods in the libcs50 module to read and go through files, index.h methods in the common module to load in the index and use the *index_t* data structure. Moreover, *Querier* is the final part of a three part lab which depends on *Crawler* to crawl through all of the webpages, putting them into a directory and *Indexer* to take the words from the directory and put them into an index which *Querier* ultimately uses. 

## Function prototypes

### querier

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in querier.c and is not repeated here.

```c
/**************** global functions ****************/
/* that is, visible outside this file */
int main(const int argc, char* argv[]);
bool querier(index_t* index, char* pageDirectory);
bool verifyString(char* query);
char** tokenize(char* query, int* size);
bool verifyArray(char** wordArray, int size);
counters_t* querySequence(char** wordArray, int size, hashtable_t* ht);
counters_t* andSequence(char** wordArray, int firstIndex, int lastIndex, hashtable_t* ht);
counters_t* intersection(counters_t* ctrs1, counters_t* ctrs2);
counters_t* combination(counters_t* ctrs1, counters_t* ctrs2, bool free);
int fileno(FILE *stream);

/**************** local functions ****************/
/* not visible outside this file */
static void docs_iterate(void *arg, const int key, const int count);
static void counters_array_iterate(void* arg, const int key, const int count);
static void counters_counters_iterate(void *arg, const int key, const int count);
static void combination_iterate(void* arg, const int key, const int count);
static void intersection_iterate(void* arg, const int key, const int count);
static void printArray(array_t* array, char* pageDirectory);
static void print_iterate(void* arg, const int key, const int count);
static void deleteArray(array_t* array);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_malloc_assert` functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_malloc_assert` functions), e.g., if a function receives bad parameters.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

*Unit testing*. Use a program called `fuzzquery.c` for testing *querier*. `fuzzquery.c`, a fuzz-testing program, generates a series of random queries on stdout, which it then pipes to the querier on stdin.

### Integration/system testing

*Integration testing*. *Querier* reads queries from stdin, one per line. For thorough and repeated testing a collection of little files is used, each of which contains one or more queries for the *querier*, and run commands like `./querier ... < testquery`. For this, there is a bash script `testing.sh` to run the *querier* through several such test files.

### Note

Will be using provided *indexer* and *crawler* outputs from a shared directory `~/cs50-dev/shared/tse/output/` to test *querier*. This will help to ensure that any problems in testing come from my `querier.c` rather than any other source of code.
