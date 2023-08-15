# Makefile for CS50 pagedir program
#
# Charlie Childress, February 2022

############## make all libs and programs ##########

L = ../libcs50

OBJS = pagedir.o index.o word.o
LLIBS = $L/libcs50.a
LIB = common.a

CFLAGS=-Wall -pedantic -std=c11 -ggdb -I$L
CC=gcc
MAKE = make

# Archiving object files and build the library
$(LIB): $(OBJS) $(LLIBS)
	ar cr $(LIB) $(OBJS)


.PHONY: all clean

all: common.a

############## clean  ##########
clean:
	rm -f core
	rm -f $(LIB) *~ *.o