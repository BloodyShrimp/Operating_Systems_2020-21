#ifndef SHMEM
#define SHMEM
#define CAPACITY 5 //ilośc elementów bufora
#define PORCJA 4   //rozmiar w bajtach kopiowanych na raz informacji (max 4 ze względu na rozmiar int)

int shmCreate(const char *name, int size);
int shmOpen(const char *name);
void shmRm(const char *name);
void *shmMap(int fd, int size);
void shmClose(void *ptr, int fd, int size);
int shmSize(int fd);
void shmInfo(int fd);
int my_random(int min, int max);

typedef struct cykliczny
{
    int size;          // Całkowity rozmiar bufora
    int head;          // Pierwszy element do odczytu
    int tail;          // Element za ostatnim zapisanym elementem
    int buf[CAPACITY]; // Dane, własny typ 'towar' może być równoważny np. 'int'
    int bufor_read[CAPACITY];
} cykliczny;

const char sem_prod[] = "/semafor-producent";
const char sem_kons[] = "/semafor-konsument";
const char mem_name[] = "/shared-memory";
const int mem_size = 1024;

#endif