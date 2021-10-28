#define _XOPEN_SOURCE 500
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>

#define PRODUCENT_PORCJA 8
#define KONSUMENT_PORCJA 8

char buffer_producent[PRODUCENT_PORCJA];
char buffer_konsument[KONSUMENT_PORCJA];
int readbytes = 0;
int writtenbytes = 0;
int i = 0;

void producent(int fd, int sourcedesc);
void konsument(int fd, int targetdesc);
int my_random(int min, int max);

int main(int argc, const char *argv[]) // 1 - source, 2 - target
{
    if (argc != 3)
    {
        printf("Zla liczba argumentow.\n");
        exit(EXIT_FAILURE);
    }
    int fd[2];
    const char *source = argv[1]; //zmienne nazw pliku źrodłowego i docelowego
    const char *target = argv[2];
    int sourcedesc = open(source, O_RDONLY);
    if (sourcedesc == -1)
    {
        perror("open() source error");
        exit(EXIT_FAILURE);
    }
    int targetdesc = open(target, O_WRONLY | O_CREAT, 0644);
    if (targetdesc == -1)
    {
        perror("open() target error");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd) == -1)
    {
        perror("pipe() error");
        exit(EXIT_FAILURE);
    }
    
    switch (fork())
    {
        case -1: //obsługa błędu funkcji fork()
            perror("Fork error");
            exit(EXIT_FAILURE);
        case 0: //konsument
            if (close(fd[1]) == -1)
            {
                perror("konsument fd[1] close() error");
                exit(EXIT_FAILURE);
            }
            konsument(fd[0], targetdesc);
            if (close(fd[0]) == -1)
            {
                perror("konsument fd[0] close() error");
                exit(EXIT_FAILURE);
            }
            break;
        default: //producent
            if (close(fd[0]) == -1)
            {
                perror("producent fd[0] close() error");
                exit(EXIT_FAILURE);
            }
            producent(fd[1], sourcedesc);
            if (close(fd[1]) == -1)
            {
                perror("producent fd[1] close() error");
                exit(EXIT_FAILURE);
            }
            wait(NULL);  
            break;
    }

    if (close(sourcedesc) == -1)
    {
        perror("close() source error");
        exit(EXIT_FAILURE);
    }
    if (close(targetdesc) == -1)
    {
        perror("close() target error");
        exit(EXIT_FAILURE);
    }
}

void producent(int fd, int sourcedesc)
{
    while ((readbytes = read(sourcedesc, buffer_producent, PRODUCENT_PORCJA)) > 0)
    {
        if (readbytes == -1)
        {
            perror("producent read() error");
            exit(EXIT_FAILURE);
        }
        usleep(my_random(100000, 999999));
        i++;
        writtenbytes = write(fd, buffer_producent, readbytes);
        if (writtenbytes == -1)
        {
            perror("producent write() error");
            exit(EXIT_FAILURE);
        }
        printf("Producent %d: ", i);
        fflush(stdout);
        if (write(STDOUT_FILENO, buffer_producent, readbytes) == -1)
        {
            perror("producent write() error");
            exit(EXIT_FAILURE);
        }
        printf("\n");
    }
}

void konsument(int fd, int targetdesc)
{
    while ((readbytes = read(fd, buffer_konsument, KONSUMENT_PORCJA)) > 0)
    {
        if (readbytes == -1)
        {
            perror("konsument read() error");
            exit(EXIT_FAILURE);
        }
        usleep(my_random(100000, 999999));
        i++;
        writtenbytes = write(targetdesc, buffer_konsument, readbytes);
        if (writtenbytes == -1)
        {
            perror("konsument write() error");
            exit(EXIT_FAILURE);
        }
        printf("Konsument %d: ", i);
        fflush(stdout);
        if (write(STDOUT_FILENO, buffer_konsument, readbytes) == -1)
        {
            perror("konsument write() error");
            exit(EXIT_FAILURE);
        }
        printf("\n");
    }
}

int my_random(int min, int max)
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}