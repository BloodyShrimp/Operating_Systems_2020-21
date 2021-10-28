#include "sem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

char sem_name[] = "/semafor";

void exit_remove()
{
    semRemove(sem_name);
}

void handler(int signum)
{
    semRemove(sem_name);
    _exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[]) // 1 - nazwa programu, 2 - liczba procesów, 3 - liczba sekcji krytycznych
{
    if (argc != 4)
    {
        printf("Zła liczba argumentów.\n");
        exit(EXIT_FAILURE);
    }

    if (atexit(exit_remove) != 0)
    {
        printf("atexit() error");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, handler) == SIG_ERR)
    {
        perror("signal() error");
        _exit(EXIT_FAILURE);
    }

    int process_amount = atoi(argv[2]);
    int critical_amount = atoi(argv[3]);
    int i = 0;
    int children_amount = 0;
    char buffer[10] = "";
    int numer = 0;
    int filedesc;

    filedesc = open("./numer.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (filedesc == -1)
    {
        perror("open() error");
        _exit(EXIT_FAILURE);
    }

    if (write(filedesc, "0", sizeof(char)) == -1)
    {
        perror("write() error");
        _exit(EXIT_FAILURE);
    }
    if (close(filedesc) == -1)
    {
        perror("close() error");
        _exit(EXIT_FAILURE);
    }

    semid semaphore_id = semCreate(sem_name, 1);
    semInfo(semaphore_id);
    printf("PID = %d | nazwa semafora: %s | adres: %p\n", getpid(), sem_name, (void *)semaphore_id);
    semClose(semaphore_id);

    for (i = 0; i < process_amount; i++)
    {
        switch (fork())
        {
        case -1:
            perror("fork() error");
            exit(EXIT_FAILURE);
        case 0:
            if (execl(argv[1], argv[1], argv[3]) == -1)
            {
                perror("execl() error");
                exit(EXIT_FAILURE);
            }
        default:
            children_amount++;
            break;
        }
    }

    for (i = 0; i < children_amount; i++)
    {
        wait(NULL);
    }

    filedesc = open("./numer.txt", O_RDONLY);
    if (filedesc == -1)
    {
        perror("open() error");
        exit(EXIT_FAILURE);
    }

    if (read(filedesc, buffer, 10 * sizeof(char)) == -1)
    {
        perror("read() error");
        exit(EXIT_FAILURE);
    }

    numer = atoi(buffer);
    printf("Numer w pliku: %d   ", numer);
    if (numer == process_amount * critical_amount)
    {
        printf("Wartość zgodna z oczekiwaną.\n");
    }
    else
    {
        printf("Wartość niezgodna z oczekiwaną.\n");
    }

    if (close(filedesc) == -1)
    {
        perror("close() error");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}