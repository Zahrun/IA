/*
 * genetic.cpp
 *
 *  Created on: 4 mai 2016
 *      Author: aroun
 */

#include "genetic.h"
#include "data.h"
#include <sstream>
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <iomanip>
# include <cmath>
# include <ctime>
# include <cstring>

# define POPSIZE 50
# define MAXGENS 1000
# define NVARS 3  //nb de tours * nombres de var par tour
# define PXOVER 0.8
# define PMUTATION 0.15

genotype population[POPSIZE+1];
genotype newpopulation[POPSIZE+1]; 

case_t temp_map[LONGUEURE_MAP][LARGEUR_MAP];
vector<unit> temp_list_ally_unit;
vector<city> temp_list_ally_city;

int gene_length = 0;

int turn = 0;
vector<char*> list_action;
ostringstream oss;
char* temp_action;
char temp_char[50];
string temp_string;
int seed;

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

		timestamp ( );		

		turn++;
		
		gene_length = list_ally_unit.size()*2; //depend de la forme du gene

		if (turn > 10) {
			/*oss << "set_city_production " << list_ally_city.at(0).id << " " << 0 << "\n"; // 0 = ARMY
			temp_char = oss.str().c_str();
			list_action.push_back(temp_char);
			oss.clear();*/
			/*cout << "salut" << endl;
			temp_string = "set_city_production " + to_string(list_ally_city.at(0).id) + " " + to_string(0) + "\n";
			temp_action = (char*)temp_string.c_str();
			//temp_char = temp_action.c_str();
			list_action.push_back(temp_action);*/
		} else {
			/*if (list_unit.size() >= 1) {
				for (int i = 0; list_unit.size(); i++) {
					oss << "move " << list_unit.at(i).id << " " << 0 << "\n";
					temp_char = oss.str().c_str();
					list_action.push_back(temp_char);
					oss.clear();
				}
			}*/
			//temp_action = "end_turn\n";
			temp_string = "set_city_production " + to_string(list_ally_city.at(0).id) + " " + to_string(0) + "\n";
			char *cstr = new char[temp_string.length() + 1];
			strcpy(cstr, temp_string.c_str());
			list_action.push_back(cstr);
			delete [] cstr;
		}

		seed = 123456789;
	//initialize(seed);
  	/*evaluate ( );

  	keep_the_best ( );

  	for ( generation = 0; generation < MAXGENS; generation++ )
  	{
    	selector ( seed );
    	crossover ( seed );
    	mutate ( seed );
    	report ( generation );
    	evaluate ( );
    	elitist ( );
  	}*/

		usleep(500000); // simulation de décision d'action

		timestamp ( );

		// prévenir que la décision est prise
		if (sem_post(&sem_attente_decision_IA)){
			error("Erreur opération V sur sem");
		}
	}
}


void crossover ( int &seed ) {
  const double a = 0.0;
  const double b = 1.0;
  int mem;
  int one;
  int first = 0;
  double x;

  for ( mem = 0; mem < POPSIZE; ++mem ) {
    x = r8_uniform_ab ( a, b, seed );

    if ( x < PXOVER ) {
      ++first;

      if ( first % 2 == 0 ) {
        Xover ( one, mem, seed );
      } else {
        one = mem;
      }
    }
  }
  return;
}

//****************************************************************************

