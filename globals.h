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

extern sem_t sem_attente_decision_IA;
extern sem_t sem_attente_get_action;

void error(const char *msg);

void init_semaphores();

#endif /* GLOBALS_H_ */
