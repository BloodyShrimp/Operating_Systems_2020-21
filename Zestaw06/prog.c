#define _XOPEN_SOURCE 500
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

int i = 0;
char sem_name[] = "/semafor";

void info(semid semaphore_id) //informacje o semaforze oraz numeru iteracji wejścia do sekcji krytycznej
{
    printf("PID = %d | nr wykonywanej sekcji krytycznej: %d | ", getpid(), i);
    semInfo(semaphore_id);
}

int my_random(int min, int max) //funkcja losująca numer z zakresu [min, max]
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int main(int argc, const char *argv[]) //1 - liczba sekcji krytycznych
{
    int critical_amount = atoi(argv[1]); //zmiena ilości wejścia do sekcji krytycznej
    semid semaphore_id = semOpen(sem_name); //id semafora
    int filedesc = 0; //deskryptor pliku numer.txt
    char buffer[10] = ""; //tablica do funkcji read write
    int numer = 0; //zmienna numeru w pliku numer.txt

    for (i = 1; i <= critical_amount; i++)
    {
        usleep(my_random(100, 999999));
        semP(semaphore_id); // sekcja krytyczna
        printf("    ");
        info(semaphore_id); 
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

        if (close(filedesc) == -1)
        {
            perror("close() error");
            exit(EXIT_FAILURE);
        }

        usleep(my_random(100, 999999));
        numer = atoi(buffer);
        printf("    Numer w pliku: %d\n", numer);
        numer++;
        sprintf(buffer, "%d", numer);
        filedesc = open("./numer.txt", O_WRONLY);
        if (filedesc == -1)
        {
            perror("open() error");
            exit(EXIT_FAILURE);
        }

        if (write(filedesc, buffer, strlen(buffer)) == -1)
        {
            perror("write() error");
            exit(EXIT_FAILURE);
        }

        if (close(filedesc) == -1)
        {
            perror("close() error");
            exit(EXIT_FAILURE);
        }

        //usleep(my_random(100, 999999));
        semV(semaphore_id); //koniec sekcji krytycznej
    }

    semClose(semaphore_id);
}