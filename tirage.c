#include "struct.h"
#include "fonction.h"
#include "ecrivain.h"


int main() {
	int pidEcrivain = 0;                        // Initialise de la variable qui contiendra le PID du processus Ecrivain
	int descpipe[2];                            // Descripteur de tube

	if (pipe(descpipe)==-1) {                   // Message d'erreur pour la création du tube
		perror("Erreur de création pipe\n");
		exit(-1);
	}
	else
	{
		printf("PID : %d Création tube ok\n",getpid());
		pidEcrivain = fork();                   // Création du fils

		if (pidEcrivain > 0) {                  //  *************  CODE DU PERE : tirage.c
			signal(SIGQUIT,stopTir);
			signal(SIGHUP,stopTir);
			signal(SIGINT,stopTir);                 //    Détourne les signaux vers la fonction stopTir

			/* Tirage de la destination et du nombre de place */
			srand(time(NULL));                      // Initialise la fonction srand sur le temps actuel.
			char destChoose[TAILLE_DEST];
			int rowRand= 0;
			int placeDispo = 0;
			int cursor=0;
			char currentChar;
			MESSAGE tirage;                        // Definit une variable tirage de type MESSAGE

			while(1){        

				destFile = fopen("destFile", "r");              // Déclare le fichier texte de destination en lecture seule

				if (destFile != NULL)                           // Test si l'ouverture du fichier est OK
				{

					// Les destinations sont dans un fichiers texte : destFile ou chaque ligne correspond
					// a une nouvelle destionation. Pour cela on vient déterminer une ligne (qui sera notre destination)
					// au hasard et on parcout le fichier texte jusqu'à cette ligne

					rowRand=randBorne(0,NOMBRE_DE_DEST);          // Définis une ligne au hasard
					while (cursor!=rowRand) {                     // Balaye le fichier de destination jusqu'à la ligne tirée
						while ((currentChar=fgetc(destFile) ) != '\n') {}
						cursor++;
					}
					cursor=0;
					placeDispo=randBorne(1,199);          // Tire au hasard un nombre de place
					fgets(destChoose, TAILLE_DEST, destFile);     // On lit la chaine de caractère jusqu'à TAILLE_DEST dans le fichier destFile et on la stocke dans la chaine destChoose
					fclose(destFile);                             // Ferme le fichier texte
				}
				else        perror("Impossible d'ouvrir le fichier destination\n");

				/* Envoie par le tube à écrivain */

				// Déclaration de la structure et initialisation de ses champs
				tirage.type = 2;
				sprintf(tirage.destination,"%s",destChoose);    // Obliger à mettre pour déclarer une chaine dans la stucture
				tirage.places = placeDispo;
				tirage.pid = getpid();

				close(descpipe[0]);
				write(descpipe[1],&tirage,sizeof(tirage));      // Ecriture dans le tube
				usleep(100);                                    // Pause dans 100 µs pour synchroniser les processus
				sleep(randBorne(1,10));                         // Sleep sur un temps aléatoire

			}
		}
		else if (pidEcrivain == -1) {
			perror("Problème création processus\n");
		}

		else if (pidEcrivain==0) {                 //*************  CODE DU FILS : ecrivain.c
			signal(SIGHUP,stopEcr);
			signal(SIGQUIT,stopEcr);
			signal(SIGINT,stopEcr);
			// Création des IPC : mémoire partager et sémaphore pour controlé la mémoire
			if ((BDDVols=shmget(cle_BDDVols,TAILLE*sizeof(MESSAGE),0777|IPC_CREAT))==-1)exit(1);
			if ((MUTEX=semget(cle_Mutex, 1, 0666|IPC_CREAT))==-1)exit(1);
			if ((SVols=semget(cle_SVols, 1, 0666|IPC_CREAT))==-1)exit(1);// Création de la BDD et des sémaphores


			// Attachement a la mémoire partagé
			pBDD=(MESSAGE*)shmat(BDDVols, 0, 0);
			if ((semctl(MUTEX, 0, SETVAL, 1))==-1)exit(1);
			if ((semctl(SVols, 0, SETVAL, 20))==-1)exit(1);

			semop(MUTEX,&down,1);
			int i;
			for(i=0;i<TAILLE;i++){//Initialisation de la BDD
				pBDD[i].type=0;
				sprintf(pBDD[i].destination,"%s","NULL");
				pBDD[i].places=-1;
				pBDD[i].pid=0;
			}
			semop(MUTEX,&up,1);
			MESSAGE tirage;      // Declarer la variable tirage de type message

			while(1){
				printf("pid : %d FILS!!", getpid());

				close(descpipe[1]);
				usleep(100);                                  // Pause dans 100 µs pour synchroniser les processus
				read(descpipe[0],&tirage,sizeof(tirage));     // Lecture dans le tube
				ecrire(tirage);//Ecriture des la destination dans la BDD

				tirage.type=0;//Remise à zéro pour ne pas écrire plusieurs fois la même destination 
				sprintf(tirage.destination,"%s","NULL");
				tirage.places=-1;
				tirage.pid=0;

			}
		}
	}
	return 0;
}

