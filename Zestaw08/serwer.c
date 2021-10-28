#include "msg.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void exit_remove();
void handler(int signum);

int server_desc; //deskryptor kolejki serwera

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

    server_desc = msgCreate(SERWER_KOLEJKA);                                                    //utworzenie kolejki serwera
    printf("    Nazwa kolejki: %s\n    Deskryptor kolejki: %d\n", SERWER_KOLEJKA, server_desc); //informacje o kolejce
    msgInfo(server_desc);
    komunikat odebrane;        //struktura odebranych danych
    komunikat wysylane;        //struktura wysylanych danych
    int target_PID;            //PID procesu wysyłającego komunikat
    char target_name[20] = ""; //nazwa kolejki procesu wysyłającego komunikat
    int target_desc;           //deskryptor kolejki procesu wysyłającego komunikat
    int wynik;                 //zmiena wyniku

    while (1)
    {
        msgRecv(server_desc, &odebrane);         //odbieranie komunikatu
        target_PID = odebrane.pid;               //zapisanie PID procesu wysyłającego komunikat
        sprintf(target_name, "/%d", target_PID); //utworzenie nazwy kolejki procesu który wysłał komunikat
        target_desc = msgOpen(target_name);      //otwarcie kolejki procesu wysyłającego komunikat
        switch (odebrane.o)                      //obliczenie wyniku na podstawie otrzymanego operatora w postaci int
        {
        case 43: //43 - '+'
            wynik = odebrane.a + odebrane.b;
            break;
        case 45: //45 - '-'
            wynik = odebrane.a - odebrane.b;
            break;
        case 42: //42 - '*'
            wynik = odebrane.a * odebrane.b;
            break;
        case 47: //47 - '/'
            wynik = odebrane.a / odebrane.b;
            break;
        default:
            printf("  Nieobsługiwany operator.\n");
            wynik = -1;
            break;
        }
        wysylane.a = wynik;              //zapisanie wyniku do struktury
        printf("  Wynik = %d\n", wynik); //wypisanie wyniku
        sleep(4);
        msgSend(target_desc, &wysylane); //wysłanie wyniku
        msgClose(target_desc);           //zamknięcie deskryptora
    }
}

void exit_remove() //funkcja używana w atexit()
{
    msgClose(server_desc);
    msgRm(SERWER_KOLEJKA);
}

void handler(int signum) //handler sygnału ctrl + C
{
    exit(EXIT_SUCCESS);
}