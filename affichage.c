#include "struct.h"

void stopAff(){
	shmdt(pBDD);
	exit(0);
}

int main(){
	signal(SIGINT, stopAff);//Détournement du signal s'interuption
	int pid=getpid(), i, volsAffiche;

	if ((BDDVols=shmget(cle_BDDVols, 40*sizeof(char), 0666))==-1)exit(1);//Connexion à la mémoire partagée
	pBDD=(MESSAGE*)shmat(BDDVols,0,0);//Attachement à la mémoire partagée
	if((pBDD)==NULL)exit(1);
	printf("Pid : %d Conexions ok",pid);

	while(1){
		volsAffiche=0;
		semop(MUTEX,&down,1);
		printf("\n\n--------LISTE DES VOLS--------\n\n\n");
		for(i=0;i<TAILLE;i++){
			if(pBDD[i].places==0){
				pBDD[i].type=0;
				sprintf(pBDD[i].destination,"%s","NULL");
				pBDD[i].places=-1;
				pBDD[i].pid=0;
				semop(SVols,&up,1);

			}else if(pBDD[i].places!=-1){
				printf("\tDestination : %s\n",pBDD[i].destination);
				printf("\tPlaces : %d\n\n",pBDD[i].places);
				volsAffiche++;
			}
		}
		if(volsAffiche==0){
			printf("\tPAS DE VOLS\n\n");
		}
		semop(MUTEX,&up,1);
		sleep(5);
	}
	return 0;
}
	
