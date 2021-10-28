#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include "msg.h"

int msgCreate(const char *name)
{
    printf("Tworzenie kolejki komunikatów %s\n", name);
    int msgDesc;
    struct mq_attr atrybuty_kolejki;
    atrybuty_kolejki.mq_maxmsg = 5; //max liczba komunikatów w kolejce
    atrybuty_kolejki.mq_msgsize = sizeof(komunikat); //max rozmiar komunikatu
    msgDesc = mq_open(name, O_RDWR | O_CREAT | O_EXCL, 0644, &atrybuty_kolejki);
    if (msgDesc == -1)
    {
        perror("msgCreate() error");
        exit(EXIT_FAILURE);
    }
    return msgDesc;
}

int msgOpen(const char *name)
{
    printf("Otwieranie kolejki komunikatów %s\n", name);
    int msgDesc;
    msgDesc = mq_open(name, O_RDWR);
    if (msgDesc == -1)
    {
        perror("msgOpen() error");
        exit(EXIT_FAILURE);
    }
    return msgDesc;
}

void msgClose(int msgid)
{
    printf("Zamykanie kolejki komunikatów (deskryptor: %d)\n", msgid);
    if (mq_close(msgid) == -1)
    {
        perror("msgClose() error");
        exit(EXIT_FAILURE);
    }
}

void msgRm(const char *name)
{
    printf("Usuwanie kolejki komunikatów %s\n", name);
    if (mq_unlink(name) == -1)
    {
        perror("msgRm() error");
        exit(EXIT_FAILURE);
    }
}

void msgSend(int msgid, komunikat *msg)
{
    printf("Wysyłanie komunikatu (deskryptor: %d)\n", msgid);
    if (mq_send(msgid, (const char *)msg, sizeof(komunikat), 1) == -1)
    {
        perror("msgSend() error");
        exit(EXIT_FAILURE);
    }
}

void msgRecv(int msgid, komunikat *msg)
{
    printf("Odbieranie komunikatu (deskryptor: %d)\n", msgid);
    if (mq_receive(msgid, (char *)msg, sizeof(komunikat), NULL) == -1)
    {
        perror("msgRecv() error");
        exit(EXIT_FAILURE);
    }
}

void msgInfo(int msgid)
{
    struct mq_attr atrybuty;
    mq_getattr(msgid, &atrybuty);
    printf("    Opcje kolejki: %d\n    Max liczba komunikatów w kolejce: %d\n    Max rozmiar komunikatu: %d\n    Liczba komunikatów w kolejce: %d\n", atrybuty.mq_flags, atrybuty.mq_maxmsg, atrybuty.mq_msgsize, atrybuty.mq_curmsgs);
}