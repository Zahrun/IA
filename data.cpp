#include "data.h"
#include "genetic.h"

case_t cases[LONGUEURE_MAP][LARGEUR_MAP];

vector<unit> list_ally_unit;

vector<city> list_city;

vector<city> list_ally_city;

void initCases(){
	for (int i = 0; i < LONGUEURE_MAP; i++){
		for (int j = 0; j < LARGEUR_MAP; j++){
			cases[i][j].unite = ' ';
			cases[i][j].terrain = -1;
			cases[i][j].visible = -1;
			cases[i][j].owner = -1;
			cases[i][j].id = -1;
			cases[i][j].transport = 0;
		}
	}
}

// peut être vaudrait-il mieux garder un vector pour lier chaque id pièce à sa position!
void find_and_destroy(int id_piece){
	int found=0;
	for (int i = 0; i < LONGUEURE_MAP; i++){
		for (int j = 0; j < LARGEUR_MAP; j++){
			if ( cases[i][j].unite != '0'  and cases[i][j].id == id_piece ){
				cases[i][j].unite= ' ';
				cases[i][j].id = -1;
				found=1;
				break;
			}
		}
		if (found==1){break;} // ça sert à rien de continuer de chercher
	}
}

void find_and_contain_transporter(int id_container, int diff){ // diff vaut 1 ou -1 selon si la pièce entre ou sort du container
	int found=0;
	for (int i = 0; i < LONGUEURE_MAP; i++){
		for (int j = 0; j < LARGEUR_MAP; j++){
			if ( cases[i][j].unite != 'T'  and cases[i][j].id == id_container ){
				cases[i][j].transport=cases[i][j].transport+diff;
				found=1;
				break;}
		}
		if (found==1){break;} // ça sert à rien de continuer de chercher
	}
}

void change_state_city(int id_city){
	int found=0;
	for (int i = 0; i < LONGUEURE_MAP; i++){
		for (int j = 0; j < LARGEUR_MAP; j++){
			if ( cases[i][j].unite == 'O'  and cases[i][j].id == id_city ){
				if (cases[i][j].owner==1){cases[i][j].owner=-1;}
				else{cases[i][j].owner=1;}
				found=1;
				break;}
		}
		if (found==1){break;} // ça sert à rien de continuer de chercher
	}
}

void new_piece( int id_city ){ // add infos pieces somewhere
	int found=0;
	for (int i = 0; i < LONGUEURE_MAP; i++){
		for (int j = 0; j < LARGEUR_MAP; j++){
			if ( cases[i][j].id == id_city ){
				cases[i][j].transport = cases[i][j].transport + 1;
				found=1;
				break;}
		}
		if (found==1){break;} // ça sert à rien de continuer de chercher
	}

}

void add_ally_unit(int id, char symbol, int owner, int x, int y) {
	unit temp;
	temp.id = id;
	temp.x = x;
	temp.y =y;
	temp.unite = symbol;
	temp.owner = owner;
	list_ally_unit.push_back(temp);
}

void delete_ally_unit(int id) {
	for(int i=0; i < list_ally_unit.size(); i++) {
		if (list_ally_unit.at(i).id == id) {
			list_ally_unit.erase(list_ally_unit.begin()+i);
			break;
		}
	} 
}

void add_city(int id, int owner, int x, int y) {
	city temp_city;
	bool found = false;
	for(int i = 0; i < list_city.size(); i++) {
		if (list_city.at(i).id == id) {
			found = true;
			break;
		}
	}
	if (not(found)) {
		temp_city.id = id;
		temp_city.owner = owner;
		temp_city.x = x;
		temp_city.y = y;
		list_city.push_back(temp_city);
	}
}

void add_ally_city(int id, int owner, int x, int y){
	city temp_city;
	bool found = false;
	cout << "add ally city : 1" << endl;
	for(int i = 0; i < list_ally_city.size(); i++) {
		if (list_ally_city.at(i).id == id) {
			found = true;
			cout << "add ally city : 2" << endl;
		}
	}
	cout << "add ally city : 3" << endl;
	if (not(found)) {
		cout << "add ally city : 4" << endl;
		temp_city.id = id;
		temp_city.owner = owner;
		temp_city.x = x;
		temp_city.y = y;
		list_ally_city.push_back(temp_city);
	}
}


void copy_map() {
	for (int i = 0; i < LONGUEURE_MAP; i++){
		for (int j = 0; j < LARGEUR_MAP; j++){
			temp_map[i][j] = cases[i][j];
		}
	}
}


void display_list_ally_city() {
	cout << "ally city : " << endl;
	for(int i=0; i < list_ally_city.size(); i++){
		cout << "city id : " << to_string(list_ally_city.at(i).id)  << " /  x : " << to_string(list_ally_city.at(i).x) << " /  y : " << to_string(list_ally_city.at(i).y) << endl;
	}
}

void display_list_ally_unit() {
	cout << "ally unit : " << endl;
	if (list_ally_unit.size() > 0) {
		for(int i=0; i < list_ally_unit.size(); i++){
			cout << "unit " << to_string(list_ally_unit.at(i).unite) << " id : " << to_string(list_ally_unit.at(i).id) << " /  x : " << to_string(list_ally_unit.at(i).x) << " /  y : " << to_string(list_ally_unit.at(i).y) << endl;
		}
		cout << "ally_unit : ok" << endl;
	}
}


int get_x_city(int id_city){
	//cout << "ally city : " << endl;
	for(int i=0; i < list_ally_city.size(); i++){

		if ( list_ally_city.at(i).id == id_city ) {
			return list_ally_city.at(i).x ;
		}
	}
	cout << " probleme get_x_city de " << to_string(id_city) << endl;
}

int get_y_city(int id_city){
	//cout << "ally city : " << endl;
	for(int i=0; i < list_ally_city.size(); i++){

		if ( list_ally_city.at(i).id == id_city ) {
			return list_ally_city.at(i).y ;
		}
	}
	cout << " probleme get_y_city de " << to_string(id_city) << endl;
}








