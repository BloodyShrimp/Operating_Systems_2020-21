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
    int i = 0;
    int n = 3; //zmiena ilości dzieci lidera grupy
    int child_status = 0;
    int signal_number = atoi(argv[2]);
    switch (child_PID = fork())
    {
        case -1: //obsługa błędu funkcji fork()
            perror("Fork error");
            exit(EXIT_FAILURE);
        case 0: //proces potomny
            setpgid(0, 0);
            for (i = 0; i < n; i++) //utworzenie procesów potomnych i uruchomienie w nich programu obsluga.x
            {
                if (fork() == 0)
                {
                    execl("./obsluga.x", "obsluga.x", argv[1], argv[2], NULL);
                }
            }
            if (signal(signal_number, SIG_IGN) == SIG_ERR) //lider grupy ignoruje sygnały
            {
                perror("signal() error");
                exit(EXIT_FAILURE);
            }
            for (i = 0; i < n; i++) //wypisanie statusu dzieci
            {
                wait(&child_status);
                printf("Status dziecka %d: %d\n", i+1, child_status);
            }
            break;
        default: //proces macierzysty
            sleep(2);
            if (kill(-child_PID, 0) != 0) //sprawdzenie czy grupa procesów istnieje
            {
                printf("Grupa procesow potomnych nie istnieje");
                exit(EXIT_FAILURE);
            }
            kill(-child_PID, signal_number); //wysłanie sygnału do grupy procesów
            break;
    }

    wait(NULL);
}