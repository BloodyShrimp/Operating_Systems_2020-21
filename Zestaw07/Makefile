# Makefile

CC		:= gcc
CFLAGS	:= -pthread -Wall -std=c99 -pedantic -c -o
LDFLAGS	:= -pthread -s -o
LDLIBS	:= -L. -lshmem -lsem -lrt
DYNLIB	:= -Wl,-rpath ./

.PHONY: all clean cleano

all: shmuse1.x shmuse2.x glowny.x producent.x konsument.x

shmem.o: shmem.c
	gcc -c -O -fPIC shmem.c

lib%.so: %.o
	$(CC) -shared -o $@ $^

sem.o: sem.c
	$(CC) -c sem.c

lib%.a: %.o
	ar rcv $@ $^

%1.x: %.o
	$(CC) $(LDFLAGS) $@ $^ $(LDLIBS)

%2.x: %.o
	$(CC) $(LDFLAGS) $@ $^ $(LDLIBS) $(DYNLIB)

%.x: %.o
	$(CC) $(LDFLAGS) $@ $^ $(LDLIBS) $(DYNLIB)

%.o: %.c
	$(CC) $(CFLAGS) $@ $<

clean:
	$(RM) *.o *.x *.a *.so

cleano:
	$(RM) *.o
	

shmuse.o: shmuse.c shmem.h libshmem.so sem.h libsem.a
shmuse1.x: shmuse.o
shmuse2.x: shmuse.o
libshmem.so: shmem.o
libsem.a: sem.o
glowny.o: glowny.c shmem.h libshmem.so sem.h libsem.a
glowny.x: glowny.o
producent.o: producent.c shmem.h libshmem.so sem.h libsem.a
producent.x: producent.o
konsument.o: konsument.c shmem.h libshmem.so sem.h libsem.a
konsument.x: konsument.o