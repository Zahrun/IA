/*
 * globals.h
 *
 *  Created on: 5 mai 2016
 *      Author: aroun
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <iostream>
#include <sys/socket.h> //socket
#include <netinet/in.h>
#include <strings.h>	//bzero, bcopy
#include <sstream>		//string streams
#include <unistd.h>		//read, write
#include <netdb.h>		//gethostbyname
#include <semaphore.h>	//sémaphores
#include <vector>

using namespace std;

#define LONGUEURE_MAP 20
#define LARGEUR_MAP 20
#define NUMBER_OF_WORDS 15

extern sem_t sem_attente_decision_IA;
extern sem_t sem_attente_get_action;

//bool my_turn = false;


void error(const char *msg);

void init_semaphores();

#endif /* GLOBALS_H_ */
