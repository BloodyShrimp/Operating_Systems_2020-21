#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "procinfo.h"

int procinfo(const char *name)
{
	int UID = getuid();
	int GID = getgid();
	int PID = getpid();
	int PPID = getppid();
	int PGID = getpgrp();

	printf("Name = %s | UID = %d | GID = %d | PID = %d | PPID = %d | PGID = %d\n", name, UID, GID, PID, PPID, PGID); //wypisanie parametrów procesu

	return 0;
}
