#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    switch (fork())
    {
    case -1: //obsługa błędu funkcji fork()
        perror("Fork error");
        exit(EXIT_FAILURE);
    case 0: 
        sleep(30);  //używam funkcji sleep() w celu łatwiejszej obserwacji zachowania procesów
        break;
    default:
        sleep(10);
        exit(0);    //proces macierzysty kończy działanie przed zakończeniem działania procesu potomnego
        break;
    }
}