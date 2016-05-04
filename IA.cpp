//============================================================================
// Name        : IA.cpp
// Author      : IR I IA2
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include "networking.h"

using namespace std;

void task1(string msg)
{
    cout << "task1 says: " << msg;
}

int main(int argc, char * argv[]) {
	if (argc != 3){
		cout << "Erreur de nombre d'arguments !" << endl;
		cout << "Usage : " << argv[0] << " <nom serveur> <port serveur>" << endl;
		return -1;
	}
	cout << "!!! Lancement de l'IA algo génétique !!!" << endl;
	cout << "L'IA va se connecter à la machine " << argv[1] << " sur le port " << argv[2] << "." << endl;

	thread t1(task1, "Hello");
	t1.join();

	return 0;
}