void elitist ( )
{
  int i;
  double best;
  int best_mem;
  double worst;
  int worst_mem;

  best = population[0].fitness;
  worst = population[0].fitness;

  for ( i = 0; i < POPSIZE - 1; ++i ) {
    if ( population[i+1].fitness < population[i].fitness ) {
      if ( best <= population[i].fitness ) {
        best = population[i].fitness;
        best_mem = i;
      }

      if ( population[i+1].fitness <= worst ) {
        worst = population[i+1].fitness;
        worst_mem = i + 1;
      }

    } else {

      if ( population[i].fitness <= worst ) {
        worst = population[i].fitness;
        worst_mem = i;
      }

      if ( best <= population[i+1].fitness ) {
        best = population[i+1].fitness;
        best_mem = i + 1;
      }
    }
  }
//  If the best individual from the new population is better than 
//  the best individual from the previous population, then 
//  copy the best from the new population; else replace the 
//  worst individual from the current population with the 
//  best one from the previous generation                     

  if ( population[POPSIZE].fitness <= best ) {
    for ( i = 0; i < NVARS; i++ ) {
      population[POPSIZE].gene[i] = population[best_mem].gene[i];
    }
    population[POPSIZE].fitness = population[best_mem].fitness;
  } else {
    for ( i = 0; i < NVARS; i++ ) {
      population[worst_mem].gene[i] = population[POPSIZE].gene[i];
    }
    population[worst_mem].fitness = population[POPSIZE].fitness;
  } 

  return;
}

//****************************************************************************

void evaluate ( ) {
  int member;
  int i;
  double x[NVARS+1];

	copy_map();

  for ( member = 0; member < POPSIZE; member++ ) {
    for ( i = 0; i < NVARS; i++ ) {
      x[i+1] = population[member].gene[i];
    } 
    population[member].fitness = get_fitness(population[member].gene); ;  //TODO : 
  }
  return;
}



//****************************************************************************
// TODO
double get_fitness(double* gene) {

	return 1;
}

//****************************************************************************

int i4_uniform_ab ( int a, int b, int &seed ) {
  int c;
  const int i4_huge = 2147483647;
  int k;
  float r;
  int value;

  if ( seed == 0 ) {
    cerr << "\n";
    cerr << "I4_UNIFORM_AB - Fatal error!\n";
    cerr << "  Input value of SEED = 0.\n";
    exit ( 1 );
  }

//  Guarantee A <= B.
  if ( b < a ) {
    c = a;
    a = b;
    b = c;
  }

  k = seed / 127773;

  seed = 16807 * ( seed - k * 127773 ) - k * 2836;

  if ( seed < 0 ) {
    seed = seed + i4_huge;
  }

  r = ( float ) ( seed ) * 4.656612875E-10;

//  Scale R to lie between A-0.5 and B+0.5.
  r = ( 1.0 - r ) * ( ( float ) a - 0.5 ) 
    +         r   * ( ( float ) b + 0.5 );

//  Use rounding to convert R to an integer between A and B.
  value = round ( r );

//  Guarantee A <= VALUE <= B.
  if ( value < a ) {
    value = a;
  }
  if ( b < value ) {
    value = b;
  }

  return value;
}

//****************************************************************************80

void initialize ( int &seed ) {
  int i;
  int j;
  double lbound;
  double ubound;


//  Initialize variables within the bounds 
  for ( i = 0; i < NVARS; i++ ) {
    input >> lbound >> ubound;

    for ( j = 0; j < POPSIZE; j++ ) {
      population[j].fitness = 0;
      population[j].rfitness = 0;
      population[j].cfitness = 0;
      population[j].lower[i] = lbound;
      population[j].upper[i]= ubound;
      population[j].gene[i] = r8_uniform_ab ( lbound, ubound, seed ); // TODO
    }
  }

  input.close ( );

  return;
}

//****************************************************************************80

void keep_the_best ( ) {
  int cur_best;
  int mem;
  int i;

  cur_best = 0;

  for ( mem = 0; mem < POPSIZE; mem++ ) {
    if ( population[POPSIZE].fitness < population[mem].fitness ) {
      cur_best = mem;
      population[POPSIZE].fitness = population[mem].fitness;
    }
  }

//  Once the best member in the population is found, copy the genes.
  for ( i = 0; i < NVARS; i++ ) {
    population[POPSIZE].gene[i] = population[cur_best].gene[i];
  }

  return;
}

//****************************************************************************80

void mutate ( int &seed ) {
  const double a = 0.0;
  const double b = 1.0;
  int i;
  int j;
  double lbound;
  double ubound;
  double x;

  for ( i = 0; i < POPSIZE; i++ ) {
    for ( j = 0; j < NVARS; j++ ) {
      x = r8_uniform_ab ( a, b, seed );
      if ( x < PMUTATION ) {
        lbound = population[i].lower[j];
        ubound = population[i].upper[j];  
        population[i].gene[j] = r8_uniform_ab ( lbound, ubound, seed );
      }
    }
  }

  return;
}

