#include "struct.h"

void YES(){
	printf("Demande accordée\n");
}

void NO(){
	printf("Demande refusée\n");
}

void stopUsr(){
	printf("Fin d'utilisateur\n");
	exit(0);
}

int main(){
	signal(SIGINT, stopUsr);

	MESSAGE requete;
	int pid=getpid(), envoyer;
	char temp[4];
	requete.type=1;
	signal(SIGUSR1, YES);
	signal(SIGUSR2, NO);
	if ((BAL=msgget(cle_BAL, 0666))==-1) exit(1);//Connexion à la boite aux lettres
	while(1){
		envoyer=0;	
		printf("PID %d Destination en 3 lettre : ",pid);
		scanf("%s", &(temp[0]));
		sprintf(requete.destination,"%s",temp);
		printf("PID %d Places : ",pid);
		scanf("%d", &requete.places);
		requete.pid=pid;
		while(envoyer==0){
			if(msgsnd(BAL,&requete,sizeof(requete.destination)+sizeof(requete.places)+sizeof(requete.pid),0)==0)envoyer=1;//Envoi du message à la boite aux lettres
		}
		pause();
	}
	return 0;
}

