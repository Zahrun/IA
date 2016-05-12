#ifndef DATA_H_
#define DATA_H_

#include <strings.h>	
#include <vector>
#include "data.h"
#include "globals.h"

#define BUFFER_SIZE 2048
#define NUMBER_OF_WORDS 14
#define LONGUEURE_MAP 50
#define LARGEUR_MAP 50

const string WORDS[NUMBER_OF_WORDS] = {"width","get_action","set_visible","delete_piece","enter_piece","enter_city","leave_piece","leave_city","leave_terrain","lose_city","move","invade","error","create_piece"};


struct case_t{
	char unite ; // 'A' 'F' 'O' etc ( ' ' for nothing ) on garde les notations symboliques de la GUI de base
	int terrain ; // mer => 0 et terre => 1
	int visible ; // pas découvert => -1, découvert mais plus à jour => 0, découvert et visible (actualisé) => 1
	int owner ; // Joueur => 0, Adversaire => 1, -1 si à personne ( ville )
	int id ; // piece_id ou city_id
	int transport ; // nombre d'unités transportées ( pour ville ou Transport )
};

struct unit{
	char unite;
	int id;
	int owner;
};

struct city{
	int id;
	int owner;
};

extern case_t cases[LONGUEURE_MAP][LARGEUR_MAP];
// Matrice de cases où on va enregistrer tout ce que nous retourne le serveur.

extern vector<unit> list_unit;

extern vector<city> list_city;

void initCases();

// peut être vaudrait-il mieux garder un vector pour lier chaque id pièce à sa position!
void find_and_destroy(int id_piece);


void find_and_contain_transporter(int id_container, int diff);

void change_state_city(int id_city);

void new_piece( int id_city );

void add_unit(int id, char symbol, int owner);

void delete_unit(int id);

#endif /* DATA_H_ */



