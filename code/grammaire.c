#include <stdio.h>
#include <stdlib.h>
#include "grammaire.h"

FILE *fichier;
char caractere;

// Méthode permettant l'analyse d'un fichier
void amorcer()
{
    fichier = fopen("exemple4.txt", "r");
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
}

// Méthode permettant de lire un caractère
void lire_caractere()
{
    do
    {
        caractere = fgetc(fichier);
    } while (caractere == ' ' || caractere == '\n' || caractere == '\t' || caractere == '\r');

    if (caractere == EOF)
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
        printf("%c", caractere);
    }
}

void consommer(char terminal)
{
    if (caractere != terminal && caractere != '\n' && caractere != '\r' && caractere != '\t')
    {
        if (caractere == EOF)
        {
            // Si nous atteignons la fin du fichier, ne pas afficher de message d'erreur
            return;
        }
        fprintf(stderr, "Erreur : caractère attendu : %c, caractère trouvé : %c\n", terminal, caractere);
        exit(1); // Quitter le programme en cas d'erreur
    }
    lire_caractere(); // Consommer le caractère terminal
}

void passer_espace()
{
    while (caractere == ' ' || caractere == '\n' || caractere == '\t' || caractere == '\r')
    {
        lire_caractere();
        if (caractere == EOF)
        {
            // Si nous atteignons la fin du fichier, ne pas continuer
            return;
        }
    }
}
void text_enrichi()
{
    document();       // Lire le document principal
    annexes();        // Lire les annexes
    lire_caractere(); // Lire le caractère suivant après les annexes
}

void document()
{
    consommer('d'); // Consommer le début du document
    contenu();      // lire le contenu du document
    consommer('f'); // Consommer la fin du document
}

void annexes()
{
    while (caractere != EOF && caractere == 'd')
    {
        consommer('d');
        contenu_annexe();
        consommer('f');
        lire_caractere(); // Assurez-vous de lire le caractère suivant après chaque itération de la boucle
    }
}

void contenu_annexe()
{
    while (caractere != '<' && caractere != EOF)
    {
        if (caractere == 'd')
        {
            section();
        }
        else if (caractere == 't')
        {
            titre();
        }
        else if (caractere == 'r')
        {
            mot_enrichi();
        }
        else
        {
            liste();
        }
        lire_caractere(); // Mettre à jour le caractère suivant après chaque itération de la boucle
    }
}

void contenu()
{
    while (caractere != 'd' && caractere != 'f')
    {
        if (caractere == 'd')
        {
            section();
        }
        else if (caractere == 't')
        {
            titre();
        }
        else if (caractere == 'r')
        {
            mot_enrichi();
        }
        else
        {
            liste();
        }
        passer_espace(); // Assurez-vous de passer les espaces après chaque consommation de caractère
    }
}

void section()
{
    consommer('d'); // Consommer le début de la section
    contenu();      // Lire le contenu de la section
    consommer('f'); // Consommer la fin de la section
}

void titre()
{
    consommer('d'); // Consommer le début du titre
    texte();        // Lire le texte du titre
    consommer('f'); // Consommer la fin du titre
}

void liste()
{
    consommer('d'); // Consommer le début de la liste
    while (caractere != 'f')
    {           // Tant que le prochain caractère n'est pas la fin de la liste
        item(); // Lire et vérifier la structure d'un item de la liste
    }
    consommer('f'); // Consommer la fin de la liste
}

void item()
{
    consommer('d'); // Consommer le début de l'item
    if (caractere == 'd' || caractere == 't' || caractere == 'r')
    {
        liste_texte();
    }
    else
    {
        texte_liste();
    }
    consommer('f');   // Consommer la fin de l'item
    lire_caractere(); // Ajout de la lecture du caractère suivant
}

void liste_texte()
{
    if (caractere == 'd' || caractere == 't' || caractere == 'r')
    {                  // Si le prochain caractère est le début d'une liste, d'un titre ou un retour à la ligne
        texte();       // Lire et vérifier la structure du texte
        liste_texte(); // Lire le reste du texte de manière récursive
    }
}

void texte_liste()
{
    if (caractere == 'd' || caractere == 't' || caractere == 'r')
    {
        liste();
    }
    lire_caractere(); // Ajout de la lecture du caractère suivant
}

void texte()
{
    while (caractere != 'd' && caractere != 't' && caractere != 'r' && caractere != 'f')
    {
        mot_enrichi();
    }
}

void mot_enrichi()
{
    if (caractere == 'd' || caractere == 't' || caractere == 'r' || caractere == '\n' || caractere == '\r' || caractere == '\t')
    {
        consommer(caractere);
    }
    else
    {
        mot_simple();
    }
}

void mot_simple()
{
    if (caractere == '\n' || caractere == '\r')
    {
        consommer(caractere);
    }
    else
    {
        consommer(caractere);
    }
}

void mot_important()
{
    consommer('d'); // Consommer le début du mot important
    while (caractere != 'f')
    {                 // Tant que le prochain caractère n'est pas la fin du mot important
        mot_simple(); // Lire et vérifier la structure du mot simple
    }
    consommer('f'); // Consommer la fin du mot important
}
