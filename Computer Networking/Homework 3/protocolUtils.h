/* Fetoiu Catalin-Emil 324CB */

#ifndef protocolUtils
#define protocolUtils
#include <iostream>

using namespace std;

#define httpPort 80

string httpBeginning("http://");

/* Este creat un socket pe care server-ul sa astepte conexiuni de tip tcp de
   la clienti, numarul maxim de clienti posibili la un moment dat fiind
   dat ca parametru                                                     */
int initialiseServer(int portNumber, int numberOfClients) {
    struct sockaddr_in serverAddress;
	int connectionSocket;

	connectionSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(connectionSocket < 0) {
		printf("[Proxy-Server] Error opening socket\n");
		return -1;
	}

	memset((char*) &serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNumber);

	if(bind(connectionSocket, (struct sockaddr*) &serverAddress,
						sizeof(struct sockaddr)) < 0) {
		printf("[Proxy-Server] Error on binding\n");
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
		printf("[Proxy-Client] Error opening socket\n");
		return -1;
	}

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	inet_aton(serverIp, &serverAddress.sin_addr);

	if(connect(tcpSocket, (struct sockaddr*) &serverAddress,
		sizeof(serverAddress)) < 0) {

		printf("[Proxy-Client] Error connecting to server\n");
		return -1;
	}

	return tcpSocket;
}

/* Este acceptata cererea de conectare a unui client la server pe canalul tcp,
   fiind returnat socket-ul folosit pentru comunicatia cu acesta          */
int acceptClient(int connectionSocket) {
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength;
	int clientSocket;

	clientAddressLength = sizeof(struct sockaddr);
	if((clientSocket = accept(connectionSocket,
		                      (struct sockaddr*) &clientAddress,
							  &clientAddressLength)) == -1) {

		printf("[Proxy-Server] Error accepting client\n");
		return -1;
	}

	return clientSocket;
}

/* Extrage numele domeniului si numarul portului din comanda aflata pe prima
   linie a unui request http. Daca portul nu este specificat, acesta este
   setat egal cu 80. Parsarea este realizata prin eliminarea comenzii si a
   secventei "http://" (daca aceasta este prezenta). Apoi sunt extrase domeniul
   si portul (prin detectarea inceputului caii catre fisier)               */
pair<string, int> parseHttpRequest(string httpRequest) {
    int index = httpRequest.find(" ");
    string url = httpRequest.substr(index + 1);

    if((int)url.find(httpBeginning) != -1) {
        url = url.substr(httpBeginning.length());
    }

    index = url.find("/");
    if(index == 0) {
        return make_pair("", -1);
    }

    string domain = url.substr(0, index);
    index = domain.find(":");

    if(index == -1) {
        return make_pair(domain, httpPort);
    } else {
        string domainName = domain.substr(0, index);
        string port = domain.substr(index + 1);

        int portNumber = atoi(port.c_str());
        return make_pair(domainName, portNumber);
    }
}

/* Extrage numele domeniului si numarul portului din header-ul "Host" al unui
   request, acestea fiind separate de ":". De asemenea, portul implicit este
   80                                                                       */
pair<string, int> parseHttpHostHeader(string hostField) {
    int index = hostField.find(" ");
    string domain = hostField.substr(index + 1);

    index = domain.find(":");

    if(index == -1) {
        return make_pair(domain, httpPort);
    } else {
        string domainName = domain.substr(0, index);
        string port = domain.substr(index + 1);

        int portNumber = atoi(port.c_str());
        return make_pair(domainName, portNumber);
    }
}

/* Returneaza adresa IP a unui domeniu, folosind protocolul dns */
char* getIpAddress(string hostName) {
    struct hostent* host = gethostbyname(hostName.c_str());
    if(host == NULL) {
    	printf("[Http-Proxy] Invalid domain name\n");
    	return NULL;
    }

    struct in_addr **adr = (struct in_addr**) host->h_addr_list;

    return inet_ntoa(*adr[0]);
}

#endif