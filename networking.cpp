/*
 * networking.cpp
 *
 */

#include "networking.h"
#include "globals.h"
#include "data.h"
#include "genetic.h"
#include <cstring>

#define BUFFER_SIZE 2048
#define NUMBER_OF_WORDS 14


int sockfd;

const char* stet;
char* temp;

void sendAction(char* action, int length){

	if (sem_post(&sem_attente_get_action)){
		error("Erreur oppération V sur sem");
	}
	cout << "Ntwrk : en attente de l'action à envoyer..." << endl;
	if (sem_wait(&sem_attente_decision_IA)){
		error("Erreur oppération P sur sem");
	}
	//const void* action = "end_turn\n";
	temp = action;
	write(sockfd, temp, length);
}

int recognizeWord(string word){
	for (int i = 0; i<NUMBER_OF_WORDS; i++){
		if (word == WORDS[i]){
			return i;
		}
	}
	return 0;
}

void readDescription(char * buffer){
	cout << "*** Parsing message :" << endl;
	istringstream issBuf(buffer), issLine;
	string line, word;
	while (getline(issBuf,line) > 0){
		issLine.str(line);
		while(issLine >> word) {
			cout << word << " ";
		}
		cout << endl;
		issLine.clear();
	}
	issBuf.clear();
	cout << "*** Message parsed" << endl;
}

void readMessage(char * buffer){
	cout << "*** Parsing first word of message" << endl;
	istringstream issBuf(buffer);
	string firstWord;
	int x;
	int y;
	string tile; // "Ground" OR "Water"
	string unite;
	int id;
	int id_container;
	int id_city;
	int owner;
	char piece_symbol;
	int piece_type;

	/* On va mettre chaque paramètres ( integers ) dans un tableau */


	if (!(issBuf >> firstWord)){ // cpp : >> on a stream reads one word at a time
		error("buffer vide !!!");
	}
	switch (recognizeWord(firstWord)){
	case 0 : // width
		cout << "premier mot : width" << endl;
		readDescription(buffer);
		break;
		
	case 1 : // get_action
		cout << "premier mot : get_action" << endl;
		//usleep(2000000);	

		if (list_action.size() == 0) {
			temp = "end_turn\n";
		} else {
			stet = list_action.front();
			list_action.erase(list_action.begin());
		}

		sendAction(temp,strlen(temp));

		break;
		
	case 2 : // set_visible
		/* il y a 4 façons d'uliser set_visible
			-> avec 4 / 5 / 6 ou 8 params complémentaires */
	
		issBuf >> x; // horizontal puis vertical
		issBuf >> y;
		issBuf >> tile;
		if (tile=="Ground"){
			cases[y][x].terrain=1;}
		else if (tile=="Water"){ // "Water" | "Ground" cf type terain Empire.ml empire-server
			cases[y][x].terrain=0;}
		else { cout << "problem reading tile" << endl;}

		issBuf >> unite;
		if (unite=="none"){
			cases[y][x].unite=' ';
			if (cases[y][x].visible == -1) { // terrain n'était pas découvert du tout
				cases[y][x].visible = 1;}
			else if ( cases[y][x].visible == 0) { // terrain était en ombre
				cases[y][x].visible = 1; }
			else { // terrain était actualisé et passe en ombre
				cases[y][x].visible = 0; }
		}
		else if (unite=="city"){ // ville prise par aucun joueur
			cases[y][x].unite='O';
			cases[y][x].owner=-1;
			issBuf >> id;
			issBuf >> owner;
			add_city(id, owner, x, y);
		}
		else if (unite=="city_owned"){ // ville prise par un joueur
			cases[y][x].unite='O';
			issBuf >> id;
			cases[y][x].id=id;
			issBuf >> owner; // dans le code du prof : 0 = joueur 0 et 1 = joueur 1;
			cases[y][x].owner=owner;
			add_city(id, owner, x, y);
			add_ally_city(id, owner, x, y);
			}
		else if (unite=="piece"){
			issBuf >> owner; // TODO : check format of "owner" ! C'est le numero de joueur
			cases[y][x].owner=owner;
			issBuf >> piece_symbol; // bien le char représentant l'unité
			cases[y][x].unite=piece_symbol;
			issBuf >> id;
			cases[y][x].id=id;
			issBuf >> piece_type;} // pas besoin ( si symbole fonctionne... ) => symbole est implémenté correctement	
		break;
		
	case 3 : // delete_piece
		issBuf >> id ;
		find_and_destroy(id);	
		delete_ally_unit(id);
		break;

	case 4 : // enter_piece
		issBuf >> id ;
		issBuf >> id_container ;
		find_and_contain_transporter(id_container,1);	
		break;	

	case 5 : // enter_city
		issBuf >> id ;
		issBuf >> id_city ;
		find_and_contain_transporter(id_city,1);	
		break;
		
	case 6 : // leave_piece
		issBuf >> id ;
		issBuf >> id_container ;
		find_and_contain_transporter(id_container,-1);	
		break;

	case 7 : // leave_city
		issBuf >> id ;
		issBuf >> id_city ;
		find_and_contain_transporter(id_city,-1);	
		break;
		
	case 8 : // leave_terrain
		issBuf >> id;
		issBuf >> y;
		issBuf >> x;			
		cases[y][x].id=-1;
		cases[y][x].unite=' ';
		cases[y][x].owner=0;
		cases[y][x].transport=0;		
		break;

	case 9 : // lose_city
		issBuf >> id_city;
		change_state_city(id_city);
		break;
		
	case 10 : //move
		// TODO, besoin ou pas?
		cout << "move not handled yet..." << endl;
		break;
		
	case 11 : // invade_city
		issBuf >> id_city;
		issBuf >> y;
		issBuf >> x;		
		cases[y][x].id=id_city;
		cases[y][x].unite='0';
		cases[y][x].owner=1;
		cases[y][x].transport=1; // une unité dans la ville ( celle qui vient de la prendre )	
		
	case 12 : // error
		cout << "Error in readMessage : " << issBuf << endl;
		break;
		
	case 13 : // create_piece
		issBuf >> id;
		issBuf >> piece_type;
		issBuf >> piece_symbol;
		issBuf >> id_city;
		new_piece( id_city );
		add_ally_unit(id,piece_symbol,1);
		
		break;		
	
	default :
		cout << "message non reconnu : \"" << firstWord << "\" !!!" << endl;
	}
	cout << "One message parsed" << endl;

}

void connexion(char * host, int port)
{
	// synchro initiale
	cout << "Synchronisation des threads..." << endl;
	if (sem_wait(&sem_attente_decision_IA)){
		error("Erreur oppération P sur sem");
	}
	cout << "Threads synchronisés" << endl;


	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[BUFFER_SIZE];

	cout << "Tentative de connexion a " << host << ":" << port << endl;

	// create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	// create host
	server = gethostbyname(host);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	// create server address
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(port);
	// connexion au serveur
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR connecting");
	cout << "Connexion au serveur réussie !!!" << endl;
	// lecture messages
	int n;

	initCases();

	while (1){
		n =read(sockfd,buffer,BUFFER_SIZE - 1);
		if (n < 0)
			error("ERROR reading from socket");
		if (n == 0){
			cout << "fin de la lecture: lu = 0" << endl;
			break;
		}
		readMessage(buffer);
		cout << "reçu :\"" << buffer << "\"" << endl;
		bzero(buffer, 256);
	}
	close(sockfd);
}

