/*
 * genetic.cpp
 *
 *  Created on: 4 mai 2016
 *      Author: aroun
 */

#include "genetic.h"
#include "data.h"
#include <math.h> 
#include <sstream>
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <iomanip>
# include <cmath>
# include <ctime>
# include <cstring>





int nvars; // normalement c'est fixé sur un algo gén (NVARS), la on va le faire en dynamique vu qu'il dépend de notre nombre d'unité actuel

genotype population[POPSIZE+1];
genotype newpopulation[POPSIZE+1]; 

case_t temp_map[LONGUEURE_MAP][LARGEUR_MAP];
vector<unit> temp_list_ally_unit;
vector<city> temp_list_ally_city;

bool new_city = true;
bool fin_du_tour = false;

int turn = 0;
vector<string> list_action;
ostringstream oss;
char* temp_action;
char temp_char[50];
string temp_string;
int seed;

void genetic(string msg){

	int generation ;	

	// synchronisation initiale des threads
	if (sem_post(&sem_attente_decision_IA)){
		error("Erreur opération V sur sem");  
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
		cout << "tour : " << to_string(turn) << endl;

		if (list_ally_unit.size()) {

			//display_list_ally_unit();

			/*cout << "genetic : 1" << endl;
			temp_string = "move " + to_string(list_ally_unit.back().id) + " " + to_string(1) + "\n";
			cout << "genetic : 2 and " << temp_string << endl;
			list_action.push_back(temp_string);
			cout << "genetic : 3" << endl;*/

			seed = 123456789;

			initialize(seed);

			evaluate();

			keep_the_best ( );

			for ( generation = 0; generation < MAXGENS; generation++ )
			{
				//cout << "genetic : " << generation << " - " << "8" << endl;

				cout << "selector" << endl;
				selector ( seed );
				cout << "crossover" << endl;
				crossover ( seed );

				cout << "mutate" << endl;
				mutate ( seed );

				cout << "evaluate" << endl;
				//report ( generation );
				evaluate ( );

				cout << "elistist" << endl;
				elitist ( );

			}



			for(int i=0; i < list_ally_unit.size(); i++) {

				if (population[POPSIZE].gene.at(i) != 6) {
					temp_string = "move " + to_string(list_ally_unit.at(i).id) + " " + to_string(population[POPSIZE].gene.at(i)) + "\n";

					list_action.push_back(temp_string);

				}
			}
			fin_du_tour = true;

		}		

		//usleep(500000); // simulation de décision d'action

		//for(int i=0; i < population[POPSIZE].gene.size()-1; i++) {
		/*for(int i=0; i < 1; i++) {
				string action = "move " + to_string(list_ally_unit.at(i).id) + " " + to_string(population[POPSIZE].gene.at(i)) + "\n";
				cout << "action " << i << " : " << action << endl;
				cout << "gene size : " << population[POPSIZE].gene.size() << endl;
				cout << "number of ally units " << list_ally_unit.size() << endl;
				list_action.push_back(action);
			}*/


		timestamp ( );

		cout << "Fini de tourner pour l'algo gén, on a un vainqueur !! Plus qu'à créer la liste d'actions corespondantes ... " << endl;

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
		//for ( i = 0; i < NVARS; i++ ) {
		population[POPSIZE].gene = population[best_mem].gene;
		//}
		population[POPSIZE].fitness = population[best_mem].fitness;
	} else {
		//for ( i = 0; i < NVARS; i++ ) {
		population[worst_mem].gene = population[POPSIZE].gene;
		//}
		population[worst_mem].fitness = population[POPSIZE].fitness;
	}

	//cout << " le meilleur score de cette génération vaut : " << to_string(population[POPSIZE].fitness) << "!!" << endl;

	return;
}

//****************************************************************************

