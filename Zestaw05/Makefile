# Makefile

CC		:= gcc
CFLAGS	:= -Wall -std=c99 -pedantic -O3
LDFLAGS	:= -s -lm
LDLIBS	:=

.PHONY: all clean

all: producent.x konsument.x fifo.x

%.x: %.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	$(RM) *.o *.x
	
producent.o: producent.c
producent.x: producent.o
konsument.o: konsument.c
konsument.x: konsument.o
fifo.o: fifo.c
fifo.x: fifo.o