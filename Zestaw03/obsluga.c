#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "procinfo.h"

void handler(int signum)
{
    //signal(signum, handler);
    printf("Sygnal przechwycony. Numer sygnalu: %d\n", signum);
}

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

    procinfo(argv[0]); //wypisanie informacji o procesie
    int signal_number = atoi(argv[2]); //zamiana stringa z numerem sygnalu na int

    sleep(1);
    switch (action) //switch case dla opcji obslugi sygnalu
    {
        case 'd':
            if (signal(signal_number, SIG_DFL) == SIG_ERR)
            {
                perror("signal() error");
                exit(EXIT_FAILURE);
            }
            printf("Zlecono wykonanie domyslnej operacji sygnalu.\n");
            break;
        case 'i':
            if (signal(signal_number, SIG_IGN) == SIG_ERR)
            {
                perror("signal() error");
                exit(EXIT_FAILURE);
            }
            printf("Zlecono zignorowanie sygnalu.\n");
            break;
        case 'p':
            if (signal(signal_number, handler) == SIG_ERR)
            {
                perror("signal() error");
                exit(EXIT_FAILURE);
            }
            printf("Zlecono przechwycenie sygnalu.\n");
            break;
    }

    pause();
    //while (pause() == -1);
}