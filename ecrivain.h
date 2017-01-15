void ecrire(MESSAGE tirage){
	int i;
	semop(MUTEX,&down,1);
	for(i=0;i<TAILLE;i++){//Parcour de la BDD pour trouver une place libre
		if(pBDD[i].places==-1){//place = -1 correspond à une place libre
			pBDD[i].type=tirage.type;//Ecriture du vol
			sprintf(pBDD[i].destination,"%s",tirage.destination);
			pBDD[i].places=tirage.places;
			pBDD[i].pid=0;
			semop(SVols,&down,1);//décrémentation du nombre de places disponible dans la BDD
			break;//sortie de la fonction
		}
	}
	semop(MUTEX,&up,1);
}
