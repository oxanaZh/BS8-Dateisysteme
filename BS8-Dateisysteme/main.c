
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //fuer Kontrollfluss
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define MAX_PATH 1024
#define OUT 1

extern int errno;

void backupFileContent(int descriptor, char *buffer, int size);
void printHalfFile(int filesize , char *bufQuelle);
int copyFile(int zielDiscr, int targetsize, int  quellsize, char *bufQuelle);

int main(int argc, char *argv[]) { // argumetenzaehler, argumentenvektor


	char quellpath[MAX_PATH];
	char zielpath[MAX_PATH];

	int filedescriptorQuelle;
	int filedescriptorZiel;

	mode_t modeQuelle = S_IRUSR;
	mode_t modeZiel = S_IRUSR | S_IWUSR;

	long long quellsize;
	long long targetsize;

	printf("argv %d\n", argc);
	printf("argv[1] %s\n", argv[1]);
	printf("argv[2] %s\n", argv[2]);

	if (argc <= 2){
		perror("Error: too few arguments");
		exit(EXIT_FAILURE);
	}



	// Pfade einlesen und verarbeiten.

	if(realpath(argv[1], quellpath) == NULL){
		perror("Error argv[1]:");
		exit(EXIT_FAILURE);
	}
	if(realpath(argv[2], zielpath) == NULL){
			perror("Error argv[2]:");
			exit(EXIT_FAILURE);
		}

	//Datei Groesse ermitteln
	struct stat quellstat;
	stat(quellpath, &quellstat);
	quellsize = (long long) quellstat.st_size;

	struct stat zielstat;
	stat(zielpath, &zielstat);
	targetsize = (long long) zielstat.st_size;


	char bufQuelle[quellsize]; //speicher für Dateiinhalt


	if ((filedescriptorQuelle = open(quellpath, O_RDONLY, modeQuelle)) == -1) {
		perror("Error by opening quellpath:");
		exit(1);
	}

	/* Zieldatei öffnen/erstellen */

	if ((filedescriptorZiel = open(zielpath, O_RDWR | O_CREAT, modeZiel))
			== -1) {
		perror("Error by opening zielpath:");
		exit(1);
	}

	backupFileContent(filedescriptorQuelle, bufQuelle, quellsize);


	printHalfFile(quellsize , bufQuelle);

	if(copyFile(filedescriptorZiel, targetsize, quellsize, bufQuelle)==-1){
		perror("Error by copyFile: ");
		exit(EXIT_FAILURE);
	}


	/* Dateien schließen */
	if ((close(filedescriptorQuelle)) == 0 && close(filedescriptorZiel) == 0) {
		printf("Dateien wurden geschlossen!\n");
	} else {
		perror("Datei konnte nicht geschlossen werden!\n");
	}

	return EXIT_SUCCESS;
}
void backupFileContent(int descriptor, char *buffer, int size) {
	size_t error = read(descriptor, buffer, size);
	if (error == -1) {
		perror("Inhalt konnte nicht gelesen werden bzw. Datei ist leer.\n");
	}
}
int copyFile(int zielDiscr, int targetsize, int  quellsize, char *bufQuelle) {

	if (lseek(zielDiscr, 10, SEEK_SET) == -1){
		perror("Couldnt set pointer ");
		return -1;
	}

	char bufeleventoend[targetsize - 10];
	backupFileContent(zielDiscr, bufeleventoend, targetsize - 10);

	/* In Zieldatei schreiben */
	ssize_t bytes_written;

	char lastchars[11];

	strncpy(lastchars, bufQuelle + quellsize - 11, 10);

	if (lseek(zielDiscr, 10, SEEK_SET) == -1){
		perror("Couldnt set pointer ");
		return -1;
	}

	bytes_written = write(zielDiscr, lastchars, 10);
	bytes_written = write(zielDiscr, bufeleventoend,
			sizeof(bufeleventoend));
}
void printHalfFile(int filesize , char *bufQuelle){


		int secondhalf = ceil(filesize/2);
		int firsthalf = filesize/2;
		char bufpartone[secondhalf];
		char bufparttwo[firsthalf];

		strncpy(bufpartone, bufQuelle + firsthalf, secondhalf);
		write(OUT,bufpartone,secondhalf);
		strncpy(bufparttwo, bufQuelle, firsthalf);
		write(OUT,bufparttwo, firsthalf);
}

