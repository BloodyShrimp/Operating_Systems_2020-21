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
    int children = 0;             //licznik dzieci

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
            children = 0;                      //licznik dzieci ustawiany na 0 przy powstaniu nowego procesu
            strncpy(argv[0], "child", buffer); //zmiana nazwy procesu potomnego na "child"
            procinfo(argv[0]);                 //wypisanie informacji o procesie potomnym
            goto exit_loop;                    //proces potomny wcyhodzi z pętli
            break;
        default:
            children++; //inkrementacja licznika dzieci
            break;
        }
    }
    exit_loop:;

    for (i = 0; i < children; i++) //wywołanie funkcji wait() tyle razy ile dzieci ma proces
    {
        wait(NULL);
    }
}