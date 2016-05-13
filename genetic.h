/*
 * genetic.h
 *
 *  Created on: 4 mai 2016
 *      Author: aroun
 */

#ifndef GENETIC_H_
#define GENETIC_H_

#include "globals.h"

using namespace std;

# define POPSIZE 50
# define MAXGENS 1000
# define NVARS 3
# define PXOVER 0.8
# define PMUTATION 0.15

struct genotype
{
  double gene[NVARS];
  double fitness;
  double upper[NVARS];
  double lower[NVARS];
  double rfitness;
  double cfitness;
};

extern genotype population[POPSIZE+1];
extern genotype newpopulation[POPSIZE+1]; 

extern vector<char*> list_action;

void genetic(string);

void crossover ( int &seed );
void elitist ( );
void evaluate ( );
int i4_uniform_ab ( int a, int b, int &seed );
void initialize ( string filename, int &seed );
void keep_the_best ( );
void mutate ( int &seed );
double r8_uniform_ab ( double a, double b, int &seed );
void report ( int generation );
void selector ( int &seed );
void timestamp ( );
void Xover ( int one, int two, int &seed );


#endif /* GENETIC_H_ */
