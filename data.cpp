#include "data.h"

case_t cases[LONGUEURE_MAP][LARGEUR_MAP];

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
