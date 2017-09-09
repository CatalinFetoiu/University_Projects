/* Fetoiu Catalin-Emil 324CB */

#ifndef protocolUtils
#define protocolUtils

/* Structura folosita pentru a retine datele unui utilizator */
typedef struct {
	char* name;
	char* surname;
	int cardNumber;
	int pin;
	char* password;
	double accountSum;
} User;

/* Creeaza un utilizator avand datele primite ca parametrii */
User createUser(char* name, char* surname, int cardNumber, int pin,
					char* password, double accountSum) {

	User user;
	user.name = calloc(14, sizeof(char));
	if(user.name == NULL) {
		return user;
	}
	user.surname = calloc(14, sizeof(char));
	if(user.surname == NULL) {
		return user;
	}
	user.password = calloc(20, sizeof(char));
	if(user.password == NULL) {
		return user;
	}

	strcpy(user.name, name);
	strcpy(user.surname, surname);
	user.cardNumber = cardNumber;
	user.pin = pin;
	strcpy(user.password, password);
	user.accountSum = accountSum;

	return user;
}

/* Este eliberata memoria folosita pentru a stoca datele unui utilizator */
void freeUser(User user) {
	free(user.name);
	free(user.surname);
	free(user.password);
}

int getmax(int a, int b) {
	if(a > b) {
		return a;
	} else {
		return b;
	}
}

/* Este creat un socket pe care server-ul sa astepte conexiuni de tip tcp de
   la clienti, numarul maxim de clienti posibili la un moment dat fiind
   dat ca parametru                                                     */
int initialiseServer(int portNumber, int numberOfClients) {
	struct sockaddr_in serverAddress;
	int connectionSocket;

	connectionSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(connectionSocket < 0) {
		printf("[Server] Error opening socket\n");
		return -1;
	}

	memset((char*) &serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNumber);

	if(bind(connectionSocket, (struct sockaddr*) &serverAddress,
						sizeof(struct sockaddr)) < 0) {
		printf("[Server] Error on binding\n");
		return -1;
	}

	listen(connectionSocket, numberOfClients);

	return connectionSocket;
}

/* Este realizata conectarea unui client la server pe canalul tcp, fiind
   returnat socket-ul tcp pe care vor fi trimise date catre server       */
int connectToTcpServer(char* serverIp, int portNumber) {
	struct sockaddr_in serverAddress;
	int tcpSocket;

	tcpSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(tcpSocket < 0) {
		printf("[Client] Error opening socket\n");
		return -1;
	}

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	inet_aton(serverIp, &serverAddress.sin_addr);

	if(connect(tcpSocket, (struct sockaddr*) &serverAddress,
		sizeof(serverAddress)) < 0) {

		printf("[Client] Error connecting to server\n");
		return -1;
	}

	return tcpSocket;
}

/* Este creata adresa de tip server udp folosita pentru comunicatie */
struct sockaddr_in createUdpServerAddress(char* serverIp, int portNumber) {
	struct sockaddr_in serverAddress;

	memset((char*) &serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	inet_aton(serverIp, &serverAddress.sin_addr);

	return serverAddress;
}

/* Este creat socket-ul udp folosit de catre client */
int createClientUdpSocket() {
	int udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(udpSocket == -1) {
		printf("[Client] Error opening socket\n");
		return -1;
	}

	return udpSocket;
}

/* Este creat socket-ul udp folosit de catre server */
int createServerUdpSocket() {
	int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(udpSocket == -1) {
		printf("[Server] Error opening socket\n");
		return -1;
	}

	return udpSocket;
}

/* Este initializata adresa server-ului de tip udp pentru a accepta date */
struct sockaddr_in initialiseUdpServer(int portNumber, int serverSocket) {
	struct sockaddr_in serverAddress;

	memset((char*) &serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(serverSocket, (struct sockaddr*) &serverAddress,
	        sizeof(serverAddress)) == -1) {

		printf("[Server] Error on binding\n");
	}

	return serverAddress;
}

/* Este acceptata cererea de conectare a unui client la server pe canalul tcp,
   fiind returnat socket-ul folosit pentru comunicatia cu acesta          */
int acceptClient(int connectionSocket) {
	struct sockaddr_in clientAddress;
	int clientAddressLength;
	int clientSocket;

	clientAddressLength = sizeof(struct sockaddr);
	if((clientSocket = accept(connectionSocket,
		                      (struct sockaddr*) &clientAddress,
							  &clientAddressLength)) == -1) {

		printf("[Server] Error accepting client\n");
		return -1;
	}

	return clientSocket;
}

#endif