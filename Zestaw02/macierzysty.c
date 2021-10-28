#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "procinfo.h"

int main(int argc, char *argv[])
{
    int i;                        //zmienna pętli
    int buffer = sizeof(argv[0]); //zmienna przechowująca rozmiar nazwy programu użyta w funkcji strncpy()
    int children = 0;   //licznik dzieci
    char* arguments[] = {argv[0], NULL};    //argumenty funkcji execv()

    strncpy(argv[0], "parent", buffer); //zmianna nazwy programu na "parent"
    procinfo(argv[0]);                  //wypisanie informacji o procesie macierzystym
    for (i = 0; i < 3; i++)             //pętla wywołująca funkcję fork() trzy razy
    {
        switch (fork())
        {
        case -1: //obsługa błędu funkcji fork()
            perror("Fork error");
            exit(EXIT_FAILURE);
        case 0: //proces potomny przy użyciu funkcji sleep() czeka aż proces macierzysty wykona wszystke swoje polecenia
            sleep(1);
            children = 0;   //licznik dzieci ustawiany na 0 przy powstaniu nowego procesu
            strncpy(argv[0], "child", buffer);   //zmiana nazwy procesu potomnego na "child"
            execv("./potomny.x", arguments); //wypisanie informacji o procesie potomnym
            perror("execv() error");    //obsługa błędu funkcji execv()
            break;
        default:
            children++; //inkrementacja licznika dzieci
            break;
        }
    }

    for (i = 0; i < children; i++)  //wywoałnie funkcji wait() tyle razy ile dzieci ma proces
    {
        wait(NULL);
    }
}