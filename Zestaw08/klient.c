#include "msg.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#define OPERATION_SIZE 100

void exit_remove();
void handler(int signum);

int klient_desc;             //deskryptor kolejki klienta
int server_desc;             //deskryptor kolejki serwera
char nazwa_kolejki[20] = ""; //nazwa kolejki klienta

int main()
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

    char dzialanie[OPERATION_SIZE] = ""; //bufor działania
    char *tokeny;
    komunikat wysylane;
    komunikat odebrane;
    sprintf(nazwa_kolejki, "/%d", getpid());                                                   //utworzenie nazwy kolejki klienta
    klient_desc = msgCreate(nazwa_kolejki);                                                    //stworzenie kolejki klienta
    printf("    Nazwa kolejki: %s\n    Deskryptor kolejki: %d\n", nazwa_kolejki, klient_desc); //informacje o kolejce
    msgInfo(klient_desc);
    server_desc = msgOpen(SERWER_KOLEJKA);                                                      //otwarcie kolejki serwera
    printf("    Nazwa kolejki: %s\n    Deskryptor kolejki: %d\n", SERWER_KOLEJKA, server_desc); //informacje o kolejce
    msgInfo(server_desc);

    while (1) //pętla póki nie zostanie wczytany EOF czyli ctrl + D
    {
        printf("Wpisz działanie.\nAby zakończyć wciśnij ctrl + D\n");
        if (fgets(dzialanie, OPERATION_SIZE, stdin) == NULL)
        {
            break;
        }
        tokeny = strtok(dzialanie, " "); //tokenizacja działania
        wysylane.a = atoi(tokeny);
        tokeny = strtok(NULL, " ");
        wysylane.o = (int)(tokeny[0]);
        tokeny = strtok(NULL, " ");
        wysylane.b = atoi(tokeny);
        wysylane.pid = getpid();

        printf("a = %d, o = %d, b = %d\n", wysylane.a, wysylane.o, wysylane.b); //wypisanie tokenów działania
        msgSend(server_desc, &wysylane);
        sleep(8);                             //wysłanie komunikatu
        msgRecv(klient_desc, &odebrane);      //odebranie komunikatu z wynikiem
        printf("  Wynik = %d\n", odebrane.a); //wypisanie wyniku
    }
}

void exit_remove() //funkcja używana w atexit()
{
    msgClose(klient_desc);
    msgClose(server_desc);
    msgRm(nazwa_kolejki);
}

void handler(int signum) //handler sygnału ctrl + C
{
    exit(EXIT_SUCCESS);
}