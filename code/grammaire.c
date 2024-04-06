#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammaire.h"

#define BUFFER_SIZE 1024

FILE *fichier;
char buffer[BUFFER_SIZE];
int buffer_index = 0;
int buffer_size = 0;
t_token mon_token;
char mon_caractere;

void amorcer(char *nom_fichier)
{
    fichier = fopen(nom_fichier, "r");
    if (fichier == NULL)
    {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    }
    lire_caractere();
}

void terminer()
{
    fclose(fichier);
}

void lire_caractere()
{
    if (buffer_index >= buffer_size || buffer_index == 0)
    {
        buffer_size = fread(buffer, 1, BUFFER_SIZE, fichier);
        buffer_index = 0;
    }
    if (buffer_size == 0)
    {
        mon_caractere = EOF;
    }
    else
    {
        mon_caractere = buffer[buffer_index++];
    }
}

void lire_token(t_token *token)
{
    char token_str[20];
    int i = 0;

    // Ignorer les espaces avant la balise
    passer_espace();

    // Si le premier caractère est '<', c'est une balise
    if (mon_caractere == '<')
    {
        // Lecture de la balise
        printf("Lecture de la balise : ");
        while (mon_caractere != '>' && mon_caractere != EOF)
        {
            printf("%c", mon_caractere); // Affichage du caractère actuel
            token_str[i] = mon_caractere;
            lire_caractere();
            i++;
        }
        if (mon_caractere == EOF)
        {
            fprintf(stderr, "Erreur : balise non terminée\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("%c", mon_caractere); // Affichage du chevron '>'
            token_str[i] = '>';
            token_str[i + 1] = '\0';

            // Consommer le caractère '>'
            consommer('>');

            // Ignorer les espaces après la balise
            passer_espace();

            // Mettre à jour le token
            mettre_a_jour_token(token, token_str);
        }
    }
    else
    {
        // Si ce n'est pas une balise, c'est du texte
        while (mon_caractere != '<' && mon_caractere != EOF)
        {
            // Ignorer les caractères spéciaux
            if (mon_caractere != '\n' && mon_caractere != '\t' && mon_caractere != '\r')
            {
                printf("%c", mon_caractere); // Affichage du caractère de texte
                lire_caractere();
            }
            else
            {
                lire_caractere(); // Passer les caractères spéciaux
            }
        }
        printf("\n");

        // Mettre à jour le token avec du texte enrichi
        mettre_a_jour_token(token, "texte_enrichi");
    }
}

void mettre_a_jour_token(t_token *token, char *token_str)
{
    // Ajouter des messages de débogage
    printf("Mise à jour du token avec la balise : %s\n", token_str);
    if (strcmp(token_str, "<document>") == 0)
    {
        token->l_etiquette = debut_doc;
    }
    else if (strcmp(token_str, "</document>") == 0)
    {
        token->l_etiquette = fin_doc;
    }
    else if (strcmp(token_str, "<annexe>") == 0)
    {
        token->l_etiquette = debut_annexe;
    }
    else if (strcmp(token_str, "</annexe>") == 0)
    {
        token->l_etiquette = fin_annexe;
    }
    else if (strcmp(token_str, "<titre>") == 0)
    {
        token->l_etiquette = debut_titre;
    }
    else if (strcmp(token_str, "</titre>") == 0)
    {
        token->l_etiquette = fin_titre;
    }
    else if (strcmp(token_str, "<liste>") == 0)
    {
        token->l_etiquette = debut_liste;
    }
    else if (strcmp(token_str, "</liste>") == 0)
    {
        token->l_etiquette = fin_liste;
    }
    else if (strcmp(token_str, "<mot_important>") == 0)
    {
        token->l_etiquette = debut_important;
    }
    else if (strcmp(token_str, "</mot_important>") == 0)
    {
        token->l_etiquette = fin_important;
    }
    else if (strcmp(token_str, "<section>") == 0)
    {
        token->l_etiquette = debut_section;
    }
    else if (strcmp(token_str, "</section>") == 0)
    {
        token->l_etiquette = fin_section;
    }
    else if (strcmp(token_str, "<br/>") == 0)
    {
        token->l_etiquette = item_texte;
    }
    else
    {
        printf("\nERREUR : balise non existante: %s . \n", token_str);
        exit(-1);
    }
}

void consommer(char caractere)
{
    if (mon_caractere == caractere)
    {
        lire_caractere(); // Avancer le curseur pour lire le prochain caractère
    }
    else
    {
        printf("Erreur: caractere attendu: %c, caractere trouve: %c\n", caractere, mon_caractere);
    }
}

void consommer_token(char *token_str)
{
    int i = 0;
    while (token_str[i] != '\0')
    {
        consommer(token_str[i]); // Utilisation de la version à un seul argument de consommer
        i++;
    }
}

void fermeture_balise(t_etiq balise_contenante)
{
    if (mon_token.l_etiquette != balise_contenante)
    {
        printf("Erreur: balise de fermeture attendue: %d, balise de fermeture trouvée: %d\n", balise_contenante, mon_token.l_etiquette);
        exit(EXIT_FAILURE);
    }
    else
    {
        // Avancer le curseur après la balise de fermeture
        lire_token(&mon_token);
    }
}

void passer_espace()
{
    if (mon_caractere == EOF)
    {
        return;
    }
    while (mon_caractere == ' ' || mon_caractere == '\n' || mon_caractere == '\t' || mon_caractere == '\r')
    {
        lire_caractere(); // Correction : Ne pas passer d'argument
    }
}

void text_enrichi()
{
    document();
    annexes();
}

void document()
{
    printf("passe par document\n");
    lire_token(&mon_token); // Lire le premier token avant de consommer le début du document
    if (mon_token.l_etiquette != debut_doc)
    {
        fprintf(stderr, "Erreur : balise d'ouverture du document manquante\n");
        exit(EXIT_FAILURE);
    }
    consommer(debut_doc);
    printf("Après avoir consommé la balise de début de document\n");
    contenu();
    printf("Après avoir appelé la fonction contenu\n");
    consommer(fin_doc);
    printf("Après avoir consommé la balise de fin de document\n");
}

void annexes()
{
    printf("passe par annexes\n");
    while (mon_caractere != EOF)
    {
        consommer(debut_annexe);
        contenu();
        consommer(fin_annexe);
    }
}

void contenu()
{
    printf("passe par contenu\n");
    passer_espace(); // Ajout : passer les espaces, les sauts de ligne et les tabulations
    if (mon_token.l_etiquette == debut_section)
    {
        section();
    }
    else if (mon_token.l_etiquette == debut_titre)
    {
        titre();
    }
    else if (mon_token.l_etiquette == debut_liste)
    {
        liste();
    }
    else if (mon_token.l_etiquette == mot_enrichi)
    {
        mot_riche();
    }
    else
    {
        mot_simple();
    }
    lire_token(&mon_token);
}

void section()
{
    printf("passe par section\n");
    consommer(debut_section);
    contenu();
    consommer(fin_section);
}

void titre()
{
    printf("passe par titre\n");
    consommer(debut_titre);
    texte();
    consommer(fin_titre);
}

void liste()
{
    printf("passe par liste\n");
    consommer(debut_liste);
    item();
    consommer(fin_liste);
}
void item()
{
    printf("passe par item\n");
    consommer(debut_item);
    lire_token(&mon_token); // Lire le token avant de consommer la balise
    if (mon_token.l_etiquette == item_liste)
    {
        liste_texte();
    }
    else if (mon_token.l_etiquette == item_texte)
    {
        texte_liste();
    }
    consommer(fin_item);
}

void liste_texte()
{
    printf("passe par liste texte\n");
    // Traiter la liste de mots enrichis
    mot_riche();
    lire_token(&mon_token); // Lire le token suivant
    if (mon_token.l_etiquette == item_texte)
    {
        liste_texte(); // Appel récursif pour le prochain mot enrichi
    }
}

void texte_liste()
{
    printf("passe par texte liste\n");
    // Traiter le texte suivi d'une liste de mots enrichis
    texte();
    if (mon_token.l_etiquette == mot_enrichi)
    {
        liste_texte(); // Appel la fonction pour traiter la liste de mots enrichis
    }
}

void texte()
{
    printf("passe par texte\n");
    mot_riche();
}

void mot_riche()
{
    printf("passe par mot riche\n");
    // Ne pas lire un nouveau token ici
    if (mon_token.l_etiquette == debut_important)
    {
        mot_important();
    }
    else
    {
        mot_simple();
    }
    // Lire un nouveau token après avoir traité le mot riche
    lire_token(&mon_token);
}

void mot_important()
{
    printf("passe par mot important\n");
    consommer(debut_important);
    mot_simple();
    consommer(fin_important);
}

void mot_simple()
{
    printf("passe par mot simple\n");
    lire_token(&mon_token);
}