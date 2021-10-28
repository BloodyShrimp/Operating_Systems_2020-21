# Makefile

CC		:= gcc
CFLAGS	:= -Wall -std=c99 -pedantic -O3
LDFLAGS	:= -s -lm
LDLIBS	:=

.PHONY: all clean

all: drzewko1.x drzewko2.x macierzysty.x potomny.x zombi.x orphan.x execute.x

%.x: %.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	$(RM) *.o *.x
	
procfinfo.o: procinfo.c procinfo.h
drzewko1.o: drzewko1.c procinfo.h
drzewko1.x: drzewko1.o procinfo.o
drzewko2.o: drzewko2.c procinfo.h
drzewko2.x: drzewko2.o procinfo.o
macierzysty.o: macierzysty.c procinfo.h
macierzysty.x: macierzysty.o procinfo.o
potomny.o: potomny.c procinfo.h
potomny.x: potomny.o procinfo.o
zombi.o: zombi.c
zombi.x: zombi.o
orphan.o: orphan.c
orphan.x: orphan.o
execute.o: execute.c procinfo.h
execute.x: execute.o procinfo.o