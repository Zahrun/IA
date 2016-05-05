/*
 * networking.h
 *
 *  Created on: 4 mai 2016
 *      Author: aroun
 */

#ifndef NETWORKING_H_
#define NETWORKING_H_

#include <iostream>
#include <sys/socket.h> //socket
#include <netinet/in.h>
#include <strings.h>	//bzero, bcopy
#include <unistd.h>		//read, write
#include <netdb.h>		//gethostbyname

#include "globals.h"

using namespace std;

void connexion(char * host, int port);



#endif /* NETWORKING_H_ */
