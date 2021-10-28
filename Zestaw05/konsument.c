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

#define KONSUMENT_PORCJA 8

char buffer_konsument[KONSUMENT_PORCJA];
int readbytes = 0;
int writtenbytes = 0;
int i = 0;

int main(int argc, const char *argv[]) // 1 - target, 2 - pipe
{
    int pipe_desc = open(argv[2], O_RDONLY);
    if (pipe_desc == -1)
    {
        perror("open() pipe error");
        exit(EXIT_FAILURE);
    }

    int target_desc = open(argv[1], O_WRONLY | O_CREAT, 0644);
    if (target_desc == -1)
    {
        perror("open() target error");
        exit(EXIT_FAILURE);
    }

    while ((readbytes = read(pipe_desc, buffer_konsument, KONSUMENT_PORCJA)) > 0)
    {
        if (readbytes == -1)
        {
            perror("konsument read() error");
            exit(EXIT_FAILURE);
        }
        i++;
        writtenbytes = write(target_desc, buffer_konsument, readbytes);
        if (writtenbytes == -1)
        {
            perror("konsument write() error");
            exit(EXIT_FAILURE);
        }
        usleep(100); //dla ładnego formatowania tekstu
        printf("Konsument %d: ", i);
        fflush(stdout);
        if (write(STDOUT_FILENO, buffer_konsument, readbytes) == -1)
        {
            perror("konsument write() error");
            exit(EXIT_FAILURE);
        }
        printf("\n");
    }

    if (close(pipe_desc) == -1)
    {
        perror("close() pipe error");
        exit(EXIT_FAILURE);
    }

    if (close(target_desc) == -1)
    {
        perror("close() target error");
        exit(EXIT_FAILURE);
    }

    _exit(EXIT_SUCCESS);
}