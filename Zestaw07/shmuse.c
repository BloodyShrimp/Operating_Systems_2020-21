#include "shmem.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    if(argc == 1)
    {
        printf("Jak należy używać programu: %s <mode>\nTryby użycia programu (mode):\nc - tworzy segment pamięci dzielonej, należy podać drugi argument określający rozmiar pamięci\nd - usuwa segment pamięci dzielonej\nr - wypisuje zawartość pamięci dzielonej\nw - kopiuje zawartość standardowego wejścia do pamięci dzielonej\ni - wypisuje informacje o pamięci dzielonej\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    char action = *argv[1];
    int size;
    if ((strlen(argv[1]) != 1) || ((action != 'c') && (action != 'd') && (action != 'r') && (action != 'w') && (action != 'i'))) //obsluga bledu przy zlym argumencie akcji
    {
        printf("Argument akcji jest nieprawidlowy.\nObslugiwane argumenty to: c d r w i\n");
        exit(EXIT_FAILURE);
    }

    if(action == 'c')
    {
        if(argc != 3)
        {
            printf("Nieprawidłowa liczba argumentów.\n");
            exit(EXIT_FAILURE);
        }
        size = atoi(argv[2]);
    }
    else
    {
        if(argc != 2)
        {
            printf("Nieprawidłowa liczba argumentów.\n");
            exit(EXIT_FAILURE);            
        }
    }

    const char memory_name[] = "/shmuse-pamiec";
    const int memory_size = size;
    int descriptor;
    char *mem_ptr;
    char buffer[memory_size];

    switch (action)
    {
    case 'c':
        descriptor = shmCreate(memory_name, size);
        close(descriptor);
        break;
    case 'd':
        shmRm(memory_name);
        break;
    case 'r':
        descriptor = shmOpen(memory_name);
        mem_ptr = shmMap(descriptor, memory_size);
        printf("Shared data:\n%s", mem_ptr);
        shmClose(mem_ptr, descriptor, memory_size);
        break;
    case 'w':
        descriptor = shmOpen(memory_name);
        mem_ptr = shmMap(descriptor, memory_size);
        read(STDIN_FILENO, buffer, memory_size);
        strcpy(mem_ptr, buffer);
        shmClose(mem_ptr, descriptor, memory_size);
    case 'i':
        descriptor = shmOpen(memory_name);
        shmInfo(descriptor);
        close(descriptor);
        break;
    }
}