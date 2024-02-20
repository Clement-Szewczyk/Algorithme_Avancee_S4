#include <stdio.h>
#include <stdlib.h>
#include "grammaire.h"

FILE *fichier;
char caractere;

// Méthode permettant l'analyse d'un fichier
void amorcer()
{
    fichier = fopen("exemple1.txt", "r");
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
    while ((caractere = fgetc(fichier)) != EOF)
    {
        printf("%c", caractere);
    }
    fclose(fichier);
}

void consommer(char terminal)
{
    if (caractere != terminal && caractere != '\n' && caractere != '\r' && caractere != '\t')
    {
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
    }
}

void text_enrichi()
{
    document();
    annexes();
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
        consommer('d');   // Consommer le début de l'annexe
        contenu_annexe(); // Lire et vérifier la structure du contenu de l'annexe
        consommer('f');   // Consommer la fin de l'annexe
    }
}

void contenu_annexe()
{
    while (caractere != 'f')
    { // Tant que le prochain caractère n'est pas la fin de l'annexe
        if (caractere == 'd')
        {
            section(); // Vérifier si le contenu est une section
        }
        else if (caractere == 't')
        {
            titre(); // Vérifier si le contenu est un titre
        }
        else if (caractere == 'r')
        {
            mot_enrichi(); // Vérifier si le contenu est un mot enrichi
        }
        else
        {
            liste(); // Vérifier si le contenu est une liste
        }
    }
}

void contenu()
{
    while (caractere != 'd' && caractere != 'f')
    { // Tant que le caractère n' est pas le début ou la fin d'une section
        if (caractere == 'd')
        {              // Si le caractère est le début d'une section
            section(); // Lire et vérifier la structure de la section
        }
        else if (caractere == 't')
        {            // Si le caractère est le début d'un titre
            titre(); // Lire et vérifier la structure du titre
        }
        else if (caractere == 'r')
        {                  // Si le caractère est un retour à la ligne
            mot_enrichi(); // Lire et vérifier le contenu enrichi (retour à la ligne)
        }
        else
        {
            liste(); // Lire et vérifier la structure de la liste
        }
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
    {                  // Si le prochain caractère est le début d'une liste, d'un titre ou un retour à la ligne
        liste_texte(); // Lire et vérifier la structure du texte de l'item
    }
    else
    {
        texte_liste(); // Lire et vérifier la structure de la liste de l'item
    }
    consommer('f'); // Consommer la fin de l'item
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
    {            // Si le prochain caractère est le début d'une liste, d'un titre ou un retour à la ligne
        liste(); // Lire et vérifier la structure de la liste
    }
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
