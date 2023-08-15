# Makefile for CS50 indexer program
#
# Charlie Childress, February 2022

############## make all libs and programs ##########
L = ../libcs50
C = ../common

PROG1 = indexer 
PROG2 = indextest
OBJS1 = indexer.o $C/index.o $C/word.o 
OBJS2 = indextest.o $C/index.o $C/word.o 
LIBS = $C/common.a $L/libcs50.a


CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: indexer indextest

$(PROG1): $(OBJS1)
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@

$(PROG2): $(OBJS2)
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@

indexer.o: $L/webpage.h $L/file.h $L/mem.h $L/hashtable.h $L/bag.h $C/pagedir.h $C/index.h 
indextest.o: $L/webpage.h $L/file.h $L/mem.h $L/hashtable.h $L/bag.h $C/pagedir.h $C/index.h 

# 'phony' targets are helpful but do not create any file by that name
.PHONY: test valgrind clean

############## test ##########
test:
	bash -v testing.sh &> testing.out

############## clean  ##########
clean:
	rm -f $(PROG)
	rm -rf *.dSYM # MacOS debugger info
	rm -r testFile* || true
	rm -r testerFile* || true
	rm -r valgrindFile* || true
	rm -r indexer
	rm -r indextest
	rm -f *~ *.o
	rm -f core