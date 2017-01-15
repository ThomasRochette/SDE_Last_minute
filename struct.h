#ifndef _STRUCT_LAST_MINUTES_
#define _STRUCT_LAST_MINUTES_

// Librairies
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Lib pour les fork()
#include <sys/types.h>  // Lib pour la synchronisation des processus
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define TAILLE_DEST 4   // 4 pour prendre en compte le caractère de fin de chaine
#define NOMBRE_DE_DEST 10// Nombre de destinations dans le fichier


// Structure
typedef struct{
	long type;
	char destination[4];
	int places;
	pid_t pid;
}MESSAGE;

// Variables
FILE* destFile = NULL;//Fichier des destination
int descpipe[2];//Descripteur du tube


key_t cle_BAL=0x100;//Clé de la boite aux lettres
key_t cle_BDDVols=0x200;//Clé de la mémoire partagée
key_t cle_SVols=0x300;//Clé du sémaphore comptant le nombre de destination dans la mémoire
key_t cle_Mutex=0x400;//Clé du mutex
int BAL;					// File de message \ Boite au lettre
int BDDVols;  		// Base de données des vols
int MUTEX;				// Sémaphore mutex
int SVols;        // Sémaphore Vol
int TAILLE=20;    // Taille pour la mémoire
MESSAGE *pBDD=NULL; // Pointeur NULL de type MESSAGE

struct sembuf up={0,1,0}, down={0,-1,0};  // Pour sémaphore

#endif
