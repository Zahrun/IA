/*
 * networking.h
 *
 *  Created on: 4 mai 2016
 *      Author: aroun
 */

#ifndef NETWORKING_H_
#define NETWORKING_H_

#include "globals.h"

using namespace std;



void connexion(char * host, int port);
void readDescription(char * buffer);
int recognizeWord(string word);
void sendAction(string action, int length);
void readMessage(char * buffer);


#endif /* NETWORKING_H_ */
