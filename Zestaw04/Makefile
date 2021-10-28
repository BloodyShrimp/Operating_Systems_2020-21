# Makefile

CC		:= gcc
CFLAGS	:= -Wall -std=c99 -pedantic -O3
LDFLAGS	:= -s -lm
LDLIBS	:=

.PHONY: all clean

all: prodkons.x

%.x: %.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	$(RM) *.o *.x
	
prodkons.o: prodkons.c
prodkons.x: prodkons.o