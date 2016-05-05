/*
 * networking.cpp
 *
 *  Created on: 4 mai 2016
 *      Author: aroun
 */

#include "networking.h"

void connexion(char * host, int port)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];

	cout << "Tentative de connection a " << host << ":" << port << endl;

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
	while(1){
		if(read(sockfd,buffer,255) < 0)
			error("ERROR reading from socket");
		printf("%s\n",buffer);
	}
	close(sockfd);
}

