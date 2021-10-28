#define _XOPEN_SOURCE 500
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int shmCreate(const char *name, int size)
{
    printf("Tworzenie pamięci dzielonej %s\n", name);
    int descriptor = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0660);
    if (descriptor == -1)
    {
        perror("shm_open() error");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(descriptor, size) == -1)
    {
        perror("ftruncate() error");
        exit(EXIT_FAILURE);
    }

    return descriptor;
}

int shmOpen(const char *name)
{
    printf("Otwieranie pamięci dzielonej %s\n", name);
    int descriptor = shm_open(name, O_RDWR, 0000);
    if (descriptor == -1)
    {
        perror("shmOpen() error");
        exit(EXIT_FAILURE);
    }

    return descriptor;
}

void shmRm(const char *name)
{
    printf("Usuwanie pamięci dzielonej %s\n", name);
    if (shm_unlink(name) == -1)
    {
        perror("shmRM() error");
        exit(EXIT_FAILURE);
    }
}

void *shmMap(int fd, int size)
{
    printf("Mapowanie pamięci dzielonej.\n");
    char *adress = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (adress == MAP_FAILED)
    {
        perror("shmMap() error");
        exit(EXIT_FAILURE);
    }

    return adress;
}

void shmClose(void *ptr, int fd, int size)
{
    printf("Zamykanie pamięci dzielonej.\n");
    if (munmap(ptr, size) == -1)
    {
        perror("munmap() error");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1)
    {
        perror("close() error");
        exit(EXIT_FAILURE);
    }
}

int shmSize(int fd)
{
    printf("Sprawdzanie rozmiaru pamięci dzielonej.\n");
    struct stat info;
    if (fstat(fd, &info) == -1)
    {
        perror("fstat() error");
        exit(EXIT_FAILURE);
    }

    int size = info.st_size;
    return size;
}

void shmInfo(int fd)
{
    printf("Sprawdzanie informacji o pamięci dzielonej.\n");
    struct stat info;
    if (fstat(fd, &info) == -1)
    {
        perror("fstat() error");
        exit(EXIT_FAILURE);
    }

    int permissions = info.st_mode % 512;
    int size = info.st_size;
    int ownerID = info.st_uid;

    printf("Prawa dostępu: 0%o\nRozmiar: %d\nWłaściciel: %d\n", permissions, size, ownerID);
}

int my_random(int min, int max) //funkcja losująca numer z zakresu [min, max]
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}