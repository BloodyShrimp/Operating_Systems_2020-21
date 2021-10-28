#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "sem.h"

semid semCreate(const char *name, int val)
{
    semid semaphore_id = sem_open(name, O_CREAT | O_EXCL, 0644, val);
    if (semaphore_id == SEM_FAILED)
    {
        perror("semCreate() error");
        exit(EXIT_FAILURE);
    }
    printf("Tworzenie semafora %s. PID = %d\n", name, getpid());

    return semaphore_id;
}

void semRemove(const char *name)
{
    if (sem_unlink(name) == -1)
    {
        perror("semRemove() error");
        exit(EXIT_FAILURE);
    }
    printf("Usuwanie semafora %s.\n", name);
}

semid semOpen(const char *name)
{
    semid semaphore_id = sem_open(name, O_RDWR);
    if (semaphore_id == SEM_FAILED)
    {
        perror("semOpen() error");
        exit(EXIT_FAILURE);
    }
    printf("Otwieranie semafora %s. PID = %d\n", name, getpid());

    return semaphore_id;
}

void semClose(semid sem)
{
    if (sem_close(sem) == -1)
    {
        perror("semClose() error");
        exit(EXIT_FAILURE);
    }
    printf("Zamykanie semafora. PID = %d\n", getpid());
}

void semP(semid sem) //opusczenie
{
    if (sem_wait(sem) == -1)
    {
        perror("semP() error");
        exit(EXIT_FAILURE);
    }
    printf("Opuszczanie semafora.\n");
}

void semV(semid sem) //podniesienie
{
    if (sem_post(sem) == -1)
    {
        perror("semV() error");
        exit(EXIT_FAILURE);
    }
    printf("Podnoszenie semafora.\n");
}

void semInfo(semid sem)
{
    int sval;
    if (sem_getvalue(sem, &sval) == -1)
    {
        perror("semInfo() error");
        exit(EXIT_FAILURE);
    }
    printf("%d\n", sval);
}