void evaluate ( ) {

	int member;
	int i;
	int unite_num;
	int tour_num;
	double score ;
	int score_to_add ;
	//double x[NVARS+1];
	int x;
	int y;
	int new_x;
	int new_y;
	case_t temp_case;



	for ( member = 0; member < POPSIZE; member++ ) {
		copy_map(); 								// intialisation de temp_map
		temp_list_ally_unit = list_ally_unit;		// initialisation temp_list_ally_unit



		score =0;

		for ( tour_num = 0; tour_num < NBTOURS ; tour_num++ ) {
			for ( unite_num = 0; unite_num < list_ally_unit.size() ; unite_num++ ) {

				//display_list_ally_city();
				// on récupère les coordonnées de l'unité
				score_to_add = 0 ;
				x= temp_list_ally_unit.at(unite_num).x;
				y= temp_list_ally_unit.at(unite_num).y;
				new_x =x;
				new_y =y;

				do {
					switch ( population[member].gene.at( (tour_num)*(list_ally_unit.size())+(unite_num) ) ) {
					case 0 :
						new_x++;
						break;
					case 1 :
						new_y--;
						break;
					case 2 :
						new_x--;
						new_y--;
						break;
					case 3 :
						new_x--;
						break;
					case 4 :
						new_y++;
						break;
					case 5 :
						new_x++;
						new_y++;
						break;
					case 6 :
						// on ne fait rien
						// la pièce ne bouge pas!
						break;
					default :
						break;
					}

					if ( !( 0 <= new_x && new_x < LARGEUR_MAP) && (0 <= new_y && new_y < LONGUEURE_MAP ) ){
						population[member].gene.at( (tour_num)*(list_ally_unit.size())+(unite_num)) =	i4_uniform_ab ( LBOUND, UBOUND, seed );
						continue;


						//score_to_add = score_to_add - 10000 ; // on sort de la map, faut surtout pas faire ça malheureux !
					} else {

						// on regarde à chaque tour si le déplacement rapporte des points
						// on va essayer de pondérer pour qu'une action rapporte plus de points faite maintenant que à 5 tours
						temp_case = cases[new_x][new_y];

						if (x==new_x and y == new_y){

						}
						else if ( temp_case.unite == 2 && temp_case.owner == 1 ) {
							population[member].gene.at( (tour_num)*(list_ally_unit.size())+(unite_num)) =	i4_uniform_ab ( LBOUND, UBOUND, seed );
							continue;
						}

						else if ( temp_case.terrain == 0 ) {
							population[member].gene.at( (tour_num)*(list_ally_unit.size())+(unite_num)) =	i4_uniform_ab ( LBOUND, UBOUND, seed );
							continue;
							//score_to_add = score_to_add - 2500 ; // unité se noie, pas tip top...

						}

						else if ( temp_case.unite == 1 && temp_case.owner == 0 ) {

							score_to_add = score_to_add + PTS_ATTAQUE_VILLE_ENNEMIE;

						}

						else if ( temp_case.unite == 1 && temp_case.owner == -1 ) {

							score_to_add = score_to_add + PTS_ATTAQUE_VILLE_NEUTRE;

						}

						else if ( temp_case.unite == 1 && temp_case.owner == 1 ) {

							score_to_add = score_to_add - 50;

						}

						else if ( temp_case.visible == -1 ) {
							score_to_add = score_to_add + PTS_DECOUVERTE_TERRAIN ;

						}

						else if ( temp_case.visible == 0 ) {

							score_to_add = score_to_add + PTS_VISION_TERRAIN ;

							// pour le moment la vision à 2 cases d'une army est négligée...
						}



						score = score + pow(score_to_add ,((NBTOURS+1-tour_num)/NBTOURS)) ;

						// TODO : on met à jour la matrice temporaire de donnée
						if (temp_map[x][y].unite == 2){temp_map[x][y].unite = -1;};
						temp_map[new_x][new_y].unite = 2;

						temp_list_ally_unit.at(unite_num).x = new_x;
						temp_list_ally_unit.at(unite_num).y = new_y;

					}
				}while(0);
			}
		}
		population[member].fitness = (int)score ;

	}

	// Possibilité d'insérer un affichage global de notre population avec leur score !

	return;
	/*
	int member;
	int i;
	int unite_num;
	int tour_num;
	double score ;
	int score_to_add ;
	//double x[NVARS+1];
	int x;
	int y;
	int new_x;
	int new_y;
	case_t temp_case;



	for ( member = 0; member < POPSIZE; member++ ) {
		copy_map(); 								// intialisation de temp_map
		temp_list_ally_unit = list_ally_unit;		// initialisation temp_list_ally_unit
		score =0;

		for ( tour_num = 0; tour_num < NBTOURS ; tour_num++ ) {
			for ( unite_num = 0; unite_num < list_ally_unit.size() ; unite_num++ ) {

				// on récupère les coordonnées de l'unité
				score_to_add = 0 ;
				x= temp_list_ally_unit.at(unite_num).x;
				y= temp_list_ally_unit.at(unite_num).y;
				new_x =x;
				new_y =y;
				switch ( population[member].gene.at( (tour_num)*(list_ally_unit.size())+(unite_num) ) ) {
				case 0 :
					new_x++;
					break;
				case 1 :
					new_y--;
					break;
				case 2 :
					new_x--;
					new_y--;
					break;
				case 3 :
					new_x--;
					break;
				case 4 :
					new_y++;
					break;
				case 5 :
					new_x++;
					new_y++;
					break;
				case 6 :
					// on ne fait rien
					// la pièce ne bouge pas!
					break;
				default :
					break;
				}

				if ( !( 0 <= new_x <= LARGEUR_MAP) && (0 <= new_y <= LONGUEURE_MAP ) ){
					score_to_add = score_to_add - 10000 ; // on sort de la map, faut surtout pas faire ça malheureux !
				} else {
					// on regarde à chaque tour si le déplacement rapporte des points
					// on va essayer de pondérer pour qu'une action rapporte plus de points faite maintenant que à 5 tours
					temp_case = cases[new_x][new_y];

					if ( temp_case.unite != ' ' && temp_case.unite != 'O' && temp_case.owner == 1 ) {
						score_to_add = score_to_add - 100 ; // on attaque sa propre unité, c'est mal !
					}

					else if ( temp_case.terrain == 0 ) {
						score_to_add = score_to_add - 25 ; // unité se noie, pas tip top...
					}

					else if ( temp_case.unite == 'O' && temp_case.owner == 0 ) {
						score_to_add = score_to_add + PTS_ATTAQUE_VILLE_ENNEMIE;
					}

					else if ( temp_case.unite == '0' && temp_case.owner == -1 ) {
						score_to_add = score_to_add + PTS_ATTAQUE_VILLE_NEUTRE;
					}

					else if ( temp_case.visible == -1 ) {
						score_to_add = score_to_add + PTS_DECOUVERTE_TERRAIN ;
					}

					else if ( temp_case.visible == 0 ) {
						score_to_add = score_to_add + PTS_VISION_TERRAIN ;
						// pour le moment la vision à 2 cases d'une army est négligée...
					}

					score = score + pow(score_to_add ,((NBTOURS+1-tour_num)/NBTOURS)) ;
					// TODO : on met à jour la matrice temporaire de donnée
					temp_map[x][y].unite = ' ';
					temp_map[new_x][new_y].unite = 'A';
				}

			}
		}
		population[member].fitness = (int)score ;
	}

	// Possibilité d'insérer un affichage global de notre population avec leur score !

	return;
	 */
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



	nvars = list_ally_unit.size() * NBTOURS;

	//  Initialize variables within the bounds
	for ( i = 0; i < nvars; i++ ) {		// inutile ??????????

		for ( j = 0; j < POPSIZE; j++ ) {
			population[j].fitness = 0;
			population[j].rfitness = 0;
			population[j].cfitness = 0;
			population[j].gene.push_back( i4_uniform_ab ( LBOUND, UBOUND, seed ) ); // un int random entre 0 et 6 ( compris )
			// tous les mouvements possibles
		}
	}


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

	//  Once the best member in the population is found, copy the genes in population POPSIZE
	//  l'indidivu POPSIZE+1 sert de mémo !
	//  for ( i = 0; i < nvars; i++ ) {
	population[POPSIZE].gene = population[cur_best].gene; // <= copie de tous les genes !
	//  }

	cout << "le meilleur de la première génération a un score de : " << to_string(population[POPSIZE].fitness) << " (qui devrait d'ailleurs valoir si tout se passe bien : " << to_string(population[cur_best].fitness) << ") ! " << endl ;
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
		for ( j = 0; j < nvars; j++ ) {
			x = r8_uniform_ab ( a, b, seed );
			if ( x < PMUTATION ) {
				population[i].gene.at(j) = r8_uniform_ab ( LBOUND, UBOUND, seed );
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

/*
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

 */

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

	//cout << "selector 1" << endl;
	//  Calculate the relative fitness of each member. ( moyenne )
	for ( mem = 0; mem < POPSIZE; mem++ ) {
		population[mem].rfitness = population[mem].fitness / sum;
	}

	//cout << "selector 2" << endl;
	//  Calculate the cumulative fitness.
	population[0].cfitness = population[0].rfitness;
	for ( mem = 1; mem < POPSIZE; mem++ ) {
		population[mem].cfitness = population[mem-1].cfitness +
				population[mem].rfitness;
	}

	cout << "selector 3" << endl;

	//  Select survivors using cumulative fitness.
	for ( i = 0; i < POPSIZE; i++ ) {
		p = r8_uniform_ab ( a, b, seed );
		if ( p < population[0].cfitness ) {
			newpopulation[i] = population[0];
		} else {
			for ( j = 0; j < POPSIZE-1; j ++ ) {
				if ( population[j].cfitness <= p && p < population[j+1].cfitness ) {
					newpopulation[i] = population[j+1];
				}
			}
		}
	}

	cout << "selector 4" << endl;

	//  Overwrite the old population with the new one.
	for ( i = 0; i < POPSIZE; i++ ) {
		population[i] = newpopulation[i];
	}

	cout << "selector 5" << endl;
	// TODO : affichage new_pop !

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
	point = i4_uniform_ab ( 0, nvars - 1, seed );

	//  Swap genes in positions 0 through POINT-1.
	for ( i = 0; i < point; i++ ) {
		t                       = population[one].gene.at(i);
		population[one].gene.at(i) = population[two].gene.at(i);
		population[two].gene.at(i) = t;
	}

	return;
}


//****************************************************************************

void display_list_action() {
	cout << "list action complete : " << endl;
	for( int i = 0; i < list_action.size(); i++) {
		cout << string(list_action.at(i)) << endl;
	}
}









