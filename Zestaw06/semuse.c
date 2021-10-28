#include "sem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]) // 1 - operacja
{
    if(argc != 2)
    {
        printf("Zła liczba argumentów.\n");
        exit(EXIT_FAILURE);
    }
    
    char action = *argv[1];
    if((strlen(argv[1]) != 1) || ((action != 'i') && (action != 'r') && (action != 'p') && (action != 'v') && (action != 'q'))) //obsluga bledu przy zlym argumencie akcji
    {
        printf("Argument akcji jest nieprawidlowy.\nObslugiwane argumenty to: i r p v q\n");
        exit(EXIT_FAILURE);
    }

    semid semaphore_id;
    char sem_name[] = "/semafor";

    switch(action)
    {
        case 'i': //initialize
            semaphore_id = semCreate(sem_name, 1);
            semInfo(semaphore_id);
            semClose(semaphore_id);
            break;
        case 'r': //remove
            semRemove(sem_name);
            break;
        case 'p': //opuść
            semaphore_id = semOpen(sem_name);
            semP(semaphore_id);
            semInfo(semaphore_id);
            semClose(semaphore_id);
            break;
        case 'v': //podnieś
            semaphore_id = semOpen(sem_name);
            semV(semaphore_id);
            semInfo(semaphore_id);
            semClose(semaphore_id);
            break;
        case 'q': //query
            semaphore_id = semOpen(sem_name);
            semInfo(semaphore_id);
            semClose(semaphore_id);
            break;
    }
}