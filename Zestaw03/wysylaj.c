#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "procinfo.h"

int main(int argc, const char *argv[])
{
    if (argc != 3) //obsluga bledu przy zlej liczbie argumentow
    {
        printf("Nieprawidlowa liczba argumentow.\n");
        exit(EXIT_FAILURE);
    }
    char action = *argv[1];
    if((strlen(argv[1]) != 1) || ((action != 'd') && (action != 'i') && (action != 'p'))) //obsluga bledu przy zlym argumencie akcji
    {
        printf("Argument akcji jest nieprawidlowy.\nObslugiwane argumenty to \"d\", \"i\" lub \"p\"\n");
        exit(EXIT_FAILURE);
    }

    int child_PID = 0;
    int signal_number = atoi(argv[2]);
    switch (child_PID = fork())
    {
        case -1: //obsługa błędu funkcji fork()
            perror("Fork error");
            exit(EXIT_FAILURE);
        case 0: //proces potomny
            execl("./obsluga.x", "obsluga.x", argv[1], argv[2], NULL);
            break;
        default: //proces macierzysty
            sleep(2);
            if (kill(child_PID, 0) != 0) //sprawdzenie czy proces istnieje
            {
                printf("Proces potomny nie istnieje");
                exit(EXIT_FAILURE);
            }
            kill(child_PID, signal_number);
            // int i = 0;
            // for (i = 0; i < 5; i++)
            // {
            //     kill(child_PID, signal_number);
            //     sleep(1);
            // }
            break;
    }

    wait(NULL);
}