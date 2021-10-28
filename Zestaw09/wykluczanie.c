#define _XOPEN_SOURCE 500
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile int counter = 0; //licznik
int critical_amount;
pthread_mutex_t mutex; //mutex

void *thread(void *ptr);
int my_random(int min, int max);

int main(int argc, const char *argv[]) // 1 - liczba wątków, 2 - liczba wejść do sekcji krytycznej
{
    if (argc != 3) //obsługa błędu przy złej liczbie argumentów
    {
        printf("Nieprawidłowa liczba argumentów.\n");
        exit(EXIT_FAILURE);
    }
    printf("\033c\n");
    printf("\033[1;1HWątek\033[1;7HNr\033[1;10HSekcja prywatna\033[1;30HSekcja krytyczna\n");
    int thread_amount = atoi(argv[1]); //zmienna ilości wątków
    critical_amount = atoi(argv[2]);   //zmienna ilość wejść do sekcji krytycznej
    int row_number = thread_amount + 4;
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("pthread_mutex_init error");
        exit(EXIT_FAILURE);
    }
    printf("\033[%d;1H\033[0m** Zainicjowano mutex pod adresem %p\n", row_number, (void *)&mutex);
    row_number++;

    pthread_t thread_ID[thread_amount];

    int i;
    int watek_numer[thread_amount];
    for (i = 0; i < thread_amount; i++)
    {
        watek_numer[i] = i;
        if (pthread_create(&thread_ID[i], NULL, thread, &watek_numer[i]) != 0)
        {
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
        printf("\033[%d;1H\033[0m-- Utworzono wątek o numerze %d i ID %p\n", row_number, i, (void *)&thread_ID[i]);
        row_number++;
    }

    row_number = row_number - i;
    for (i = 0; i < thread_amount; i++)
    {
        if (pthread_join(thread_ID[i], NULL) != 0)
        {
            perror("pthread_join error");
            exit(EXIT_FAILURE);
        }
        printf("\033[%d;1H\033[2K\033[0m-- Zakończono wątek o numerze %d i ID %p\n", row_number, i, (void *)&thread_ID[i]);
        row_number++;
    }
    if (pthread_mutex_destroy(&mutex) != 0)
    {
        perror("pthread_mutex_destroy error");
        exit(EXIT_FAILURE);
    }
    printf("\033[%d;1H\033[0m** Usunięto mutex\n", row_number);
    row_number++;
    printf("\033[0m\033[%d;1HWartość licznika = \033[92m%d\n", row_number, counter);
    row_number++;
    printf("\033[0m\033[%d;1HPrawidłowa wartość licznika = \033[96m%d\n", row_number, thread_amount * critical_amount);
}

void *thread(void *ptr)
{
    //int *pointer = ptr;
    int thread_number = *(int *)ptr;
    int row = thread_number + 3;
    printf("\033[0m\033[%d;1H\033[2KWątek\033[%d;7H%d\033[%d;10H\033[93mutworzono\033[%d;30H\033[0m\n", row, row, thread_number, row, row);
    usleep(my_random(300000, 999999));
    int thread_counter;
    int i;
    for (i = 0; i < critical_amount; i++)
    {
        printf("\033[0m\033[%d;1H\033[2KWątek\033[%d;7H%d\033[%d;10H\033[32mczekam\033[%d;30H\033[0m\n", row, row, thread_number, row, row);
        usleep(my_random(300000, 999999));
        if (pthread_mutex_lock(&mutex) != 0) //wejście do sekcji krytycznej
        {
            perror("pthread_mutex_lock error");
            exit(EXIT_FAILURE);
        }
        thread_counter = counter;
        printf("\033[0m\033[%d;1H\033[2KWątek\033[%d;7H%d\033[%d;10H\033[%d;30H\033[31msekcja krytyczna\033[0m (%d/%d)\033[0m\n", row, row, thread_number, row, row, i, thread_counter);
        usleep(my_random(300000, 999999));
        thread_counter++;
        counter = thread_counter;
        if (pthread_mutex_unlock(&mutex) != 0) //wyjście z sekcji krytycznej
        {
            perror("pthread_mutex_unlock error");
            exit(EXIT_FAILURE);
        }
        printf("\033[0m\033[%d;1H\033[2KWątek\033[%d;7H%d\033[%d;10H\033[95mśpię\033[%d;30H\033[0m\n", row, row, thread_number, row, row);
        usleep(my_random(300000, 999999));
    }
    printf("\033[0m\033[%d;1H\033[2KWątek\033[%d;7H%d\033[%d;10H\033[93mzakończono\033[%d;30H\033[0m\n", row, row, thread_number, row, row);

    return NULL;
}

int my_random(int min, int max) //funkcja losująca numer z zakresu [min, max]
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}