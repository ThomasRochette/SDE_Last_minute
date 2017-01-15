#ifndef _FONCTION_LAST_MINUTES_
#define _FONCTION_LAST_MINUTES_

int randBorne(int a, int b) {//Fonction des donnes un nombre aléatoire compris entre le nombre a et b
	return rand()%(b-a) +a;
}

void YES(){
	printf("Demande accordée\n");
}

void NO(){
	printf("Demande refusée\n");
}

void stopUsr(){
	exit(0);
}

void stopTir() {
	system("skill -2 affichage");// fermeture des autres processus
	system("skill -2 utilisateur");
	system("skill -2 ecrivain");
	system("skill -2 agence");

	fclose(destFile);
	close(descpipe[1]);
	close(descpipe[0]);
	exit(0);
}

void stopAgn(){//Fonction d'arret d'agence
	system("skill -2 affichage");// fermeture des autres processus
	system("skill -2 utilisateur");
	system("skill -2 ecrivain");
	system("skill -2 tirage");

	shmdt(pBDD);// Détachement de la mémoire partagée
	msgctl(BAL, IPC_RMID, NULL);// Destruction de la boite au lettre
	exit(0);// Fermeture du processus
}

void stopAff(){//Fonction d'arret d'Affichage
	system("skill -2 agence");// fermeture des autres processus
	system("skill -2 utilisateur");
	system("skill -2 ecrivain");
	system("skill -2 tirage");

	shmdt(pBDD);//Détachement de la mémoire partagée

	exit(0);
}

void stopEcr(){
	system("skill -2 agence");// fermeture des autres processus
	system("skill -2 utilisateur");
	system("skill -2 affichage");
	system("skill -2 tirage");

	shmdt(pBDD);    // Détachement de la mémoire partagée
	shmctl(BDDVols, IPC_RMID, NULL);  // Contrôle sur le segment de mémoire partagée : destruction du segment
	semctl(MUTEX, 1, IPC_RMID,0);     // Opérations de contrôle sur le mutex
	semctl(SVols, 1, IPC_RMID,0);      // Opérations de contrôle sur le sémaphore Svol
	exit(0);
}


#endif
