/* Fetoiu Catalin-Emil 324CB */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "protocolUtils.h"

#define bufferSize 100

/* Vectorul cu utilizatorii posibili ai bancomatului */
int numberOfUsers;
User* users;

/* Vectori ce retin starea curenta a fiecarui user, mai exact daca acesta este
   logat in sistem si daca este blocat, precum si de cate ori au fost introduse
   credentiale gresite pentru acesta                                         */
int* loggedIn;
int* blockedUser;
int* wrongCredentials;

/* Vector ce retine, pentru fiecare client, utilizatorul ce il foloseste la
   momentul curent                                                          */
int* currentUser;

/* Citeste datele utilizatorilor din fisierul dat ca parametru la pornirea 
   server-ului, aceste date fiind stocate in vectorul users                 */
void readUserData(char* userDataFile) {
	FILE* userData = fopen(userDataFile, "r");

	fscanf(userData, "%d", &numberOfUsers);
	users = malloc(numberOfUsers * sizeof(User));
	if(users == NULL) {
		return;
	}

	char* name = calloc(14, sizeof(char));
	if(name == NULL) {
		return;
	}
	char* surname = calloc(14, sizeof(char));
	if(surname == NULL) {
		return;
	}
	int cardNumber;
	int pin;
	char* password = calloc(20, sizeof(char));
	if(password == NULL) {
		return;
	}
	double accountSum;

	int index;
	for(int index = 0; index < numberOfUsers; index++) {
		fscanf(userData, "%s%s", name, surname);
		fscanf(userData, "%d%d", &cardNumber, &pin);
		fscanf(userData, "%s", password);
		fscanf(userData, "%lf", &accountSum);

		users[index] = createUser(name, surname, cardNumber, pin,
									  password, accountSum);
	}

	free(name);
	free(surname);
	free(password);
	fclose(userData);
}

/* Returneaza indicele utilizatorului ce are numarul de card dat ca parametru.
   Este returnat -1 daca numarul de card nu este gasit                      */
int searchCardNumber(int cardNumber) {
	int index;
	for(index = 0; index < numberOfUsers; index++) {
		if(users[index].cardNumber == cardNumber) {
			return index;
		}
	}

	return -1;
}

/* Proceseaza o cerere de login primita de la un client */
char* loginRequest(int clientSocket, int cardNumber, int pin) {
	char* responseToClient = calloc(bufferSize, sizeof(char));

	int index = searchCardNumber(cardNumber);

	/* Se verifica existenta numarului de card */
	if(index == -1) {
		strcpy(responseToClient, "-4 : Numar card inexistent");
	} else {
		/* Este actualizat utilizatorul curent al socket-ului/client-ului de la
		   care a fost primita cererea de login                              */
		if(index != currentUser[clientSocket]) {
			currentUser[clientSocket] = index;
		}

		/* Se verifica daca utilizatorul este deja logat sau daca acesta are
		   cardul blocat si sunt contorizate credentialele gresite trimise */
		if(loggedIn[index]) {
			strcpy(responseToClient, "-2 : Sesiune deja deschisa");
		} else if(blockedUser[index]) {
			strcpy(responseToClient, "-5 : Card blocat");
		} else if(users[index].pin != pin) {
			wrongCredentials[index]++;

			if(wrongCredentials[index] >= 3) {
				strcpy(responseToClient, "-5 : Card blocat");

				blockedUser[index] = 1;
				wrongCredentials[index] = 0;
			} else {
				strcpy(responseToClient, "-3 : Pin gresit");
			}
		} else {
			sprintf(responseToClient, "Welcome %s %s",
			users[index].name, users[index].surname);

			loggedIn[index] = 1;
			wrongCredentials[index] = 0; 
		}
	}

	return responseToClient;
}

/* Proceseaza o cerere de logout primita de la un client */
char* logoutRequest(int clientSocket) {
	char* responseToClient = calloc(bufferSize, sizeof(char));

	/* Este setat ca nefiind logat utilizatorul curent al socket-ului pe care
	   a fost primita comanda                                               */
	int index = currentUser[clientSocket];
	loggedIn[index] = 0;
	currentUser[clientSocket] = -1;
	strcpy(responseToClient, "Deconectare de la bancomat");

	return responseToClient;
}

