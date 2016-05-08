/*
 * genetic.cpp
 *
 *  Created on: 4 mai 2016
 *      Author: aroun
 */

#include "genetic.h"

void genetic(string msg){
	// synchronisation initiale des threads
	if (sem_post(&sem_attente_decision_IA)){
		error("Erreur oppération V sur sem");
	}

	while(1){
		cout << "IA : attente de get_action..." << endl;
		// en attente de demande d'action
		if (sem_wait(&sem_attente_get_action)){
			error("Erreur oppération P sur sem");
		}
		cout << "IA : prise de décision..." << endl;
		usleep(500000); // simulation de décision d'action
		// prévenir que la décision est prise
		if (sem_post(&sem_attente_decision_IA)){
			error("Erreur oppération V sur sem");
		}
	}
}

