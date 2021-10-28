#include "sem.h"
#include "shmem.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

semid sem_prod_id;
semid sem_kons_id;
int mem_desc;
cykliczny *mem_ptr;

void exit_remove() //funkcja używana w atexit()
{
    semClose(sem_prod_id);
    semRemove(sem_prod);
    semClose(sem_kons_id);
    semRemove(sem_kons);
    shmClose(mem_ptr, mem_desc, mem_size);
    shmRm(mem_name);
}

void handler(int signum) //handler sygnału ctrl + C w razie potrzeby manualnego zakończenia programu
{
    semClose(sem_prod_id);
    semRemove(sem_prod);
    semClose(sem_kons_id);
    semRemove(sem_kons);
    shmClose(mem_ptr, mem_desc, mem_size);
    shmRm(mem_name);
    _exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[]) // 1 - source, 2 - target
{
    if (atexit(exit_remove) != 0) //ustawienie funkcji wyjścia
    {
        printf("atexit() error");
        _exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, handler) == SIG_ERR) //ustawienie obsługi sygnału ctrl + C
    {
        perror("signal() error");
        _exit(EXIT_FAILURE);
    }

    if(argc != 3)
    {
        printf("Zła liczba argumentów.\n");
        _exit(EXIT_FAILURE);
    }

    sem_prod_id = semCreate(sem_prod, CAPACITY); //utworzenie semafora producenta o wartości CAPACITY zdefiniowanej w nagłówku shmem.h
    printf("Wartość semafora producenta: ");
    semInfo(sem_prod_id);
    sem_kons_id = semCreate(sem_kons, 0); //utworzenie semafora konsumenta o wartości 0
    printf("Wartośc semafora konsumenta: ");
    semInfo(sem_kons_id);
    mem_desc = shmCreate(mem_name, mem_size); //utworzenie pamięci dzielonej
    shmInfo(mem_desc);
    mem_ptr = (cykliczny *)shmMap(mem_desc, mem_size); //mapowanie pamięci dzielonej
    mem_ptr->size = CAPACITY;
    mem_ptr->head = 0;
    mem_ptr->tail = 0;

    switch (fork()) //producent
    {
    case -1:
        perror("fork() error");
        exit(EXIT_FAILURE);
    case 0:
        if (execl("./producent.x", "producent.x", argv[1]) == -1)
        {
            perror("producent execl() error");
            exit(EXIT_FAILURE);
        }
    default:
        break;
    }

    switch (fork()) //konsument
    {
    case -1:
        perror("fork() error");
        exit(EXIT_FAILURE);
    case 0:
        if (execl("./konsument.x", "konsument.x", argv[2]) == -1)
        {
            perror("konsument execl() error");
            exit(EXIT_FAILURE);
        }
    default:
        break;
    }

    wait(NULL);
    wait(NULL);

    exit(EXIT_SUCCESS);
}