//****************************************************************************80

double r8_uniform_ab ( double a, double b, int &seed ) {
  int i4_huge = 2147483647;
  int k;
  double value;

  if ( seed == 0 ) {
    cerr << "\n";
    cerr << "R8_UNIFORM_AB - Fatal error!\n";
    cerr << "  Input value of SEED = 0.\n";
    exit ( 1 );
  }

  k = seed / 127773;

  seed = 16807 * ( seed - k * 127773 ) - k * 2836;

  if ( seed < 0 ) {
    seed = seed + i4_huge;
  }

  value = ( double ) ( seed ) * 4.656612875E-10;

  value = a + ( b - a ) * value;

  return value;
}

//****************************************************************************80

void report ( int generation ) {
  double avg;
  double best_val;
  int i;
  double square_sum;
  double stddev;
  double sum;
  double sum_square;

  if ( generation == 0 ) {
    cout << "\n";
    cout << "  Generation       Best            Average       Standard \n";
    cout << "  number           value           fitness       deviation \n";
    cout << "\n";
  }

  sum = 0.0;
  sum_square = 0.0;

  for ( i = 0; i < POPSIZE; i++ ) {
    sum = sum + population[i].fitness;
    sum_square = sum_square + population[i].fitness * population[i].fitness;
  }

  avg = sum / ( double ) POPSIZE;
  square_sum = avg * avg * POPSIZE;
  stddev = sqrt ( ( sum_square - square_sum ) / ( POPSIZE - 1 ) );
  best_val = population[POPSIZE].fitness;

  cout << "  " << setw(8) << generation 
       << "  " << setw(14) << best_val 
       << "  " << setw(14) << avg 
       << "  " << setw(14) << stddev << "\n";

  return;
}

//****************************************************************************80

void selector ( int &seed ) {
  const double a = 0.0;
  const double b = 1.0;
  int i;
  int j;
  int mem;
  double p;
  double sum;

//  Find the total fitness of the population.
  sum = 0.0;
  for ( mem = 0; mem < POPSIZE; mem++ ) {
    sum = sum + population[mem].fitness;
  }

//  Calculate the relative fitness of each member.
  for ( mem = 0; mem < POPSIZE; mem++ ) {
    population[mem].rfitness = population[mem].fitness / sum;
  }

//  Calculate the cumulative fitness.
  population[0].cfitness = population[0].rfitness;
  for ( mem = 1; mem < POPSIZE; mem++ ) {
    population[mem].cfitness = population[mem-1].cfitness +       
      population[mem].rfitness;
  }

//  Select survivors using cumulative fitness. 
  for ( i = 0; i < POPSIZE; i++ ) { 
    p = r8_uniform_ab ( a, b, seed );
    if ( p < population[0].cfitness ) {
      newpopulation[i] = population[0];      
    } else {
      for ( j = 0; j < POPSIZE; j++ ) { 
        if ( population[j].cfitness <= p && p < population[j+1].cfitness ) {
          newpopulation[i] = population[j+1];
        }
      }
    }
  }

//  Overwrite the old population with the new one.
  for ( i = 0; i < POPSIZE; i++ ) {
    population[i] = newpopulation[i]; 
  }

  return;     
}

//****************************************************************************80

void timestamp ( ) {
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t len;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}

//****************************************************************************80

void Xover ( int one, int two, int &seed ) {
  int i;
  int point;
  double t;

//  Select the crossover point.
  point = i4_uniform_ab ( 0, NVARS - 1, seed );

//  Swap genes in positions 0 through POINT-1.
  for ( i = 0; i < point; i++ ) {
    t                       = population[one].gene[i];
    population[one].gene[i] = population[two].gene[i];
    population[two].gene[i] = t;
  }

  return;
}
















