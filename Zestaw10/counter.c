#define _XOPEN_SOURCE 500
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#define THREAD_AMOUNT 8
#define CRITICAL_AMOUNT 3

volatile bool entering[THREAD_AMOUNT] = {false};
volatile int number[THREAD_AMOUNT] = {0};
volatile int counter = 0;

void lock(int thread);
void unlock(int thread);
int my_random(int min, int max);
void *thread_body(void *ptr);

int main()
{
    pthread_t thread_ID[THREAD_AMOUNT];
    int i;
    int watek_numer[THREAD_AMOUNT];
    for (i = 0; i < THREAD_AMOUNT; i++)
    {
        watek_numer[i] = i;
        if (pthread_create(&thread_ID[i], NULL, thread_body, &watek_numer[i]) != 0)
        {
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
        //printf("-- Utworzono wątek o numerze %d i ID %p\n", i, (void *)&thread_ID[i]);
    }

    for (i = 0; i < THREAD_AMOUNT; i++)
    {
        if (pthread_join(thread_ID[i], NULL) != 0)
        {
            perror("pthread_join error");
            exit(EXIT_FAILURE);
        }
        //printf("-- Zakończono wątek o numerze %d i ID %p\n", i, (void *)&thread_ID[i]);
    }

    printf("Wartość licznika = %d\n", counter);
    printf("Prawidłowa wartość licznika = %d\n", THREAD_AMOUNT * CRITICAL_AMOUNT);
}

void lock(int thread)
{
    int i;
    int max_ticket = 0;
    entering[thread] = true; //wybieranie numeru biletu
    for (i = 0; i < THREAD_AMOUNT; i++)
    {
        max_ticket = (number[i] > max_ticket) ? number[i] : max_ticket;
    }
    number[thread] = max_ticket + 1;
    entering[thread] = false;
    for (i = 0; i < THREAD_AMOUNT; i++) //wejście do sekcji krytycznej
    {
        while (entering[i])
            ;
        while (number[i] != 0 && ((number[i] < number[thread]) || (number[i] == number[thread] && i < thread)))
            ;
    }
}

void unlock(int thread)
{
    number[thread] = 0;
}

int my_random(int min, int max) //funkcja losująca numer z zakresu [min, max]
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void *thread_body(void *ptr)
{
    int thread_number = *(int *)ptr;
    //usleep(my_random(300000, 999999));
    int i;
    for (i = 0; i < CRITICAL_AMOUNT; i++)
    {
        //usleep(my_random(300000, 999999));
        lock(thread_number); //wejście do sekcji krytycznej
        counter = counter + 1;
        //usleep(my_random(300000, 999999));
        unlock(thread_number); //wyjście z sekcji krytycznej
        //usleep(my_random(300000, 999999));
    }

    return NULL;
}