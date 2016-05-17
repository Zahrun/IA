/*
 * networking.cpp
 *
 
Warning !!
Il semblerait qu'il faille plutôt utiliser des const char* plutôt que char* en c++
à peu près partout...

compilation => " warning: deprecated conversion from string constant to ‘char*’ "
http://stackoverflow.com/questions/59670/how-to-get-rid-of-deprecated-conversion-from-string-constant-to-char-warnin
 
 *
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
string temp;
string end_turn = "end_turn\n";
//string temp_string;

void sendAction(string action, int length){ 

	cout << "before the write : " << string(action) << endl;
	write(sockfd, action.c_str(), length);
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
	istringstream issBuffer(buffer), issBuf;
	string firstWord, line;
	int x;
	int y;
	string tile; // "ground" OR "water"
	string unite;
	int id;
	int id_container;
	int id_city;
	int owner;
	char piece_symbol;
	int piece_type;
	bool test = true;

	/* On va mettre chaque paramètre ( integers ) dans un tableau */

while (getline(issBuffer,line) > 0){
		issBuf.str(line);

	if (!(issBuf >> firstWord)){ // cpp : >> on a stream reads one word at a time
		error("buffer vide !!!");
	}

	switch (recognizeWord(firstWord)){
	case 0 : // width
		cout << "premier mot : width" << endl;
		//readDescription(buffer);
		break;
		
	case 1 : // get_action
		cout << "premier mot : get_action" << endl;
		
		if (sem_post(&sem_attente_get_action)){
			error("Erreur oppération V sur sem"); 
		}
		cout << "Ntwrk : en attente de l'action à envoyer..." << endl;
		if (sem_wait(&sem_attente_decision_IA)){
			error("Erreur oppération P sur sem");
		}

		cout << "size list action : " << to_string(list_action.size()) << endl;
		display_list_action();
		if (list_action.size() == 0) {
			sendAction(end_turn,strlen(end_turn.c_str()));
			cout << "end_turn sent" << endl;
		} else {
			temp = list_action.front();
			cout << "action to send : " << string(temp) << endl;
			list_action.erase(list_action.begin());
			sendAction(temp,strlen(temp.c_str()));
		}



		break;
		
	case 2 : // set_visible
		/* il y a 4 façons d'uliser set_visible
			-> avec 4 / 5 / 6 ou 8 params complémentaires */
	
		issBuf >> x; // horizontal puis vertical
		issBuf >> y;
		issBuf >> tile;
		if (tile=="ground"){
			cases[y][x].terrain=1;}
		else if (tile=="water"){ // "water" | "ground" cf type terain Empire.ml empire-server
			cases[y][x].terrain=0;}
		else { cout << "problem reading tile" << endl;}

		issBuf >> unite;
		cout << "unite : " << string(unite) << endl;
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
		else if (unite=="owned_city"){ // ville prise par un joueur
			cases[y][x].unite='O';
			issBuf >> id;
			cases[y][x].id=id;
			issBuf >> owner; // dans le code du prof : 0 = joueur 0 et 1 = joueur 1;
			cases[y][x].owner=owner;
			add_city(id, owner, x, y);
			add_ally_city(id, owner, x, y);
			cout << "add ally city : 0" << endl;
			}
		else if (unite=="piece"){
			issBuf >> owner; // C'est le numero de joueur ( 0 | 1 )
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
		// normalement on reçoit pas de move, on les envoie
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
		add_ally_city(id_city, owner, x, y);
		new_city = true;
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
	
	issBuf.clear();
} 

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
		cout << "reçu :\"" << buffer << "\"" << endl;
		readMessage(buffer);

		bzero(buffer, 256);
	}
	close(sockfd);
}

