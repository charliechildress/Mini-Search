#!/bin/bash
#
# testing.sh - execute a sequence of commands that demonstrate indexer.c works
#
# usage:
#   Different indexer functions with various number of test cases
# output:
#   The print statements of various indexer.c processes
#   Files with indexer.c outputs
#
# Charlie Childress, February, cs50
#

##### Errors #####
# zero arguments
./indexer 

# one argument
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-0

# too many arguments
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-0 ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-1 errorFile1

# invalid pageDirectory (non-existent path)
./indexer nonexistentDirectory errorFile2

# invalid pageDirectory (not a crawler directory)
./indexer indexer errorFile3

# invalid indexFile (non-existent path)
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-0 ../crawler/indexer/fakefile


##### letters #####

## indexer ##
# indexer letters depth 0
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-0 testFile1

# indexer letters depth 1
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-1 testFile2

# indexer letters depth 2
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-2 testFile3

# indexer letters depth 3
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-3 testFile4

# indexer letters depth 10
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-10 testFile5

## indextest ##
# indextest letters depth 0
./indextest testFile1 testerFile1

# indextest letters depth 1
./indextest testFile2 testerFile2

# indextest letters depth 2
./indextest testFile3 testerFile3

# indextest letters depth 3
./indextest testFile4 testerFile4

# indextest letters depth 10
./indextest testFile5 testerFile5


##### toscrape #####
## indexer ##
# indexer toscrape depth 1
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-toscrape-depth-1 testFile7

# indexer toscrape depth 2
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-toscrape-depth-2 testFile8


## indextest ##
# indextest toscrape depth 1
./indextest testFile7 testerFile7

# indextest toscrape depth 2
./indextest testFile8 testerFile8


##### wikipedia #####
## indexer ##
# indexer wikipedia depth 1
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-wikipedia-depth-1 testFile9


## indextest ##
# indextest wikipedia depth 1
./indextest testFile9 testerFile9


##### valgrind #####
# indexer
valgrind --leak-check=full --show-leak-kinds=all ./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-1 valgrindFile

# indextester
valgrind --leak-check=full --show-leak-kinds=all ./indextest valgrindFile valgrindFile2