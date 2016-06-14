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


void backupFileContent(int descriptor, char *buffer, int size);

int main(int argc, char *argv[]) { // argumetenzaehler, argumentenvektor

	int anzahlArgumente = argc - 1;
	char * quelldatei;
	char * zieldatei;

	int filedescriptorQuelle;
	mode_t modeQuelle = S_IRUSR;
	long long quellsize;

	int filedescriptorZiel;
	mode_t modeZiel = S_IRUSR | S_IWUSR;
	long long targetsize;

	if (anzahlArgumente <= 1)
		exit(EXIT_FAILURE);

	if (anzahlArgumente > 3) {
		printf("Zu viele Argumente!, bitte nur Quell- und Zieldatei angeben.");
	} else {
		// Pfade einlesen und verarbeiten.
		quelldatei = argv[1];
		zieldatei = argv[2];

		//Quelldatei Groesse ermitteln
		struct stat quellstat;
		stat(quelldatei, &quellstat);
		quellsize = (long long) quellstat.st_size;

		if ((filedescriptorQuelle = open(quelldatei, O_RDONLY, modeQuelle))
				== -1) {
			fprintf(stderr, "Datei %s kann nicht geöffnet werden\n",
					quelldatei);
			exit(1);
		} else {
			printf("Datei %s geöffnet\n", quelldatei);
		}

		/* Zieldatei öffnen/erstellen */

		if ((filedescriptorZiel = open(zieldatei, O_RDWR | O_CREAT, modeZiel))
				== -1) {
			fprintf(stderr, "Datei %s kann nicht geöffnet/erstellt werden.\n",
					zieldatei);
			exit(1);
		} else {
			printf("Datei %s geöffnet/erstellt\n", zieldatei);
		}

		/* Quelldatei Lesen */
		ssize_t error;
		char bufQuelle[quellsize]; //speicher für Dateiinhalt
		char bufpartone[quellsize / 2];
		char bufparttwo[quellsize / 2];

		backupFileContent(filedescriptorQuelle,bufQuelle,quellsize);

		int half = quellsize / 2;
		printf("Datei konnte gelesen werden!\n");
		strncpy(bufpartone, bufQuelle + half, half);
		printf(bufpartone);
		strncpy(bufparttwo, bufQuelle, half);
		printf(bufparttwo);


		/* Ziel datei lesen*/
		struct stat zielstat;
		stat(zieldatei, &zielstat);
		targetsize = (long long) zielstat.st_size;

		if (lseek(filedescriptorZiel, 10, SEEK_SET) == -1)
			return 1;

		char bufeleventoend[targetsize - 10];
		backupFileContent(filedescriptorZiel,bufeleventoend,targetsize-10);


		/* In Zieldatei schreiben */
		ssize_t bytes_written;

		char lastchars[11];

		strncpy(lastchars, bufQuelle + quellsize - 11, 10);

		if (lseek(filedescriptorZiel, 10, SEEK_SET) == -1)
			return 1;

		bytes_written = write(filedescriptorZiel, lastchars, 10);
		bytes_written = write(filedescriptorZiel, bufeleventoend,
				sizeof(bufeleventoend));


		//Neue zusammengestellte Datei ausgeben
		stat(zieldatei, &zielstat);
		targetsize = (long long) zielstat.st_size;
		char fulltarget[targetsize];
		if (lseek(filedescriptorZiel, 0, SEEK_SET) == -1)
			return 1;
		backupFileContent(filedescriptorZiel,fulltarget,targetsize);
		puts(fulltarget);

		/* Dateien schließen */
		if ((close(filedescriptorQuelle)) == 0
				&& close(filedescriptorZiel) == 0) {
			printf("Dateien wurden geschlossen!\n");
		} else {
			printf("Datei konnte nicht geschlossen werden!\n");
		}

	}

	return EXIT_SUCCESS;
}
void backupFileContent(int descriptor, char *buffer, int size) {
	size_t error = read(descriptor, buffer, size);
			if (error == -1) {
				printf("Inhalt konnte nicht gelesen werden bzw. Datei ist leer.\n");
			} else {
				printf("Datei konnte gelesen werden!\n");
			}
}
void copyFile(int zielDiscr, int quelDiscr) {

}

