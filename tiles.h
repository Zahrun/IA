#define LONGUEURE_MAP 50
#define LARGEUR_MAP 50

struct case_t{
	char unite ; // 'A' 'F' 'O' etc ( ' ' for nothing ) on garde les notations symboliques de la GUI de base
	int terrain ; // mer => 0 et terre => 1
	int visible ; // pas découvert => -1, découvert mais plus à jour => 0, découvert et visible (actualisé) => 1
	int owner ; // Joueur => 1, Adversaire => -1, 0 si à personne ( ville )
	int id ; // piece_id ou city_id
	int transport ; // nombre d'unités transportées ( pour ville ou Transport )
};

struct case_t cases[LONGUEURE_MAP][LARGEUR_MAP];



void initCases();
void find_and_destroy(int id_piece);
void find_and_contain_transporter(int id_container, int diff);
void change_state_city(int id_city);
void new_piece( int id_city );
