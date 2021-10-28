#include <stdio.h>
#include <unistd.h>
#include "procinfo.h"

int main(int argc, char *argv[])
{
    printf("pierwszy komunikat\n");
    execl("./potomny.x", "potomny.x", NULL); //wywołanie funkcji execl()
    perror("execl() error");             //obsługa błędu funkcji execl()
    printf("drugi komunikat\n");
}