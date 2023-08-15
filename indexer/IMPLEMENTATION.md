# CS50 TSE Indexer
## Implementation Spec
In this document we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions. The knowledge unit noted that an implementation spec may include many topics; not all are relevant to the TSE or the Indexer. Here we focus on the core subset:

* Data structures
* Control flow: pseudo code for overall flow, and for each of the functions
* Detailed function prototypes and their parameters
* Error handling and recovery
* Testing plan

### Data structures
The main data structure of the indexer is the _index_ which is a 'hashtable' with words as keys and 'counters' as items. The _hashtable_ itself is an array of pointers to 'sets'. Additionally, the words we use in the 'hashtable' come from _webpages_. Thus, here are all the structs of the relevant data structures:

#### index
```c
typedef struct index {
  hashtable_t* hashtable;
} index_t;
```

#### hashtable
```c
typedef struct hashtable {
  int num_slots;
  struct set_t** table;
	pointers to sets that make up the hashtable
} hashtable_t;
```

#### set
```c
typedef struct setnode {
  char* key;
  void* item;
  struct setnode *next;
} setnode_t;
```

#### webpages
```c
typedef struct webpage {
  char* url;
  char* html;
  size_t html_len;
  int depth;
} webpage_t;
```

### Control flow
The Indexer is implemented in one file `indexer.c` using three functions and has helper methods/modules in `pagedir.c`, `word.c`, and `index.c`. The Indexer is then tested with the program `indextest.c` and `testing.sh`.

#### Indexer

##### main
Pseudocode:
```
parse the command line, validate parameters, initialize other modules
call indexBuild, with pageDirectory
```
* Command-line arguments should be of the syntax:
	* `./indexer pageDirectory indexFilename`
* Call `pagedir_validate` on _pageDirectory_ to make sure it is a crawler directory
	* exit program if not
* Check to see that _indexFilename_ can be opened or created
	* exit program if not

##### indexBuild
Pseudocode:
```
creates a new 'index' object
  loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage
```

##### indexPage
Pseudocode:
```
steps through each word of the webpage,
   skips trivial words (less than length 3),
   normalizes the word (converts to lower case),
   looks up the word in the index,
     adding the word to the index if needed
   increments the count of occurrences of this word in this docID
```

### Other modules
#### pagedir
We build upon the module pagedir.c to add functions to support indexer.c processes. Functions like `pagedir_validate(dir)` to verify whether `dir` is indeed a Crawler-produced directory, and `pagedir_load` to load a page from a file in that directory.

Pseudocode for `pagedir_validate`:
```
check if the pageDirectory is NULL
	if so return false
allocate memory for a filename
create a filename of the directory with ".crawler" at the end
open the file to read
if the file is NULL
	close the file
	free the filename
	return false
else
	close the file
	free the filename
	return true
```
Pseudocode for `pagedir_load`:
```
check if the pageDirectory and docID are valid paramaters
	return NULL if not
allocate memory for a filename
create a file name with the directory/docID
open the file
if the file is NULL
	free the filename
	return NULL
else
	read the first line of the file and assign it to the url
	read the second line of the file and assign it to pageDepth
	read the rest of the file and assign it to the html
	close the file
	free the filename
	return a new webpage with the url, pageDepth, and html
```

#### word
Module that ignores words with fewer than three characters, and "normalize" the word before indexing using the `normalizeWord` function. (Here, "normalize" means to convert all letters to lower-case.)

Pseudocode for `normalizeWord`:
```
check that the word not NULL
	return NULL if so
for each index of the char* word
	make every index of the char* lowercase
return the word
```

#### index
We use an `index` module to the `common` library – which is a module to implement an abstract `index_t` type that represents an index in memory, and supports functions 
1. `index_new()` - create a new index
2. `index_insert()` - insert a word into the index
3. `index_load()` - load an index from a index filename
4. `index_write()` - write the contents of the index into the new index filename
5. `index_delete()` - once done, delete the index and its contents
Much of it is a wrapper for a hashtable.

Pseudocode for `index_new`:
```
allocate a new index struct memory
if the new index is NULL
	delete the index
	return NULL
else
	instantiate a new hashtable in the index
	if the hashtable is NULL
		delete the index and hashtable
		return NULL
	else
		return new index
```

Pseudocode for `index_insert`:
```
check that all of the parameters are valid
	return false if not
set a counter to the item that word maps to in the hashtable
if this counter is NULL
	create a new counter
	insert the word into the hashtable
		return false if insert fails
else
	add the word counter to counter for the docID
	return true
```

Pseudocode for `index_load`:
```
check that the file is not NULL
	return null if it is
open the file to read
if NULL
	print error to stderr
	close file
	return NULL
set size of hashtable of the index to the number of lines plus one
while there are words in the file
	while there are pairs of ints on the rest of the line
		scan the lines for pairs of ints 
		put these values as docID and counter
		for every counter
		insert into the index
	free the word
close the file
```

Pseudocode for `index_write`:
```
Check if the index is NULL
	return false if so
Check that the new index file can be opened/created
	print error to stderr and exit program if not
Print out the contents of the index into the new index file using iteration
close the file
return true
```

Pseudocode for `index_delete`:
```
if index is not NULL
	delete the contents of the hashtable
	free the index
```

#### libcs50
We leverage the modules of libcs50, most notably `counter`, `hashtable`, and `webpage`. See that directory for module interfaces. The new `webpage` module allows us to represent pages as webpage_t objects, to fetch a page from the Internet, and to put its words into the `hashtable`

### Function prototypes
#### indexer
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in indexer.c and is not repeated here.
```c
int main(const int argc, char* argv[]);
void indexBuild(char* pageDirectory, char* indexFilename);
void indexPage(index_t* index, webpage_t* webpage, int docID);
```

#### pagedir
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in pagedir.h and is not repeated here.
```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* pageDirectory, int docID);
```

#### word
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in word.h and is not repeated here.
```c
char* normalizeWord(char* word);
```

#### index
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in index.h and is not repeated here.
```c
index_t* index_new();
bool index_insert(index_t* index, const char* word, int docID);
bool index_write(index_t* index, char* newIndexFilename);
void index_delete(index_t* index);
```

#### indextest
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in indextest.c and is not repeated here.
```c
int main(const int argc, const char* argv[]);
```

### Error handling and recovery
All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the mem_assert functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the mem_assert functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally

### Testing Plan
Here is an implementation-specific testing plan.

#### Unit testing 
A program _indextest_ will serve as a unit test for the _index_ module; it reads an index file into the internal _index_ data structure, then writes the index out to a new index file.

#### Integration testing
The _indexer_, as a complete program, will be tested by building an index from a pageDirectory, and then the resulting index will be validated by running it through the _indextest_ to ensure it can be loaded.

1. Use the script `testing.sh` to test `indexer` with various invalid arguments. 2. no arguments 3. one argument 4. three or more arguments 5. invalid `pageDirectory` (non-existent path) 5. invalid `pageDirectory` (not a crawler directory) 6. invalid `indexFile` (non-existent path)
2. Run _indexer_ on a variety of pageDirectories and use _indextest_ as one means of validating the resulting _index_.
3. Run _valgrind_ on both _indexer_ and _indextest_ to ensure no memory leaks or errors.