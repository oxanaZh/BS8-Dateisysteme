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

		if((filedescriptorQuelle = open(quelldatei, O_RDONLY, modeQuelle)) == -1){
		    fprintf(stderr, "Datei %s kann nicht geöffnet werden\n", quelldatei);
		    exit(1);
		} else {
			printf("Datei %s geöffnet\n", quelldatei);
		}


		/* Quelldatei Lesen */
		//struct stat attribut; //Dateigröße
		//long int dateigroesse = attribut.st_size;
		size_t nbytes;
		ssize_t bytes_read;
		char bufQuelle[500]; //speicher für Dateiinhalt
		nbytes = sizeof(bufQuelle);
		bytes_read = read(filedescriptorQuelle, bufQuelle, nbytes);

		if(bytes_read == -1){
			printf("Inhalt konnte nicht gelesen werden bzw. Datei ist leer.\n");
		} else {
			printf("Datei konnte gelesen werden!\n");
			//printf("Dateigröße %ld Bytes.\n", dateigroesse);
			printf("Datei-Inhalt: \n%s\n", bufQuelle);
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
		nbytes = strlen(bufZiel);
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

