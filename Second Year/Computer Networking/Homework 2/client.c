/* Fetoiu Catalin-Emil 324CB */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "protocolUtils.h"

#define bufferSize 100

/* Acest flag este folosit pentru a retine faptul ca un utilizator este
   logat de pe procesul client                                          */
int openSession;

/* Datele utilizatorului curent al client-ului */
char* command;
char* password;
int cardNumber, pin;
double sum;

/* Extrage datele introduse de la tastatura de catre utilizator */
void parseUserInput(char* buffer) {
	memset(command, 0, bufferSize);
	memset(password, 0, bufferSize);

	sscanf(buffer, "%s", command);
	if(strcmp(command, "login") == 0) {
		sscanf(buffer + strlen(command), "%d%d", &cardNumber, &pin);
	}
	if(strcmp(command, "getmoney") == 0) {
		sscanf(buffer + strlen(command), "%lf", &sum);
	}
	if(strcmp(command, "putmoney") == 0) {
		sscanf(buffer + strlen(command), "%lf", &sum);
	}
}


int main(int argc, char* argv[]) {

	/* Este determinat pid client-ului */
	int processPid = getpid();

	/* Este creat fisierul de log al client-ului */
	char* clientLogFile = calloc(bufferSize, sizeof(char));
	sprintf(clientLogFile, "client-%d.log", processPid);
	FILE* log = fopen(clientLogFile, "w");

	/* Este creat socket-ul tcp folosit pentru comunicatia cu server-ul */
	int tcpSocket = connectToTcpServer(argv[1], atoi(argv[2]));
	if(tcpSocket == -1) {
		return -1;
	}

	/* Este creat un socket udp, impreuna cu adresa server-ului catre care
	   vor fi trimise date pe canalul udp                                  */
	struct sockaddr_in serverAddress = createUdpServerAddress(argv[1],
															  atoi(argv[2]));
	int udpSocket = createClientUdpSocket();
	if(udpSocket == -1) {
		return -1;
	}

	openSession = 0;

	/* Multimea stream-urilor pe care client-ul primeste date */
	fd_set inputStreams;
	fd_set inputStreamsCopy;
	int maximumStreamIndex = getmax(tcpSocket, udpSocket);

	FD_ZERO(&inputStreams);
	FD_ZERO(&inputStreamsCopy);

	/* Stream-urile pe care client-ul primeste date sunt stdin, socket-ul
	   tcp, respectiv socket-ul udp                                       */
	FD_SET(tcpSocket, &inputStreams);
	FD_SET(udpSocket, &inputStreams);
	FD_SET(0, &inputStreams);

	/* Sunt initializate buffer-ele folosite in comunicatia cu server-ul, precum
	   si datele utilizatorului ce foloseste client-ul                       */
	char* buffer = calloc(bufferSize, sizeof(char));
	char* serverResponse = calloc(bufferSize, sizeof(char));
	command = calloc(bufferSize, sizeof(char));
	password = calloc(bufferSize, sizeof(char));
	cardNumber = -1, pin = -1;

	int unlockRequest = 0;

	/* Cat timp este realizata comunicatia cu server-ul */
	while(1) {
		inputStreamsCopy = inputStreams;

		/* Este folosit apelul select pentru a determina stream-urile de pe
		   care pot fi citite date                                          */
		if(select(maximumStreamIndex + 1, &inputStreamsCopy,
		          NULL, NULL, NULL) == -1) {
			printf("[Client] Error on select\n");
		} else {

			/* Daca au fost introduse date de la tastatura */
			if(FD_ISSET(0, &inputStreamsCopy)) {
				/* Datele sunt citite, fiind scrise si in fisierul de log */
				memset(buffer, 0, bufferSize);
				fgets(buffer, bufferSize, stdin);
				buffer[strlen(buffer) - 1] = '\0';
				fprintf(log, "%s\n", buffer);

				/* Sunt extrase datele utilizatorului */
				parseUserInput(buffer);

				/* Daca a fost primita comanda quit, server-ul este anuntat
				   de intentia client-ului, fiind apoi oprita comunicatia */
				if(strcmp(command, "quit") == 0) {
					int code = send(tcpSocket, buffer, strlen(buffer), 0);
					if(code < 0) {
						printf("[Client] Error in send\n");
						return -1;
					}
					break;

				/* Daca a fost primita comanda unlock, aceasta este trimisa
				   catre server impreuna cu numarul de card corespunzator   */
				} else if(strcmp(command, "unlock") == 0) {
					int serverAddressLength = sizeof(serverAddress);
					sprintf(buffer, "unlock %d", cardNumber);
					int code = sendto(udpSocket, buffer, strlen(buffer), 0,
						   			  (struct sockaddr*) &serverAddress,
						   			  serverAddressLength);
					if(code < 0) {
						printf("[Client] Error in sendto\n");
						return -1;
					}

					unlockRequest = 1;

				/* Daca a fost introdusa parola pentru unlock, se verifica mai
				   intai daca aceasta a fost precedata de o comanda unlock, in
				   caz afirmativ fiind trimisa catre server impreuna cu numarul
				   de card corespunzator                                    */
				} else if(unlockRequest) {
					memset(password, 0, bufferSize);
					strcpy(password, buffer);
					sprintf(buffer, "%d %s", cardNumber, password);

					int serverAddressLength = sizeof(serverAddress);
					int code = sendto(udpSocket, buffer, strlen(buffer), 0,
						   			  (struct sockaddr*) &serverAddress,
						   			  serverAddressLength);
					if(code < 0) {
						printf("[Client] Error in sendto\n");
						return -1;
					}

					unlockRequest = 0;

				/* Daca s-a incercat logarea in timp ce alta sesiune este deja
				   deschisa                                                  */
				} else if(strcmp(command, "login") == 0 && openSession) {
					printf("-2 : Sesiune deja deschisa\n");
					fprintf(log, "-2 : Sesiune deja deschisa\n");
					fprintf(log, "\n");
					unlockRequest = 0;

				/* Daca nu este deschisa nicio sesiune si se incearca folosirea
				   altei comenzi inafara de login                            */
				} else if(strcmp(command, "login") != 0 && !openSession) {
					printf("-1 : Clientul nu este autentificat\n");
					fprintf(log, "-1 : Clientul nu este autentificat\n");
					fprintf(log, "\n");
					unlockRequest = 0;

				/* In cazul unei comenzi valide, aceasta este trimisa catre
				   server pe socket-ul tcp                                  */
				} else {
					int code = send(tcpSocket, buffer, strlen(buffer), 0);
					if(code < 0) {
						printf("[Client] Error in send\n");
						return -1;
					}
					unlockRequest = 0;
				}
			}

			/* Daca au fost primite date pe socket-ul tcp */
			if(FD_ISSET(tcpSocket, &inputStreamsCopy)) {
				memset(buffer, 0, bufferSize);
				
				/* Sunt citite datele de pe socket */
				int code = recv(tcpSocket, buffer, bufferSize, 0);
				if(code < 0) {
					printf("[Client] Error in recv\n");
					return -1;
				}

				/* Daca server-ul a anuntat intentia de inchidere, comunicatia
				   este oprita                                              */
				if(strcmp(buffer, "serverClosing") == 0) {
					break;
				}

				/* Daca logarea a fost efectuata cu succes, este activat
				   flag-ul openSession                                   */
				if(strcmp(command, "login") == 0) {
					if(buffer[0] != '-') {
						openSession = 1;
					}
				}
				/* In caz de logout, flagul este setat la 0 */
				if(strcmp(command, "logout") == 0) {
					openSession = 0;
				}

				memset(serverResponse, 0, bufferSize);
				strcpy(serverResponse, "ATM> ");
				strcpy(serverResponse + 5, buffer);

				/* Este afisat si scris in fisierul de log raspunsul primit
				   de la server                                             */
				printf("%s\n", serverResponse);
				fprintf(log, "%s\n", serverResponse);
				fprintf(log, "\n");
			}

			/* Daca au fost primite date pe socket-ul udp */
			if(FD_ISSET(udpSocket, &inputStreamsCopy)) {
				memset(buffer, 0, bufferSize);

				/* Sunt citite datele de pe socket */
				int serverAddressLength = sizeof(serverAddress);
				int code = recvfrom(udpSocket, buffer, bufferSize, 0, (struct sockaddr*)
									&serverAddress, &serverAddressLength);
				if(code < 0) {
					printf("[Client] Error in recvfrom\n");
					return -1;
				}

				memset(serverResponse, 0, bufferSize);
				strcpy(serverResponse, "UNLOCK> ");
				strcpy(serverResponse + 8, buffer);

				/* Este afisat si scris in fisierul de log raspunsul primit
				   de la server                                             */
				printf("%s\n", serverResponse);
				fprintf(log, "%s\n", serverResponse);
				fprintf(log, "\n");
			}
		}
	}

	/* Sunt inchisi socket-ii tcp si udp folositi, de asemenea fiind inchis
	   fisierul de log si fiind eliberata memoria folosita                 */
	close(udpSocket);
	close(tcpSocket);

	fclose(log);
	free(buffer);
	free(serverResponse);
	free(command);
	free(password);

	return 0;
}