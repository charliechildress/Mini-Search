#!/bin/bash
#
# testing.sh - execute a sequence of commands that demonstrate crawler.c works
#
# usage:
#   Different ./crawler functions with various number of test cases
# output:
#   The print statements of various crawler.c processes
#   Directory with all crawler.c output files
#
# Charlie Childress, February, cs50
#

## create directory
mkdir testDirectory 

##### Errors #####
# zero arguments
./crawler

# one argument
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/

# two arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ testDirectory

# too many arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ testDirectory 3

# non-existing url
./crawler http://doesnotexist/ testDirectory 3

# external url
./crawler http://cs50tse.cs.dartmouth.edu testDirectory 3

# depth below lower bound
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ testDirectory -5

# depth above upper bound
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ testDirectory 15


##### Valgrind #####
mkdir testDirectory/valgrind
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ testDirectory/valgrind 2


##### Letters #####
# depth 0
mkdir testDirectory/letters-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ testDirectory/letters-0 0

# depth 1
mkdir testDirectory/letters-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ testDirectory/letters-1 1

# depth 2
mkdir testDirectory/letters-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ testDirectory/letters-2 2

# depth 10
mkdir testDirectory/letters-10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ testDirectory/letters-10 10


##### Toscrape #####
# depth 0
mkdir testDirectory/toscrape-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ testDirectory/toscrape-0 0

# depth 1
mkdir testDirectory/toscrape-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ testDirectory/toscrape-1 1


##### Wikipedia #####
# depth 0
mkdir testDirectory/wikipedia-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ testDirectory/wikipedia-0 0

# depth 1
mkdir testDirectory/wikipedia-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ testDirectory/wikipedia-1 1

# depth 2
mkdir testDirectory/wikipedia-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ testDirectory/wikipedia-2 2
