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

void exit_unlink();

const char *pipe_name;

int main(int argc, const char *argv[]) // 1 - source, 2 - target, 3 - pipe
{
    if(argc != 4)
    {
        printf("Zla liczba argumentow.\n");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(argv[3], 0644) == -1)
    {
        perror("mkfifo() error");
        exit(EXIT_FAILURE);
    }
    pipe_name = argv[3];
    if (atexit(exit_unlink) < 0)
    {
        printf("atexit() error");
        exit(EXIT_FAILURE);
    }

    switch(fork()) //producent
    {
        case -1:
            perror("fork() error");
            exit(EXIT_FAILURE);
        case 0:
            if (execl("./producent.x", "producent.x", argv[1], argv[3]) == -1)
            {
                perror("producent execl() error");
                exit(EXIT_FAILURE);
            }
        default:
            break;
    }

    switch(fork()) //konsument
    {
        case -1:
            perror("fork() error");
            exit(EXIT_FAILURE);
        case 0:
            if (execl("./konsument.x", "konsument.x", argv[2], argv[3]) == -1)
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

void exit_unlink()
{
    if (unlink(pipe_name) == -1)
    {
        perror("unlink() error");
    }
}