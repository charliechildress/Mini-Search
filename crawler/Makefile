# Makefile for CS50 crawler program
#
# Charlie Childress, February 2022

############## make all libs and programs ##########
L = ../libcs50
C = ../common

PROG = crawler
OBJS = crawler.o $C/pagedir.o
LIBS = $C/common.a $L/libcs50.a


CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I$C
CC = gcc
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: $L/webpage.h $L/file.h $L/mem.h $L/hashtable.h $L/bag.h $C/pagedir.h 


# 'phony' targets are helpful but do not create any file by that name
.PHONY: test valgrind clean

############## test ##########
valgrind: $(PROG)
	$(VALGRIND) ./crawler

test:
	bash -v testing.sh &> testing.out

############## clean  ##########
clean:
	rm -f $(PROG)
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -r testDirectory || true
	rm -f core