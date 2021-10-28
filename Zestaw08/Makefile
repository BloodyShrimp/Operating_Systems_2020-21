# Makefile

CC		:= gcc
CFLAGS	:= -Wall -std=c99 -pedantic -c -o
LDFLAGS	:= -s -o
LDLIBS	:= -L. -lmsgdyn -lrt
DYNLIB	:= -Wl,-rpath ./

.PHONY: all clean cleano

all: serwer.x klient.x

msgdyn.o: msg.c
	gcc -c -O -fPIC $^ -o $@

lib%.so: %.o
	$(CC) -shared -o $@ $^

msgstat.o: msg.c
	$(CC) -c $^ -o $@

lib%.a: %.o
	ar rcv $@ $^

%.x: %.o
	$(CC) $(LDFLAGS) $@ $^ $(LDLIBS) $(DYNLIB)

%.o: %.c
	$(CC) $(CFLAGS) $@ $<

clean:
	$(RM) *.o *.x *.a *.so

cleano:
	$(RM) *.o
	

libmsgdyn.so: msgdyn.o
libmsgstat.a: msgstat.o
serwer.o: serwer.c msg.h libmsgdyn.so libmsgstat.a
serwer.x: serwer.o
klient.o: klient.c msg.h libmsgdyn.so libmsgstat.a
klient.x: klient.o