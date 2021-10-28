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
        sleep(10);  //używam funkcji sleep() w celu łatwiejszej obserwacji zachowania procesów
        exit(0);    //proces potomny kończy działanie ale proces macierzysty jest w tym czasie uśpiony
        break;
    default:
        sleep(30);
        break;
    }
}