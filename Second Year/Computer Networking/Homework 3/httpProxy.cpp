/* Fetoiu Catalin-Emil 324CB */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "protocolUtils.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

#define bufferSize 1500
#define numberOfClients 100
#define requestNumber 1000
#define postCommand -2
#define newRequest -1

/* Contine, pentru fiecare request http, fisierul de cache corespunzator */
map<string, int> requestCacheFile;


/* Extrage identificatorul unui request http. In cazul in care acesta contine
   o singura linie, ea este returnata. In caz contrar sunt returnate primele
   2 linii. Motivul pentru care fiecare request are asociat un identificator
   unic este ca tipul comenzii impreuna cu url-ul sunt continute in primele
   2 linii                                                                  */
string extractRequestIdentifier(string httpRequest) {
    int index = httpRequest.find("\n");
    if(index == (int)httpRequest.length() - 1) {
        return httpRequest;
    }

    index = httpRequest.find("\n", index+1);
    return httpRequest.substr(0, index+1);
}

/* Adauga identificatorul unui request in mapa ce contine fisierul de cache
   asociat fiecarui request                                                 */
void addRequest(string requestIdentifier) {
    int index = requestCacheFile.size();

    requestCacheFile.insert(make_pair(requestIdentifier, index));
}

/* Extrage comanda dintr-un request http */
string extractRequestCommand(string httpRequest) {
    int index = httpRequest.find(" ");
    return httpRequest.substr(0, index);
}

/* Verifica daca un request http contine o restrictie ce indica faptul ca
   raspunsul primit nu trebuie salvat in cache                            */
bool cacheRestriction(string httpRequest) {
    if((int)httpRequest.find("no-cache") != -1) {
        return true;
    } else if((int)httpRequest.find("private") != -1) {
        return true;
    }
    return false;
}

/* Verifica daca raspunsul pentru un request a fost salvat in cache, caz in
   care este returnat indicele fisierului corespunzator. Altfel, se verifica
   daca tipul comenzii primite este "POST" (pentru ca raspunsul sa nu fie
   salvat), in caz contrar fiind returnat codul newRequest, fiind nevoie
   de creearea unui fisier de cache                                        */
int getCacheIndex(string httpRequest) {
    string command = extractRequestCommand(httpRequest);
    if(command == "POST") {
        return postCommand;
    }

    string identifier = extractRequestIdentifier(httpRequest);
    if(requestCacheFile.find(identifier) == requestCacheFile.end()) {
        addRequest(identifier);
        return newRequest;
    }

    map<string, int>::iterator it = requestCacheFile.find(identifier);
    return it->second;
}

/* Returneaza numele fisierului de cache avand indicele primit ca parametru */
char* getCacheFileName(int cacheFileIndex) {
    char* fileName = (char*)calloc(requestNumber, sizeof(char));
    sprintf(fileName, "cache_%d", cacheFileIndex);

    return fileName;
}

/* Realizeaza o conexiune tcp catre server-ul continut in url-ul asociat unui
   request http. Dupa parsarea url-ului, este determinata adresa IP a
   server-ului folosind functia getIpAdress, fiind apoi returnat socket-ul
   creat de functia connectToTcpServer                                      */
int connectToWebServer(string httpRequest) {
    int index = httpRequest.find("\n");
    string requestCopy = httpRequest;
    string hostHeader = "";
    
    if(index != -1) {
        httpRequest = httpRequest.substr(0, index + 1);
    }

    if(httpRequest.length() < requestCopy.length()) {
        requestCopy = requestCopy.substr(index + 1);
        index = httpRequest.find("\n");
        hostHeader = requestCopy.substr(0, index + 1);
    }

    pair<string, int> domain = parseHttpRequest(httpRequest);
    if(domain.second == -1) {
        domain = parseHttpHostHeader(hostHeader);
    }

    char* serverIp = getIpAddress(domain.first);
    if(serverIp == NULL) {
        return -1;
    }
    int portNumber = domain.second;

    return connectToTcpServer(serverIp, portNumber);
}


