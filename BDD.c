#include "struct.h"

void stopBDD(){
	shmdt(pBDD);
	shmctl(BDDVols, IPC_RMID, NULL);
	semctl(MUTEX, 1, IPC_RMID,0);
	semctl(SVols, 1, IPC_RMID,0);
	exit(0);
}

int main(){
	int i;
	signal(SIGINT, stopBDD);

	if ((BDDVols=shmget(cle_BDDVols, TAILLE*sizeof(MESSAGE), 0777|IPC_CREAT))==-1)exit(1);
	if ((MUTEX=semget(cle_Mutex, 1, 0666|IPC_CREAT))==-1)exit(1);
	if ((SVols=semget(cle_SVols, 1, 0666|IPC_CREAT))==-1)exit(1);
	
	pBDD=(MESSAGE*)shmat(BDDVols,0,0);
	if ((semctl(MUTEX, 0, SETVAL, 1))==-1)exit(1);
	if ((semctl(SVols, 0, SETVAL, 20))==-1)exit(1);

	semop(MUTEX,&down,1);
	for(i=0;i<TAILLE;i++){
		pBDD[i].type=0;
		sprintf(pBDD[i].destination,"%s","NULL");
		pBDD[i].places=-1;
		pBDD[i].pid=0;
	}
	semop(SVols,&down,1);
	pBDD[12].type=0;
	sprintf(pBDD[12].destination,"%s","CDG");
	pBDD[12].places=100;
	pBDD[12].pid=0;
	semop(MUTEX,&up,1);	

	printf("pid : %d, Création et attachement ok\n", getpid());
	
	while(1){
		sleep(1);
	}
	
	return 0;
}
