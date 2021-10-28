#ifndef SEM
#define SEM
#include <semaphore.h>

typedef sem_t *semid;

semid semCreate(const char *name, int val);
void semRemove(const char *name);
semid semOpen(const char *name);
void semClose(semid sem);
void semP(semid sem);
void semV(semid sem);
void semInfo(semid sem);

#endif