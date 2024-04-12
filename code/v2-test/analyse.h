#ifndef ANALYSE_H
#define ANALYSE_H


typedef enum e_etiq
{
    DEBUT_DOC,
    FIN_DOC,
    DEBUT_ANNEXE,
    FIN_ANNEXE,
    DEBUT_TITRE,
    FIN_TITRE,
    DEBUT_SECTION,
    FIN_SECTION,
    DEBUT_LISTE,
    FIN_LISTE,
    DEBUT_ITEM,
    FIN_ITEM,
    DEBUT_IMPORTANT,
    FIN_IMPORTANT,
    SAUT_LIGNE,
    MOT
}t_etiq;

//Tableau image pour l'enumération

extern const char *etiq_str[];

typedef struct s_token
{
    t_etiq type;
    char* la_valeur;
} t_token;

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

int verification();

void afficher_token();

void consommer_token();

//Grammaire
void Text_enrichi();

void Document();

void Annexe();

void contenu();

void section();

void titre();

void mot_enrichi();

void liste();

void texte();

void mot_simple();

void mot_important();

void item();

void texte_liste();

void liste_texte();

#endif /* ANALYSE_H */


