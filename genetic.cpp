/*
 * genetic.cpp
 *
 *  Created on: 4 mai 2016
 *      Author: aroun
 */

#include "genetic.h"
#include "data.h"

int turn = 0;
vector<const char*> list_action;

void genetic(string msg){
	// synchronisation initiale des threads
	if (sem_post(&sem_attente_decision_IA)){
		error("Erreur opération V sur sem");  //opération ca prend pas 2 p
	}

	while(1){
		cout << "IA : attente de get_action..." << endl;
		// en attente de demande d'action
		if (sem_wait(&sem_attente_get_action)){
			error("Erreur opération P sur sem");
		}
		cout << "IA : prise de décision..." << endl;

		turn++;
		//ligne de code
		if (turn == 1) {
				list_action.push_back("set_city_production 0 )
		}

		usleep(500000); // simulation de décision d'action

		// prévenir que la décision est prise
		if (sem_post(&sem_attente_decision_IA)){
			error("Erreur opération V sur sem");
		}
	}
}

