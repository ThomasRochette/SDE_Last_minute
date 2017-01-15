#include "struct.h"
#include "fonction.h"

int main(){
	signal(SIGQUIT,stopAff);
	signal(SIGHUP,stopAff);
	signal(SIGINT, stopAff);//Détournement du signal s'interuption
	int pid=getpid(), i, volsAffiche;

	if ((BDDVols=shmget(cle_BDDVols, 40*sizeof(char), 0666))==-1)exit(1);//Connexion à la mémoire partagée
	pBDD=(MESSAGE*)shmat(BDDVols,0,0);//Attachement à la mémoire partagée
	if((pBDD)==NULL)exit(1);
	printf("Pid : %d Conexions ok",pid);

	while(1){
		volsAffiche=0;//Nombre de vols affichés
		semop(MUTEX,&down,1);
		printf ("\33[H\33[2J");//Permet de ne pas voir les précédents affichage sur le terminal sans pour autant les effacer
		printf("\n\n--------LISTE DES VOLS--------\n\n\n");
		for(i=0;i<TAILLE;i++){
			if(pBDD[i].places==0){//Suppression des vols sans places
				pBDD[i].type=0;
				sprintf(pBDD[i].destination,"%s","NULL");
				pBDD[i].places=-1;//Utiliser pour voir les cases mémoire disponibles
				pBDD[i].pid=0;
				semop(SVols,&up,1);

			}else if(pBDD[i].places!=-1 && pBDD[i].type==2){//Affichage en vert des nouveaux vols
				printf("\tDestination : \033[32m%s\033[0m\n",pBDD[i].destination);
				printf("\tPlaces : \033[32m%d\033[0m\n\n",pBDD[i].places);
				pBDD[i].type=1;//Le vols n'est plus nouveau
				volsAffiche++;
			}else if(pBDD[i].places!=-1){//Affichage classique
				printf("\tDestination : %s\n",pBDD[i].destination);
				printf("\tPlaces : %d\n\n",pBDD[i].places);
				volsAffiche++;
			}

		}

		if(volsAffiche==0){
			printf("\tPAS DE VOLS\n\n");
		}
		semop(MUTEX,&up,1);
		sleep(5);//Attente entre chaque affichage
	}
	return 0;
}

