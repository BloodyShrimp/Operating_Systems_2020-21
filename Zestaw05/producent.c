#define _XOPEN_SOURCE 500
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>

#define PRODUCENT_PORCJA 8

char buffer_producent[PRODUCENT_PORCJA];
int readbytes = 0;
int writtenbytes = 0;
int i = 0;

int main(int argc, const char *argv[]) // 1 - source, 2 - pipe
{
    int pipe_desc = open(argv[2], O_WRONLY);
    if (pipe_desc == -1)
    {
        perror("open() pipe error");
        exit(EXIT_FAILURE);
    }
    
    int source_desc = open(argv[1], O_RDONLY);
    if (source_desc == -1)
    {
        perror("open() source error");
        exit(EXIT_FAILURE);
    }

    while ((readbytes = read(source_desc, buffer_producent, PRODUCENT_PORCJA)) > 0)
    {
        if (readbytes == -1)
        {
            perror("producent read() error");
            exit(EXIT_FAILURE);
        }
        i++;
        writtenbytes = write(pipe_desc, buffer_producent, readbytes);
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

    if (close(pipe_desc) == -1)
    {
        perror("close() pipe error");
        exit(EXIT_FAILURE);
    }

    if (close(source_desc) == -1)
    {
        perror("close() source error");
        exit(EXIT_FAILURE);
    }

    _exit(EXIT_SUCCESS);
}