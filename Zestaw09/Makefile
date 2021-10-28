# Makefile

CC		:= gcc
CFLAGS	:= -pthread -Wall -std=c99 -pedantic -c -o
LDFLAGS	:= -pthread -s -o
LDLIBS	:= -L. -lmythread -lrt
DYNLIB	:= -Wl,-rpath ./

.PHONY: all clean cleano

all: wykluczanie.x

%.x: %.o
	$(CC) $(LDFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $@ $<

clean:
	$(RM) *.o *.x *.a *.so

cleano:
	$(RM) *.o

wykluczanie.o: wykluczanie.c
wykluczanie.x: wykluczanie.o