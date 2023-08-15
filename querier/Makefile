# Makefile for CS50 querier program
#
# Charlie Childress, February 2022

############## make all libs and programs ##########
L = ../libcs50
C = ../common

PROG1 = querier 
PROG2 = fuzzquery
OBJS1 = querier.o 
OBJS2 = fuzzquery.o
LIBS = $C/common.a $L/libcs50.a


CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# 'phony' targets are helpful but do not create any file by that name
.PHONY: all test valgrind clean

all: querier fuzzquery

$(PROG1): $(OBJS1) $(LIBS)
	$(CC) $(CFLAGS) $^ -lm -o $@

$(PROG2): $(OBJS2) $(LIBS)
	$(CC) $(CFLAGS) $^ -lm -o $@

querier.o: $L/webpage.h $L/file.h $L/mem.h $L/hashtable.h $L/counters.h $C/pagedir.h $C/index.h $C/word.h
fuzzquery.o: $L/file.h $L/mem.h

############## test ##########
valgrind: $(PROG)
	$(VALGRIND) ./querier

test:
	bash -v testing.sh &> testing.out

############## clean  ##########
clean:
	rm -f $(PROG1)
	rm -f $(PROG2)
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core