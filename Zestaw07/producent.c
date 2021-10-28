#define _XOPEN_SOURCE 500
#include "sem.h"
#include "shmem.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int buffer_producent = 0;
int source_desc = 0;
int readbytes = 0;
semid sem_prod_id = 0;
semid sem_kons_id = 0;
cykliczny *mem_ptr;
char read_text[PORCJA];

void push(int read_push);

int main(int argc, const char *argv[]) // 1 - source
{
    sem_prod_id = semOpen(sem_prod);                   //otwarcie semafora producenta
    sem_kons_id = semOpen(sem_kons);                   //otwarcie semafora konsumenta
    int mem_desc = shmOpen(mem_name);                  //otwiarcie pamięci dzielonej
    mem_ptr = (cykliczny *)shmMap(mem_desc, mem_size); //mapowane pamięci w przestrzeni adresowej procesu
    source_desc = open(argv[1], O_RDONLY);             //otwarcie pliku źródłowego
    if (source_desc == -1)                             //obsługa błędu
    {
        perror("open() source error");
        exit(EXIT_FAILURE);
    }

    while ((readbytes = read(source_desc, &buffer_producent, PORCJA)) > 0) //odczyt z pliku póki odczytano więcej niż 0 bajtów
    {
        if (readbytes == -1)
        {
            perror("producent read() error");
            exit(EXIT_FAILURE);
        }
        push(buffer_producent); //zapisanie odczytanych bajtów do bufora
    }
    push(-1); //jeżeli konsument odczyta -1 z bufora to będzie to informacja, że powinien zakończyć odczyt

    semClose(sem_prod_id);
    semClose(sem_kons_id);
    shmClose(mem_ptr, mem_desc, mem_size);

    exit(EXIT_SUCCESS);
}

void push(int read_push)
{
    usleep(my_random(100, 500000));
    semP(sem_prod_id);                              //opuszczenie semafora producenta
    mem_ptr->buf[mem_ptr->tail] = read_push;        //ustawienie elementu bufora na podane bajty
    mem_ptr->bufor_read[mem_ptr->tail] = readbytes; //informacja o ilości przeczytanych bajtów trzymanych w odpowiadającym elemencie bufora
    printf("    SEKCJA KRYTYCZNA PRODUCENT\n    Wartość semafora producenta: ");
    semInfo(sem_prod_id);
    printf("    Wartość semafora konsumenta: ");
    semInfo(sem_kons_id);
    printf("    Bierzący indeks elementu bufora: %d\n", mem_ptr->tail);
    printf("    Liczba wstawianych bajtów: %d\n", readbytes);
    printf("    Wstawiany tekst: ");
    fflush(stdout);
    write(STDOUT_FILENO, &buffer_producent, readbytes);
    mem_ptr->tail++;                 //inkrementacja licznika kolejnego miejsca do zapisu
    if ((mem_ptr->tail) == CAPACITY) //cykliczność
    {
        mem_ptr->tail = 0;
    }
    printf("\n    SEKCJA KRYTYCZNA PRODUCENT KONIEC    \n");
    usleep(my_random(100, 500000));
    semV(sem_kons_id); //podniesienie semafora konsumenta
}