#ifndef _STRUCT_LAST_MINUTES_
#define _STRUCT_LAST_MINUTES_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

typedef struct message {
	long type;
	char destination[4];
	int places;
	pid_t pid;
}MESSAGE;

key_t cle_BAL=0x100;
key_t cle_BDDVols=0x200;
key_t cle_SVols=0x300;
key_t cle_Mutex=0x400;
int BAL;
int BDDVols;
int MUTEX;
int SVols;
int TAILLE=20;
MESSAGE *pBDD=NULL;

struct sembuf up={0,1,0}, down={0,-1,0};
/*void down(int *idsem){
	struct sembuf operation;
	operation.sem_num=0;
	operation.sem_op=-1;
	operation.sem_flg=0;
	if(semop(*idsem,&operation,1)==-1)exit(-1);
}

void up(int *idsem){
	struct sembuf operation;
	operation.sem_num=0;
	operation.sem_op=1;
	operation.sem_flg=0;
	if(semop(*idsem,&operation,1)==-1)exit(-1);
}*/


#endif
