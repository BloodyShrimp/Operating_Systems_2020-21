# Makefile

CC		:= gcc
CFLAGS	:= -pthread -Wall -std=c99 -pedantic
LDFLAGS	:= -pthread -s
LDLIBS	:= -L. -lsem -lrt

.PHONY: all clean

all: semuse.x prog.x powielacz.x

sem.o: sem.c
	gcc -c sem.c

libsem.a: sem.o
	ar rcv libsem.a sem.o

%.x: %.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDLIBS)

clean:
	$(RM) *.o *.x *.a *.txt
	

semuse.o: semuse.c sem.h libsem.a
semuse.x: semuse.o
prog.o: prog.c sem.h libsem.a
prog.x: prog.o
powielacz.o: powielacz.c sem.h libsem.a
powielacz.x: powielacz.o