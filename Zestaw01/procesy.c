#include "procinfo.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;                        //zmienna pętli
    int buffer = sizeof(argv[0]); //zmienna przechowująca rozmiar nazwy programu użyta w funkcji strncpy()

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
            sleep(2);
            strncpy(argv[0], "child", buffer); //zmiana nazwy procesu potomnego na "child"
            procinfo(argv[0]);                 //wypisanie informacji o procesie potomnym
        }
    }
    while (wait(NULL) > 0); //proces czeka aż wszystkie jego procesy potomne się zakończą
                            //jeżeli proces nie ma dzieci albo wszystkie dzieci zakończyły działanie to funkcja wait() zwróci wartość -1 i pętla się zakończy
}