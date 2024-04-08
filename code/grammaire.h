#ifndef GRAMMAIRE_H
#define GRAMMAIRE_H

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

typedef struct s_token
{
    t_etiq l_etiquette;
    char la_valeur[81];
} t_token;

void amorcer(char *nom_fichier);
void terminer();
void lire_caractere();
void lire_token(t_token *token);
void passer_espace();
void mettre_a_jour_token(t_token *token, char *balise);
void consommer(char caractere);
void consommer_token(char *token_str);
void fermeture_balise(t_etiq balise_contenante);
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
void mot_important();
void mot_simple();

#endif