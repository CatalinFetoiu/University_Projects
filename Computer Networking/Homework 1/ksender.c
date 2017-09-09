#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib.h"
#include "protocolUtils.h"

#define HOST "127.0.0.1"
#define PORT 10000



int main(int argc, char*argv[]) {
	msg send;
	msg* received;

	init(HOST, PORT);

	/* sunt scrise in mesaj datele corespunzatoare pachetuli send-init, impreuna
	   cu crc-ul calculat, fiind apoi trimise catre receptor                  */
	unsigned char* initData = writeInitData(createInitData());
	Package initPackage = createPackage(0, initData, 11, 'S');
	writePackage(initPackage, &send);
	unsigned short crc = crc16_ccitt(send.payload, getCrcPosition(initPackage));
	memcpy(send.payload + getCrcPosition(initPackage), &crc, 2);
	send_message(&send);

	/* este trimis pachetul de tip send-init de maxim 3 ori */
	int countTimeouts = 0;
	while(1) {
		if(countTimeouts == 3) {
			return -1;
		}

		received = receive_message_timeout(maxWaitingTime + 50);
		if(received != NULL) {
			countTimeouts = 0;

			if(received->payload[3] == 'Y') {
				break;
			}
			else {
				send_message(&send);
			}
		}
		/* este incrementat contorul ce retine numarul de timeout-uri */
		else {
			send_message(&send);
			countTimeouts++;
		}
	}


	char* fileName = calloc(maxPackageLength, sizeof(char));
	if(fileName == NULL) {
		return -1;
	}
	Package package;

	int numberOfFiles = argc;
	int i;
	int sendEot = 0;

	/* sunt trimise toate fisierele primite ca parametrii in linia de comanda,
	   fiind apoi trimis pachetul de tip end of transfer                     */
	for(i = 1; i <= numberOfFiles; i++) {

		unsigned char* buffer;
		int fid;

		/* daca mai sunt fisiere de trimis, acestea sunt deschise pentru citire */
		if(!sendEot) {
			fid = open(argv[i], O_RDONLY);
			buffer = calloc(maxPackageLength + 5, sizeof(char));
			if(buffer == NULL) {
				return -1;
			}
		}

		/* este initializat contorul ce retine urmatoarea secventa ce trebuie
		trimisa, impreuna cu variabile folosite pentru a indica ca se citesc
		date din fisier sau ca urmeaza trimiterea unui pachet de tip file header
		sau end of file                                                       */
		unsigned char nextSequence = 0;
		int differentPackage = 1;
		int fileEnd = 0;
		char type = 'D';
		int fileHeader = 1;
		int countTimeouts = 0;

		/* cat timp mai sunt date de trimis din fisier */
		while(1) {

			/* daca un pachet a fost retransmis de 3 ori la rand, executia
			emitatorului este intrerupta                                   */
			if(countTimeouts == 3) {
				close(fid);
				free(buffer);
				return -1;
			}

			/* daca trebuie trimis un pachet ce contine date noi din fisier */
			if(differentPackage) {

				/* este creat pachetul de tip end of transfer */
				if(sendEot) {
					package = createPackage(nextSequence, (unsigned char*)"", 0, 'B');
				}

				/* este creat pachetul de tip file header */
				else if(fileHeader) {
					memcpy(fileName, argv[i], strlen(argv[i]));
					int length = strlen(argv[i]);
					fileName[length] = '\0';
					package = createPackage(nextSequence, (unsigned char*) fileName,
											strlen(fileName), 'F');
					fileHeader = 0;
				}

				/* sunt citite date fisier si este creat un pachet ce le contine,
				   de asemenea este detectat tipul pachetului (data sau end of
				   file) in functie de numarul de octeti cititi din fisier    */
				else {
					memset(buffer, '\0', maxPackageLength + 5);
					unsigned char size = read(fid, buffer, maxPackageLength);
					buffer[size] = '\0';

					if(size == 0 || size < maxPackageLength) {
						fileEnd = 1;
						type = 'E';
					}
					else {
						type = 'D';
					}
					package = createPackage(nextSequence, buffer, size, type);
				}	
			}
			/* daca datele din pachet nu trebuiesc modificate, este actualizat
			doar numarul sau de secventa                                     */
			else {
				package.sequenceNumber = nextSequence;
			}

			/* este trimis pachetul continand date din fisier, impreuna cu
			crc-ul asociat acestuia                                       */
			memset(send.payload, '\0', sizeof(send.payload));
			writePackage(package, &send);
			unsigned short crc = crc16_ccitt(send.payload, getCrcPosition(package));
			memcpy(send.payload + getCrcPosition(package), &crc, 2);
			send_message(&send);

			/* se asteapta maxWaitingTime milisecunde pentru primirea unui
			respuns din partea receptorului								   */
			received = receive_message_timeout(maxWaitingTime + 50);
			
			/* daca s-a primit timeout, pachetul ce urmeaza a fi trimis ramane
			   neschimbat                                                     */
			if(received == NULL) {
				countTimeouts++;
				differentPackage = 0;
			}

			/* daca s-a primit un raspuns din partea receptorului */
			else {
				countTimeouts = 0;

				/* daca s-a primit ack pentru ultimul pachet trimis */
				if(received->payload[3] == 'Y') { //s-a primit ack
					unsigned char arrivingSequence = received->payload[2];
					
					/* in cazul in care confirmarea este pentru ultimul pachet
					trimis, este incrementat numarul de secventa             */
					int aux = (nextSequence + 1) % 64;
					if(aux == arrivingSequence) {
						nextSequence = (nextSequence + 2) % 64;
						differentPackage = 1;
						if(fileEnd || sendEot) {
							break;
						}
					}

					/* in caz contrar pachetul ramane acelasi */
					else {
						differentPackage = 0;
					}
				}

				/* daca s-a primit un nak pentru ultimul pachet trimis */
				else if(received->payload[3] == 'N') {
					unsigned char arrivingSequence = received->payload[2];
					differentPackage = 0;

					/* in cazul in care confirmarea negativa este pentru ultimul 
					pachet trimis, este incrementat numarul de secventa       */
					int aux = (nextSequence + 1) % 64;
					if(aux == arrivingSequence) {
						nextSequence = (nextSequence + 2) % 64;
					}
				}

				else {
					differentPackage = 0;
				}
			}
		} 

		/* dupa transmiterea intregului fisier, acesta este inchis, fiind
		eliberat si zona de memorie in care au fost citite datele           */
		close(fid);
		free(buffer);

		/* dupa transmiterea ultimul fisier, este activat flag-ul ce indica
		faptul ca urmeaza trimiterea pachetului de tip end of transfer      */
		if(i == numberOfFiles - 1) {
			sendEot = 1;
		}
	}

	free(fileName);

	return 0;
}
