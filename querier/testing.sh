#!/bin/bash
#
# testing.sh - execute a sequence of commands that demonstrate querier.c works
#
# usage:
#   Different querier functions with various number of test cases
# output:
#   The print statements of various querier.c processes
#   
#
# Charlie Childress, February, cs50
#


##### Errors With Argument #####
# zero arguments
./querier 

# one argument
./querier ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-0

# too many arguments
./querier ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-0 ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-1 errorFile1

# invalid pageDirectory (non-existent path)
./querier nonexistentDirectory errorFile2

# invalid pageDirectory (not a crawler directory)
./querier querier errorFile3

# invalid indexFilename (non-existent path)
./querier ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-0 ../crawler/querier/fakefile

##### Errors With Queries #####
./querier ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-3 ~/cs50-dev/shared/tse/output/indexer/index-letters-3 < errortest

##### Successful Queries #####
./querier ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-3 ~/cs50-dev/shared/tse/output/indexer/index-letters-3 < successtest

##### valgrind #####
valgrind --leak-check=full --show-leak-kinds=all ./querier ~/cs50-dev/shared/tse/output/crawler/pages-wikipedia-depth-2 ~/cs50-dev/shared/tse/output/indexer/index-wikipedia-2 <valgrindtest

##### Fuzz Tests #####
./fuzzquery ~/cs50-dev/shared/tse/output/indexer/index-toscrape-1 10 0 | ./querier ~/cs50-dev/shared/tse/output/crawler/pages-toscrape-depth-1 ~/cs50-dev/shared/tse/output/indexer/index-toscrape-1

exit 0