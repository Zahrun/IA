/*
 * genetic.h
 *
 *  Created on: 4 mai 2016
 *      Author: aroun
 */

#ifndef GENETIC_H_
#define GENETIC_H_

#include "globals.h"
#include "data.h"
#include "networking.h"

using namespace std;

# define POPSIZE 20
# define MAXGENS 500
//# define NVARS 3
# define PXOVER 0.8
# define PMUTATION 0.15

# define NBTOURS 3

# define PTS_ATTAQUE_VILLE_ENNEMIE 20000
# define PTS_ATTAQUE_VILLE_NEUTRE 1000
# define PTS_DECOUVERTE_TERRAIN 20
# define PTS_CANNIBAL -5
# define PTS_ATTAQUE_ENNEMI 100
# define PTS_VISION_TERRAIN 1000
# define PTS_UNITE_DANS_LEAU -10

# define LBOUND 0
# define UBOUND 6 // mouvement possibles numérotés de 0 à 5 ( D,Z,A,Q,X,C) ( -> le 7ème mouvement étant de ne pas bouger la pièce ! Qui peut parfois être le meilleur coup à jouer)

struct genotype
{
	vector<int> gene; // on enregistre tous le sdéplacements de chaque unité
	// utilisation d'un vecteur car taille varibale !
	// soit la liste des k déplacements de chaque unité pour les n prochains tours
	double fitness;
	double rfitness;
	double cfitness;
};

extern case_t temp_map[LONGUEURE_MAP][LARGEUR_MAP];

extern genotype population[POPSIZE+1];
extern genotype newpopulation[POPSIZE+1]; 

extern vector<string> list_action;

extern bool new_city;
extern bool fin_du_tour;
extern int turn;

void genetic(string);

void crossover ( int &seed );
void elitist ( );
void evaluate ( );
double get_fitness(double* gene);
int i4_uniform_ab ( int a, int b, int &seed );
void initialize ( int &seed );
void keep_the_best ( );
void mutate ( int &seed );
double r8_uniform_ab ( double a, double b, int &seed );
void report ( int generation );
void selector ( int &seed );
void timestamp ( );
void Xover ( int one, int two, int &seed );
void display_list_action() ;
void display_list_ally_city() ;

#endif /* GENETIC_H_ */
