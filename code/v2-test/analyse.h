#ifndef ANALYSE_H
#define ANALYSE_H

/*
* Fonction qui ouvre un fichier
* @param nom_fichier le nom du fichier à ouvrir
* @brief ouvre un fichier
* @return void
*/
void ouverture_fichier(char *nom_fichier);

/*
* Fonction qui ferme un fichier
* @brief ferme un fichier
* @return void
*/
void fermeture_fichier();

/*
* Fonction qui lit un caractère
* @brief lit un caractère
* @return void
*/
void terminer();

void passer_espaces();


/*
* Fonction qui lit un caractère
* @brief lit un caractère
* @return void
*/
void lire_caractere();

/*
* Fonction qui consomme un caractère
* @param caractere le caractère à lire
* @brief consomme un caractère
* @return void
*/
void consommer(char caractere);

/*
* Fonction qui permet d'afficher un buffer
* @brief affiche un buffer
* @return void
*/
void affichage_buffer();

void creation_item();

void creation_token();





#endif /* ANALYSE_H */