/* Returneaza sold-ul utilizatorului ce a trimis comanda listsold */
char* listsoldRequest(int clientSocket) {
	char* responseToClient = calloc(bufferSize, sizeof(char));

	int index = currentUser[clientSocket];
	double accountSum = users[index].accountSum;
	sprintf(responseToClient, "%.2lf", accountSum);

	return responseToClient;
}

/* Proceseaza o comanda de tip getmoney. Se verifica mai intai daca tranzactia
   poate fi efectuata (daca suma ceruta este multiplu de 10 si nu depaseste
   sold-ul curent), in caz afirmativ suma fiind eliminata din cont          */
char* getMoneyRequest(int clientSocket, int sum) {
	char* responseToClient = calloc(bufferSize, sizeof(char));

	int index = currentUser[clientSocket];
	double accountSum = users[index].accountSum;
	if(sum % 10 != 0) {
		strcpy(responseToClient, "-9 : Suma nu e multiplu de 10");
	} else if((double)sum > accountSum) {
		strcpy(responseToClient, "-8 : Fonduri insuficiente");
	} else {
		users[index].accountSum -= (double)sum;
		sprintf(responseToClient, "Suma %d retrasa cu succes", sum);
	}

	return responseToClient;
}

/* Proceseaza o comanda de tip putmoney, fiind adaugata in cont suma ceruta */
char* putMoneyRequest(int clientSocket, double sum) {
	char* responseToClient = calloc(bufferSize, sizeof(char));

	int index = currentUser[clientSocket];
	users[index].accountSum += sum;
	strcpy(responseToClient, "Suma depusa cu succes");

	return responseToClient;
}

/* Proceseaza o comanda de tip unlock. In cazul in care numarul de card
   apartine unui utilizator, este solicitata trimiterea parolei secrete a
   acestuia                                                              */
char* unlockRequest(int udpSocket, int cardNumber) {
	char* responseToClient = calloc(bufferSize, sizeof(char));

	int index = searchCardNumber(cardNumber);
	if(index == -1) {
		strcpy(responseToClient, "-4 : Numar card inexistent");
	} else {
		if(!blockedUser[index]) {
			strcpy(responseToClient, "-6 : Operatie esuata");
		} else {
			strcpy(responseToClient, "Trimite parola secreta");
		}
	}

	return responseToClient;
}

/* Proceseaza o cerere de deblocare a unui card. Se verifica daca parola
   primita de la client corespunde cu parola utilizatorului ce detine cardul */
char* passwordUnlockRequest(int udpSocket, int cardNumber, char* password) {
	char* responseToClient = calloc(bufferSize, sizeof(char));

	int index = searchCardNumber(cardNumber);
	if(strcmp(users[index].password, password) == 0) {
		strcpy(responseToClient, "Client deblocat");
		blockedUser[index] = 0;
	} else {
		strcpy(responseToClient, "-7 : Deblocare esuata");
	}

	return responseToClient;
}


