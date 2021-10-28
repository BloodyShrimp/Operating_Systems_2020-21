# Makefile

CC		:= gcc
CFLAGS	:= -Wall -std=c99 -pedantic -O3
LDFLAGS	:= -s -lm
LDLIBS	:=

.PHONY: all clean

all: main.x kopiuj.x procesy.x lider.x

%.x: %.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	$(RM) *.o *.x
	
main.o: main.c procinfo.h
main.x: main.o procinfo.o
procfinfo.o: procinfo.c procinfo.h
kopiuj.o: kopiuj.c
kopiuj.x: kopiuj.o
procesy.o: procesy.c procinfo.h
procesy.x: procesy.o procinfo.o
lider.o: lider.c procinfo.h
lider.x: lider.o procinfo.o