#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammaire.h"

FILE *fichier;
char mon_caractere;
t_token mon_token;

// analyse du fichier
void amorcer(char *nom_fichier)
{
    fichier = fopen(nom_fichier, "r");
    if (fichier == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier\n");
        exit(-1);
    }
    else
    {
        printf("Fichier ouvert avec succès\n");
    }
    lire_caractere(); // Commencer la lecture du fichier
    lire_token();     // Mettre à jour le premier token
}

// lire un caractère
void lire_caractere()
{
    mon_caractere = fgetc(fichier);
    if (mon_caractere == EOF)
    {
        if (feof(fichier))
        {
            printf("Fin du fichier atteinte\n");
            fclose(fichier);
        }
        else
        {
            fprintf(stderr, "Erreur : erreur lors de la lecture du fichier\n");
            exit(-1);
        }
    }
    else
    {
        printf("%c", mon_caractere);
    }
}

void lire_token()
{
    passer_espace(); // Passer les espaces, les retours à la ligne, etc.

    // Vérifier si nous avons atteint la fin du fichier
    if (mon_caractere == EOF)
    {
        return; // Fin du fichier, rien à faire
    }

    if (mon_caractere == '<')
    {
        // Lire la balise complète jusqu'au '>'
        char balise[81];
        int i = 0;
        lire_caractere(); // Passer '<'
        while (mon_caractere != '>')
        {
            balise[i++] = mon_caractere;
            lire_caractere();
        }
        balise[i] = '\0';                    // Ajouter le caractère de fin de chaîne
        printf("Balise lue : %s\n", balise); // Débogage : afficher la balise lue

        // Identifier et mettre à jour le token en fonction de la balise lue
        if (balise[0] == '/')
        {
            // Balise de fermeture
            if (strcmp(&balise[1], "document") == 0)
            {
                mon_token.l_etiquette = fin_doc;
            }
            else if (strcmp(&balise[1], "section") == 0)
            {
                mon_token.l_etiquette = fin_section;
            }
            else if (strcmp(&balise[1], "annexe") == 0)
            {
                mon_token.l_etiquette = fin_annexe;
            }
            else if (strcmp(&balise[1], "titre") == 0)
            {
                mon_token.l_etiquette = fin_titre;
            }
            else if (strcmp(&balise[1], "liste") == 0)
            {
                mon_token.l_etiquette = fin_liste;
            }
            else if (strcmp(&balise[1], "item") == 0)
            {
                mon_token.l_etiquette = fin_item;
            }
            else if (strcmp(&balise[1], "strong") == 0)
            {
                mon_token.l_etiquette = fin_important;
            }
            else
            {
                mon_token.l_etiquette = mot_enrichi; // Balise non reconnue, traitée comme un mot enrichi
            }
        }
        else
        {
            // Balise d'ouverture
            if (strcmp(balise, "document") == 0)
            {
                mon_token.l_etiquette = debut_doc;
            }
            else if (strcmp(balise, "section") == 0)
            {
                mon_token.l_etiquette = debut_section;
            }
            else if (strcmp(balise, "annexe") == 0)
            {
                mon_token.l_etiquette = debut_annexe;
            }
            else if (strcmp(balise, "titre") == 0)
            {
                mon_token.l_etiquette = debut_titre;
            }
            else if (strcmp(balise, "liste") == 0)
            {
                mon_token.l_etiquette = debut_liste;
            }
            else if (strcmp(balise, "item") == 0)
            {
                mon_token.l_etiquette = debut_item;
            }
            else if (strcmp(balise, "strong") == 0)
            {
                mon_token.l_etiquette = debut_important;
            }
            else
            {
                mon_token.l_etiquette = mot_enrichi; // Balise non reconnue, traitée comme un mot enrichi
            }
        }

        printf("Étiquette de mon_token après lecture : %d\n", mon_token.l_etiquette); // Débogage : afficher l'étiquette mise à jour
    }
}

void consommer()
{
    if (mon_caractere == EOF)
    {
        // Si nous atteignons la fin du fichier, ne pas afficher de message d'erreur
        return;
    }
    lire_token(); // Mettre à jour le token avant de vérifier le jeton terminal
}

void passer_espace()
{
    if (mon_caractere == EOF)
    {
        // Si nous atteignons la fin du fichier, ne pas continuer
        return;
    }
    while (mon_caractere == ' ' || mon_caractere == '\n' || mon_caractere == '\t' || mon_caractere == '\r')
    {
        lire_caractere();
    }
}

// Analyse

void text_enrichi()
{
    document(); // Lire le document principal
    annexes();  // Lire les annexes
}

void document()
{
    // Vérifier la présence de la balise d'ouverture du document
    printf("Étiquette de mon_token : %d\n", mon_token.l_etiquette); // Débogage : afficher l'étiquette stockée dans mon_token
    if (mon_token.l_etiquette != debut_doc)
    {
        fprintf(stderr, "Erreur : balise d'ouverture du document manquante\n");
        exit(1);
    }
    consommer(debut_doc);

    contenu(); // Traiter le contenu du document

    consommer(fin_doc);
}

void annexes()
{
    while (mon_caractere != EOF)
    {
        consommer(debut_annexe);
        contenu();
        consommer(fin_annexe);
    }
}

void contenu()
{
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
    else
    {

        mot_riche();
    }

    lire_caractere(); // Consommer le caractère après avoir traité le contenu
}

void section()
{

    consommer(debut_section);
    contenu();
    consommer(fin_section);
}

void titre()
{

    consommer(debut_titre);
    texte();
    consommer(fin_titre);
}

void liste()
{

    consommer(debut_liste);
    item();
    consommer(fin_liste);
}

void item()
{
    lire_token(); // Mettre à jour le token avant de commencer le traitement
    consommer(debut_item);
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
    // Traiter la liste de mots enrichis
    mot_riche();
    if (mon_token.l_etiquette == mot_enrichi)
    {
        liste_texte(); // Appel récursif pour le prochain mot enrichi
    }
}

void texte_liste()
{
    // Traiter le texte suivi d'une liste de mots enrichis
    texte();
    if (mon_token.l_etiquette == mot_enrichi)
    {
        liste_texte(); // Appel la fonction pour traiter la liste de mots enrichis
    }
}

void texte()
{
    mot_riche();
}

void mot_riche()
{
    printf("passe ici");

    if (mon_token.l_etiquette == debut_important)
    {
        mot_important();
    }

    else
    {
        mot_simple();
    }
    lire_token();
}

void mot_important()
{
    consommer(debut_important);
    mot_simple();
    consommer(fin_important);
}

void mot_simple()
{
    lire_token();
}