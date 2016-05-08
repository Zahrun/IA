/*
 * globals.cpp
 *
 *  Created on: 5 mai 2016
 *      Author: aroun
 */

#include "globals.h"

sem_t sem_attente_decision_IA;
sem_t sem_attente_get_action;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void init_semaphores(){
    sem_init(&sem_attente_decision_IA, 0, 0);
    sem_init(&sem_attente_get_action, 0, 0);
}
