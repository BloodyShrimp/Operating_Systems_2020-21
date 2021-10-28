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

int buffer_konsument = 0;
int target_desc = 0;
int readbytes = 0;
semid sem_prod_id = 0;
semid sem_kons_id = 0;
cykliczny *mem_ptr;
char read_text[PORCJA];

int pop();

int main(int argc, const char *argv[]) // 1 - source
{
    sem_prod_id = semOpen(sem_prod);                                 //otwarcie semafora producenta
    sem_kons_id = semOpen(sem_kons);                                 //otwarcie semafora konsumenta
    int mem_desc = shmOpen(mem_name);                                //otwarcie pamięci dzielonej
    mem_ptr = (cykliczny *)shmMap(mem_desc, mem_size);               //mapowane pamięci w przestrzeni adresowej procesu
    target_desc = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644); //otwarcie pliku docelowego
    if (target_desc == -1)                                           //obsługa błędu
    {
        perror("open() target error");
        exit(EXIT_FAILURE);
    }

    while ((buffer_konsument = pop()) != -1) //póki nie zostanie odczytane -1 z bufora co sygnalizuje koniec odczytu
    {
        write(target_desc, &buffer_konsument, readbytes);
    }

    semClose(sem_prod_id);
    semClose(sem_kons_id);
    shmClose(mem_ptr, mem_desc, mem_size);

    exit(EXIT_SUCCESS);
}

int pop()
{
    usleep(my_random(500000, 999999));
    semP(sem_kons_id);                              //opuszczenie semafora konsumenta
    int read_pop = mem_ptr->buf[mem_ptr->head];     //odczytanie zawartości elementu bufora
    readbytes = mem_ptr->bufor_read[mem_ptr->head]; //odczytanie ilości przeczytanych bajtów przechowywanych w odpowiadającym buforze
    printf("            SEKCJA KRYTYCZNA KONSUMENT\n            Wartość semafora producenta: ");
    semInfo(sem_prod_id);
    printf("            Wartość semafora konsumenta: ");
    semInfo(sem_kons_id);
    printf("            Bierzący indeks elementu bufora: %d\n", mem_ptr->head);
    printf("            Liczba pobieranych bajtów: %d\n", readbytes);
    printf("            Pobierany tekst: ");
    fflush(stdout);
    write(STDOUT_FILENO, &read_pop, readbytes);
    mem_ptr->head++;                 //inkrementacja licznika kolejnego miejsca do odczytu
    if ((mem_ptr->head) == CAPACITY) //cykliczność
    {
        mem_ptr->head = 0;
    }
    printf("\n            SEKCJA KRYTYCZNA KONSUMENT KONIEC\n");
    usleep(my_random(500000, 999999));
    semV(sem_prod_id); //podniesienie semafora producenta
    return read_pop;   //zwracam przeczytane bajty przechowywane w buforze
}