int main(int argc, char* argv[]) {

    /* Este creat un socket pe care proxy-ul sa astepte conexiuni de tip tcp
       de la clienti                                                         */
    int connectionSocket = initialiseServer(atoi(argv[1]), numberOfClients);
    if(connectionSocket == -1) {
        return -1;
    }

    /* Buffer-ele folosite pentru primirea de request-uri, respectiv raspunsuri
       de tip http                                                           */
    char* httpRequest = (char*)calloc(bufferSize, sizeof(char));
    char* httpResponse = (char*)calloc(bufferSize, sizeof(char));

    /* Multimea stream-urilor pe care proxy-ul primeste, respectiv
       trimite date                                               */
    fd_set inputStreams;
    fd_set inputStreamsCopy;

    /* Indicele maxim al tuturor stream-urilor proxy-ului */
    int maximumStreamIndex = max(0, connectionSocket);

    FD_ZERO(&inputStreams);
    FD_ZERO(&inputStreamsCopy);

    /* Stream-ul initial este socket-ul folosit pentru a astepta conexiuni */
    FD_SET(connectionSocket, &inputStreams);

    /* Cat timp proxy-ul este activ */
    while(true) { 
        inputStreamsCopy = inputStreams;

        /* Este folosit apelul select pentru a determina stream-urile de pe
           care pot fi citite date                                         */
        if(select(maximumStreamIndex + 1, &inputStreamsCopy, NULL, NULL,
                  NULL) == -1) {
            printf("[Http-Proxy] Error on select\n");
        }

        for(int currentSocket = 0; currentSocket <= maximumStreamIndex;
            currentSocket++) {

            if(FD_ISSET(currentSocket, &inputStreamsCopy)) {

                /* Daca este primita o cerere de conexiune, socket-ul
                   corespunzator noului client este adaugat in multimea de
                   stream-uri                                              */
                if(currentSocket == connectionSocket) {
                    int clientSocket = acceptClient(connectionSocket);
                    if(clientSocket == -1) {
                        return -1;
                    }

                    FD_SET(clientSocket, &inputStreams);
                    if(clientSocket > maximumStreamIndex) {
                        maximumStreamIndex = clientSocket;
                        break;
                    }

                /* Daca a fost primit un request http de la un client */
                } else {
                    int clientSocket = currentSocket;

                    memset(httpRequest, 0, bufferSize);

                    int size = recv(clientSocket, httpRequest, bufferSize, 0);
                    if(size < 0) {
                        printf("[Http-Proxy] Error in recv\n");
                        return -1;
                    }
                    
                    int cacheIndex = getCacheIndex(httpRequest);

                    /* Se verifica daca exista o restrictie asupra salvarii
                       in cache a raspunsului asociat request-ului          */
                    if(cacheIndex == postCommand ||
                        cacheRestriction(httpRequest)) {

                        /* Este realizata conexiunea catre server-ul indicat
                           de url                                            */
                        int webServerSocket = connectToWebServer(httpRequest);
                        if(webServerSocket == -1) {
                            return -1;
                        }
                        
                        /* Request-ul este trimis mai departe catre server-ul
                           web, raspunsul primit fiind trimis inapoi catre
                           client, fiind apoi inchisii socket-ii asociati
                           client-ului si server-ului web                   */
                        int res = send(webServerSocket, httpRequest, size, 0);
                        if(res < 0) {
                            printf("[Http-Proxy] Error in send\n");
                            return -1;
                        }

                        while(true) {
                            memset(httpResponse, 0, bufferSize);
                            int size = recv(webServerSocket, httpResponse,
                                            bufferSize, 0);
                            if(size < 0) {
                                printf("[Http-Proxy] Error in recv\n");
                                return -1;
                            }

                            if(size <= 0) {
                                break;
                            }

                            int res = send(clientSocket, httpResponse, size, 0);
                            if(res < 0) {
                                printf("[Http-Proxy] Error in send\n");
                                return -1;
                            }
                        }
                        
                        close(webServerSocket);

                    /* Daca este primit un request ce nu este salvat in cache,
                       raspunsul este trimis inapoi catre client, fiind totodata
                       salvat in fisierul de cache corespunzator             */
                    } else if(cacheIndex == newRequest) {
                        cacheIndex = getCacheIndex(httpRequest);
                        char* fileName = getCacheFileName(cacheIndex);
                        free(fileName);

                        int cache = open(fileName, O_WRONLY | O_CREAT, 0640);

                        int webServerSocket = connectToWebServer(httpRequest);
                        if(webServerSocket == -1) {
                            return -1;
                        }
                            
                        int res = send(webServerSocket, httpRequest, size, 0);
                        if(res < 0) {
                            printf("[Http-Proxy] Error in send\n");
                            return -1;
                        }

                        while(true) {
                            memset(httpResponse, 0, bufferSize);
                            int size = recv(webServerSocket, httpResponse,
                                            bufferSize, 0);
                            if(size < 0) {
                                printf("[Http-Proxy] Error in recv\n");
                                return -1;
                            }

                            write(cache, httpResponse, size);

                            if(size <= 0) {
                                break;
                            }

                            int res = send(clientSocket, httpResponse, size, 0);
                            if(res < 0) {
                                printf("[Http-Proxy] Error in send\n");
                                return -1;
                            }
                        }
                            
                        close(webServerSocket);
                        close(cache);

                    /* Daca raspunsul asociat request-ului este salvat in cache,
                       acesta este trimis catre client fara a mai fi deschisa
                       o conexiune cu server-ul web                          */
                    } else {
                        char* fileName = getCacheFileName(cacheIndex);
                        int cache = open(fileName, O_RDONLY);
                        free(fileName);

                        while(true) {
                            memset(httpResponse, 0, bufferSize);
                            int size = read(cache, httpResponse, bufferSize);

                            if(size == 0) {
                                break;
                            }

                            int res = send(clientSocket, httpResponse, size, 0);
                            if(res < 0) {
                                printf("[Http-Proxy] Error in send\n");
                                return -1;
                            }
                            
                            if(size < bufferSize) {
                                break;
                            }
                        }

                        close(cache);
                    }

                    /* Socket-ul corespunzator unui client este inchis dupa
                       ce raspunsul primit a fost trimis inapoi catre acesta */
                    FD_CLR(clientSocket, &inputStreams);
                    close(clientSocket);
                }
            }
        }
    }

    free(httpRequest);
    free(httpResponse);

    return 0;
}