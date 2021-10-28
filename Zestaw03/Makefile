# Makefile

CC		:= gcc
CFLAGS	:= -Wall -std=c99 -O3
LDFLAGS	:= -s -lm
LDLIBS	:=

.PHONY: all clean

all: obsluga.x wysylaj.x grupa.x

%.x: %.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	$(RM) *.o *.x
	
procfinfo.o: procinfo.c procinfo.h
obsluga.o: obsluga.c procinfo.h
obsluga.x: obsluga.o procinfo.o
wysylaj.o: wysylaj.c procinfo.h
wysylaj.x: wysylaj.o procinfo.o
grupa.o: grupa.c procinfo.h
grupa.x: grupa.o procinfo.o