int main(int argc, char* argv[]) {

	/* Sunt citite si stocate datele utilizatorilor posibili */
	readUserData(argv[2]);

	/* Este creat un socket udp pe care server-ul sa poata primi comenzile
	   de tip unlock de la clienti                                          */
	int udpSocket = createServerUdpSocket();
	if(udpSocket == -1) {
		return -1;
	}
	/* Este creat un socket pe care server-ul sa astepte conexiuni de tip tcp
	   de la clienti. Server-ul poate avea la un moment dat un numar de
	   clienti cel mult egal cu numarul de utilizatori, deoarece un utilizator
	   nu poate fi logat simultan folosind doi clienti diferiti             */
	int connectionSocket = initialiseServer(atoi(argv[1]), numberOfUsers);
	if(connectionSocket == -1) {
		return -1;
	}

	/* Este creata adresa ce va folosita pentru comunicatia pe canalul udp */
	struct sockaddr_in serverAddress = initialiseUdpServer(atoi(argv[1]),
														   udpSocket);

	/* Sunt initilizati vectorii ce retin starea curenta a fiecarui utilizator,
	   respectiv a fiecarui client conectat la server                        */
	loggedIn = calloc(numberOfUsers, sizeof(int));
	blockedUser = calloc(numberOfUsers, sizeof(int));
	wrongCredentials = calloc(numberOfUsers + 10, sizeof(int));
	currentUser = calloc(numberOfUsers + 10, sizeof(int));
	memset(currentUser, -1, numberOfUsers + 10);

	/* Multimea stream-urilor pe care server-ul primeste, respectiv trimite
	   date                                                                 */
	fd_set inputStreams;
	fd_set inputStreamsCopy;

	/* Indicele maxim al tuturor stream-urilor server-ului */
	int maximumStreamIndex = getmax(0, connectionSocket);
	maximumStreamIndex = getmax(maximumStreamIndex, udpSocket);

	FD_ZERO(&inputStreams);
	FD_ZERO(&inputStreamsCopy);

	/* Stream-urile initiale sunt stdin, socket-ul pe care se asculta dupa
	   conexiuni tcp, respectiv socketul udp al server-ului                 */
	FD_SET(0, &inputStreams);
	FD_SET(connectionSocket, &inputStreams);
	FD_SET(udpSocket, &inputStreams);

	/* Buffer-ele folosite pentru primirea, respectiv transmiterea de date */
	char* buffer = calloc(bufferSize, sizeof(char));
	char* responseToClient = calloc(bufferSize, sizeof(char));
	char* command = calloc(bufferSize, sizeof(char));
	char* password = calloc(bufferSize, sizeof(char));
	char* closingMessage = calloc(bufferSize, sizeof(char));
	strcpy(closingMessage, "serverClosing");


	/* Cat timp server-ul nu primeste comanda quit */
	while(1) {
		inputStreamsCopy = inputStreams;

		/* Este folosit apelul select pentru a determina stream-urile de pe
		   care pot fi citite date                                         */
		if(select(maximumStreamIndex + 1, &inputStreamsCopy, NULL, NULL,
		          NULL) == -1) {
			printf("[Server] Error on select\n");
		}

		int currentSocket;
		int closedServer = 0;

		/* Sunt parcurse toate stream-urile posibile, de la 0 la valoarea
		   stocata in maximumStreamIndex                                  */
		for(currentSocket = 0; currentSocket <= maximumStreamIndex; 
			currentSocket++) {

			/* Daca pot fi citite date de pe socket-ul curent */
			if(FD_ISSET(currentSocket, &inputStreamsCopy)) {

				/* Daca au fost introduse date de la tastatura */
				if(currentSocket == 0) {

					/* Este citita comanda primita */
					memset(command, 0, bufferSize);
					fgets(command, bufferSize, stdin);
					command[strlen(command) - 1] = '\0';

					/* Daca serverul primeste comanda quit, este trimis mesajul
					   "serverClosing" catre toti clientii conectati, fiind
					   apoi inchisi socket-ii tcp folosit pentru comunicatia
					   cu acestia                                            */
					if(strcmp(command, "quit") == 0) {
						int socket;
						for(socket = 3; socket <= maximumStreamIndex; socket++) {
							if(FD_ISSET(socket, &inputStreams) &&
							   socket != udpSocket &&
							   socket != connectionSocket) {

								int code = send(socket, closingMessage, 
									 			strlen(closingMessage), 0);
								if(code < 0) {
									printf("[Server] Error in send\n");
									return -1;
								}
								close(socket);
							}
						}

						closedServer = 1;
					}

				/* Daca server-ul primeste o cerere de conexiune */
				} else if(currentSocket == connectionSocket) {

					/* Conexiunea este acceptata, fiind creat un nou socket
					   tcp corespunzator noului client                     */
					int clientSocket = acceptClient(connectionSocket);
					if(clientSocket == -1) {
						return -1;
					}

					/* Socket-ul tcp creat este adaugat in multimea de
					   stream-uri                                      */
					FD_SET(clientSocket, &inputStreams);
					if(clientSocket > maximumStreamIndex) {
						maximumStreamIndex = clientSocket;
						break;
					}

				/* Daca au fost trimise date pe canalul udp */
				} else if(currentSocket == udpSocket) {
					memset(buffer, 0, bufferSize);
					memset(command, 0, bufferSize);
					memset(password, 0, bufferSize);
					int cardNumber;
					
					/* Sunt citite datele de pe socket-ul udp */
					int serverAddressLength = sizeof(serverAddress);
					int code = recvfrom(udpSocket, buffer, bufferSize, 0,
										(struct sockaddr*) &serverAddress,
										&serverAddressLength);
					if(code < 0) {
						printf("[Server] Error in recvfrom\n");
						return -1;
					}

					sscanf(buffer, "%s", command);

					/* Se verifica daca a fost primita comanda unlock sau daca
					   a fost primita parola corespunzatoare unui numar de
					   card, fiind apelata metoda unlockRequest sau metoda
					   passwordUnlockRequest                                */
					if(strcmp(command, "unlock") == 0) {
						sscanf(buffer + strlen(command), "%d", &cardNumber);
						responseToClient = unlockRequest(udpSocket, cardNumber);
					} else {
						sscanf(buffer, "%d%s", &cardNumber, password);
						responseToClient = passwordUnlockRequest(udpSocket,
							 									 cardNumber,
																 password);
					}

					/* Este trimis raspunsul catre client */
					code = sendto(udpSocket, responseToClient, strlen(responseToClient),
						   0, (struct sockaddr*) &serverAddress,
						   serverAddressLength);
					if(code < 0) {
						printf("[Server] Error in sendto\n");
						return -1;
					}

					free(responseToClient);

				/* Daca au fost trimise date pe unul din socket-ii tcp */
				} else {
					int clientSocket = currentSocket;
					int clientExited = 0;

					memset(buffer, 0, bufferSize);

					/* Sunt citite datele de pe socket */
					int code = recv(clientSocket, buffer, bufferSize, 0);
					if(code < 0) {
						printf("[Server] Error in send\n");
						return -1;
					}

					/* Este citita comanda trimisa catre server */
					memset(command, 0, bufferSize);
					sscanf(buffer, "%s", command);

					/* Este procesata comanda trimisa */
					if(strcmp(command, "login") == 0) {
						int cardNumber, pin;
						sscanf(buffer + strlen(command), "%d%d",
							   &cardNumber, &pin);

						responseToClient = loginRequest(clientSocket,
													    cardNumber, pin);
					}
					if(strcmp(command, "logout") == 0) {
						responseToClient = logoutRequest(clientSocket);
					}
					if(strcmp(command, "listsold") == 0) {
						responseToClient = listsoldRequest(clientSocket);
					}
					if(strcmp(command, "getmoney") == 0) {
						int sum;
						sscanf(buffer + strlen(command), "%d", &sum);

						responseToClient = getMoneyRequest(clientSocket, sum);
					}
					if(strcmp(command, "putmoney") == 0) {
						double sum;
						sscanf(buffer + strlen(command), "%lf", &sum);

						responseToClient = putMoneyRequest(clientSocket, sum);
					}

					/* Daca este primita comanda quit, este inchis socket-ul
					   tcp al client-ului care a trimis comanda             */
					if(strcmp(command, "quit") == 0) {
						FD_CLR(clientSocket, &inputStreams);
						close(clientSocket);

						int index = currentUser[clientSocket];
						if(index != -1) {
							loggedIn[index] = 0;
							currentUser[clientSocket] = -1;
						}
						clientExited = 1;
					}

					/* In cazul in care comanda primita nu a fost quit, este
					   trimis raspunsul inapoi catre client                  */
					if(!clientExited) {
						int code = send(clientSocket, responseToClient,
							 			strlen(responseToClient), 0);
						if(code < 0) {
							printf("[Server] Error in send\n");
						}
						free(responseToClient);
					}
				}
			}
		}

		if(closedServer) {
			break;
		}
	}

	/* Este inchis socket-ul udp, impreuna cu socket-ul pe care au fost primite
	   conexiuni, fiind de asemenea eliberata memoria folosita              */
	close(udpSocket);
	close(connectionSocket);

	free(loggedIn);
	free(blockedUser);
	free(wrongCredentials);
	free(currentUser);
	free(buffer);
	free(command);
	free(password);
	free(closingMessage);

	int index;
	for(index = 0; index < numberOfUsers; index++) {
		freeUser(users[index]);
	}
	free(users);

	return 0;
}