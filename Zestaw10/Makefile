# Makefile

CC		:= gcc
CFLAGS	:= -pthread -Wall -std=c99 -pedantic -c -o
LDFLAGS	:= -pthread -s -o
LDLIBS	:= -L. -lmythread -lrt
DYNLIB	:= -Wl,-rpath ./

.PHONY: all clean cleano

all: bakery.x counter.x

%.x: %.o
	$(CC) $(LDFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $@ $<

clean:
	$(RM) *.o *.x *.a *.so

cleano:
	$(RM) *.o

bakery.o: bakery.c
bakery.x: bakery.o
counter.o: counter.c
counter.x: counter.o