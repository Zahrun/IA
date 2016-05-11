/*
 * networking.cpp
 *
 */

#include "networking.h"
#include "globals.h"
#include "data.h"


int sockfd;


void sendAction(const void* action){
	if (sem_post(&sem_attente_get_action)){
		error("Erreur oppération V sur sem");
	}
	cout << "Ntwrk : en attente de l'action à envoyer..." << endl;
	if (sem_wait(&sem_attente_decision_IA)){
		error("Erreur oppération P sur sem");
	}
	//const void* action = "end_turn\n";
	write(sockfd, action, 9);
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
	string tile; // ground OR water
	string unite;
	int id;
	int id_container;
	int id_city;
	int owner;
	char piece_symbol;
	string piece_type;

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
		/*if (sem_wait(&sem_attente_decision_IA)){
			error("Erreur opération V sur sem"); 
		}*/
		sendAction("end_turn\n");
		break;
		
	case 2 : // set_visible
		/* il y a 4 façons d'uliser set_visible
			-> avec 4 / 5 / 6 ou 8 params complémentaires */
	
		issBuf >> y; // peut être l'inverse entre x et y!
		issBuf >> x;
		issBuf >> tile;
		if (tile=="ground"){
			cases[y][x].terrain=1;}
		else if (tile=="water"){
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
			cases[y][x].owner=-1;}
		else if (unite=="city_owned"){ // ville prise par un joueur
			cases[y][x].unite='O';
			issBuf >> id;
			cases[y][x].id=id;
			issBuf >> owner; // TODO : check format of "owner" !
			cases[y][x].owner=owner;}
		else if (unite=="piece"){
			issBuf >> owner; // TODO : check format of "owner" !
			cases[y][x].owner=owner;
			issBuf >> piece_symbol; // TODO : check si symbole à bien été implanté comme prévu dans les NOTES
			cases[y][x].unite=piece_symbol;
			issBuf >> id;
			cases[y][x].id=id;
			issBuf >> piece_type;} // pas besoin ( si symbole fonctionne... )	
		break;
		
	case 3 : // delete_piece
		issBuf >> id ;
		find_and_destroy(id);	
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
		cases[y][x].owner=-1;
		cases[y][x].transport=0;		
		break;
		
/* TODO : from lose_city to create_piece
TODO : vérifier que tous les formats ( typages ) sont corrects ! */
		
	default :
		cout << "message non reconnu : \"" << firstWord << "\" !!!" << endl;
	}
	cout << "*** First word of message parsed" << endl;

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

