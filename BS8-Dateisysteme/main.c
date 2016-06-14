/*
 ============================================================================
 Name        : Bs8.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //fuer Kontrollfluss
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[]) { // argumetenzaehler, argumentenvektor

	int anzahlArgumente = argc-1;
	char * quelldatei;
	char * zieldatei;

	if(anzahlArgumente<=1)
		exit(EXIT_FAILURE);


	if(anzahlArgumente > 3){
		printf("Zu viele Argumente!, bitte nur Quell- und Zieldatei angeben.");
	} else {
		// Pfade einlesen und verarbeiten.
		quelldatei = argv[1];
		zieldatei = argv[2];

		printf("Programmname: %s\n", argv[0]);
		printf("Quelldatei: %s\n", quelldatei);
		printf("Zieldatei: %s\n", zieldatei);


		/* Quelldatei öffnen */
		int filedescriptorQuelle;
		mode_t modeQuelle = S_IRUSR;


		//Quelldatei Groesse ermitteln
		struct stat attribut;
		stat(quelldatei,&attribut);
		printf("GROESSE \n");
		printf("\t%lld \n",(long long) attribut.st_size);
		long long zielsize = (long long) attribut.st_size;

		if((filedescriptorQuelle = open(quelldatei, O_RDONLY, modeQuelle)) == -1){
		    fprintf(stderr, "Datei %s kann nicht geöffnet werden\n", quelldatei);
		    exit(1);
		} else {
			printf("Datei %s geöffnet\n", quelldatei);
		}



		/* Quelldatei Lesen */
		ssize_t bytes_read;
		char bufQuelle[zielsize]; //speicher für Dateiinhalt
		char bufpartone[zielsize/2];
		char bufparttwo[zielsize/2];

		bytes_read = read(filedescriptorQuelle, bufQuelle, zielsize);
		int half = zielsize/2;
		if(bytes_read == -1){
			printf("Inhalt konnte nicht gelesen werden bzw. Datei ist leer.\n");
		} else {
			printf("Datei konnte gelesen werden!\n");
			strncpy(bufpartone,bufQuelle+half,half);
			printf(bufpartone);
			strncpy(bufparttwo,bufQuelle,half);
			printf(bufparttwo);
		}


		/* Zieldatei öffnen/erstellen */
		int filedescriptorZiel;
		mode_t modeZiel = S_IRUSR | S_IWUSR;

		if((filedescriptorZiel = open(zieldatei, O_RDWR | O_CREAT, modeZiel)) == -1){
		    fprintf(stderr, "Datei %s kann nicht geöffnet/erstellt werden.\n", zieldatei);
		    exit(1);
		} else {
			printf("Datei %s geöffnet/erstellt\n", zieldatei);
		}


		/* In Zieldatei schreiben */
		char bufZiel[500];
		size_t nbytesZiel = sizeof(bufZiel);
		ssize_t bytes_written;
		//int filedescriptorZiel;
		size_t nbytes = strlen(bufZiel);
		bytes_written = write(filedescriptorZiel, bufZiel, nbytesZiel);



		/* Dateien schließen */
		if((close(filedescriptorQuelle)) == 0 && close(filedescriptorZiel) == 0){
			printf("Dateien wurden geschlossen!\n");
		} else {
			printf("Datei konnte nicht geschlossen werden!\n");
		}

	}

	return EXIT_SUCCESS;
}

