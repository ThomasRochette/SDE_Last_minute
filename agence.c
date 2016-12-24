#include "struct.h"

void stopAgn(){//Fonction d'arret
	shmdt(pBDD);// Détachement de la mémoire partagée
	msgctl(BAL, IPC_RMID, NULL);// Destruction de la boite au lettre
	exit(0);// Fermeture du processus
}

int main(){
	signal(SIGINT, stopAgn);//Détournement du signal d'interuption
	MESSAGE requete;
	int pid=getpid(), i, ok;

	if ((BAL=msgget(cle_BAL, 0666|IPC_CREAT))==-1)exit(1);//Création de la boite aux lettres
	printf("pid %d création BAL ok\n",pid);
	if ((BDDVols=shmget(cle_BDDVols, 40*sizeof(char), 0666))==-1)exit(1);//Connexion à la mémoire partagée
	pBDD=(MESSAGE*)shmat(BDDVols,0,0);//Attachement à la mémoire partagée
	if((pBDD)==NULL)exit(1);

	while(1){
		msgrcv(BAL, &requete, sizeof(requete.destination)+sizeof(requete.places)+sizeof(requete.pid), 1, 0);//Lecture de la boite aux lettres
		printf("pid %d message reçu :\n",pid);
		printf("\ttype %ld\n",requete.type);
		printf("\tdestination %s\n",requete.destination);
		printf("\tplaces %d\n",requete.places);
		printf("\tpar pid %d\n\n",requete.pid);
		ok=0;

		semop(MUTEX,&down,1);
		printf("mutex down");
		for(i=0;i<TAILLE;i++){	
			if((strcmp(pBDD[i].destination,requete.destination)==0) && (requete.places<=pBDD[i].places)){//Comparaison avec la BDD
				pBDD[i].places=pBDD[i].places-requete.places;//Changement du nombre de places disponible si nécessaire
				ok=1;			
			}else{

			}
		}
		semop(MUTEX,&up,1);
		printf("mutex up");
		if(ok==1){
			kill(requete.pid, SIGUSR1);//Réponse si réservation ok
		}else{
			kill(requete.pid, SIGUSR2);//Réponse si réservation pas ok
		}


	}
	return 0;
}

