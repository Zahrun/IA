/*
 * networking.cpp
 *
 *  Created on: 4 mai 2016
 *      Author: aroun
 */

#include "networking.h"

#define BUFFER_SIZE 2048
#define NUMBER_OF_WORDS 3

const string WORDS[NUMBER_OF_WORDS] = {"width","get_action","set_visible"};

int sockfd;

void sendAction(){
	if (sem_post(&sem_attente_get_action)){
		error("Erreur oppération V sur sem");
	}
	cout << "Ntwrk : en attente de l'action à envoyer..." << endl;
	if (sem_wait(&sem_attente_decision_IA)){
		error("Erreur oppération P sur sem");
	}

	write(sockfd, "end_turn\n", 9);
}

int recognizeWord(string word){
	for (int i = 0; i<NUMBER_OF_WORDS; i++){
		if (word == WORDS[i]){
			return i;
		}
	}
	return 0;
}

void readDescription(char * buffer){
	cout << "*** Parsing message :" << endl;
	istringstream issBuf(buffer), issLine;
	string line, word;
	while (getline(issBuf,line) > 0){
		issLine.str(line);
		while(issLine >> word) {
			cout << word << " ";
		}
		cout << endl;
		issLine.clear();
	}
	issBuf.clear();
	cout << "*** Message parsed" << endl;
}

void readMessage(char * buffer){
	cout << "*** Parsing first word of message" << endl;
	istringstream issBuf(buffer);
	string firstWord;
	if (!(issBuf >> firstWord)){
		error("buffer vide !!!");
	}
	switch (recognizeWord(firstWord)){
	case 0 : // width
		cout << "premier mot : width" << endl;
		readDescription(buffer);
		break;
	case 1 : // get_action
		cout << "premier mot : get_action" << endl;
		sendAction();
		break;
	default :
		cout << "message non reconnu : \"" << firstWord << "\" !!!" << endl;
	}
	cout << "*** First word of message parsed" << endl;

}

void connexion(char * host, int port)
{
	// synchro initiale
	cout << "Synchronisation des threads..." << endl;
	if (sem_wait(&sem_attente_decision_IA)){
		error("Erreur oppération P sur sem");
	}
	cout << "Threads synchronisés" << endl;


	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[BUFFER_SIZE];

	cout << "Tentative de connexion a " << host << ":" << port << endl;

	// create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	// create host
	server = gethostbyname(host);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	// create server address
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(port);
	// connexion au serveur
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR connecting");
	cout << "Connexion au serveur réussie !!!" << endl;
	// lecture messages
	int n;

	while (1){
		n =read(sockfd,buffer,BUFFER_SIZE - 1);
		if (n < 0)
			error("ERROR reading from socket");
		if (n == 0){
			cout << "fin de la lecture: lu = 0" << endl;
			break;
		}
		readMessage(buffer);
		cout << "reçu :\"" << buffer << "\"" << endl;
		bzero(buffer, 256);
	}
	close(sockfd);
}

