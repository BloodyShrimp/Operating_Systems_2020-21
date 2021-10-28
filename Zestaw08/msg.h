#ifndef MSG_H
#define MSG_H

#define SERWER_KOLEJKA "/SERWER"

typedef struct komunikat
{
    int pid;
    int a;
    int o;
    int b;
} komunikat;

int msgCreate(const char *name);
int msgOpen(const char *name);
void msgClose(int msgid);
void msgRm(const char *name);
void msgSend(int msgid, komunikat *msg);
void msgRecv(int msgid, komunikat *msg);
void msgInfo(int msgid);

#endif