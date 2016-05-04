//============================================================================
// Name        : IA.cpp
// Author      : IR I IA2
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

int main(int argc, char * argv[]) {
	if (argc != 3){
		cout << "Erreur de nombre d'arguments !" << endl;
		cout << "Usage : " << argv[0] << " <nom serveur> <port serveur>" << endl;
		return -1;
	}
	cout << "!!! Lancement de l'IA algo génétique !!!" << endl;
	cout << "L'IA va se connecter à la machine " << argv[1] << " sur le port " << argv[2] << "." << endl;


	return 0;
}
