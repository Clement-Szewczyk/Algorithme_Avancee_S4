#ifndef GRAMMAIRE_H
#define GRAMMAIRE_H

// Enumération pour les étiquettes des tokens
typedef enum e_etiq
{
    debut_doc,
    fin_doc,
    debut_section,
    fin_section,
    debut_annexe,
    fin_annexe,
    debut_titre,
    fin_titre,
    debut_liste,
    fin_liste,
    debut_item,
    fin_item,
    item_texte,
    item_liste,
    mot_enrichi,
    debut_important,
    fin_important
} t_etiq;

// Structure pour représenter un token
typedef struct s_token
{
    t_etiq l_etiquette;
    char la_valeur[81];
} t_token;

void amorcer(char *nom_fichier);

void lire_caractere();

void consommer(t_etiq terminal);

void lire_token();

void passer_espace();

void text_enrichi();

void document();

void annexes();

void contenu();

void section();

void titre();

void liste();

void item();

void liste_texte();

void texte_liste();

void texte();

void mot_riche();

void mot_simple();

void mot_important();

#endif