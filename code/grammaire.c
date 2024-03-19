#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammaire.h"

FILE *fichier;
char caractere;
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
}

// lire un caractère
void lire_caractere()
{
    caractere = fgetc(fichier);
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
    if (caractere == EOF)
    {
        // Si nous atteignons la fin du fichier, ne pas afficher de message d'erreur
        return;
    }
    if (caractere != terminal)
    {
        fprintf(stderr, "Erreur : caractère attendu : %c, caractère trouvé : %c\n", terminal, caractere);
        exit(1);
    }
    lire_caractere(); // Consommer le caractère terminal
}

void passer_espace()
{
    if (caractere == EOF)
    {
        // Si nous atteignons la fin du fichier, ne pas continuer
        return;
    }
    while (caractere == ' ' || caractere == '\n' || caractere == '\t' || caractere == '\r')
    {
        lire_caractere();
    }
}

// OK

void text_enrichi()
{
    document(); // Lire le document principal
    annexes();  // Lire les annexes
}

void document()
{
    // Vérifier si la balise d'ouverture du document est présente
    if (caractere == '<')
    {
        lire_caractere(); // Avancer au prochain caractère pour identifier la balise
        if (caractere == 'd')
        {
            // Si la balise d'ouverture du document est trouvée, la consommer
            consommer('d');
            consommer('o');
            consommer('c');
            consommer('u');
            consommer('m');
            consommer('e');
            consommer('n');
            consommer('t');
            consommer('>');
            mon_token.l_etiquette = debut_doc;
            mon_token.la_valeur[0] = '\0';

            // Lire et analyser le contenu du document
            while (caractere != EOF)
            {
                // Vérifier si on rencontre la balise de fin du document
                if (caractere == '<')
                {
                    lire_caractere(); // Avancer au prochain caractère pour identifier la balise suivante
                    if (caractere == '/')
                    {
                        // Consommer la balise de fin du document
                        consommer('/');
                        if (caractere == 'd')
                        {
                            consommer('d');
                            consommer('o');
                            consommer('c');
                            consommer('u');
                            consommer('m');
                            consommer('e');
                            consommer('n');
                            consommer('t');
                            consommer('>');
                            mon_token.l_etiquette = fin_doc;
                            mon_token.la_valeur[0] = '\0';
                            return; // Sortir de la fonction une fois que la balise de fin du document est trouvée
                        }
                        else
                        {
                            fprintf(stderr, "Erreur : balise de fin incorrecte pour <document>\n");
                            exit(1);
                        }
                    }
                    else
                    {
                        // Si ce n'est pas la balise de fin du document, étiqueter le contenu
                        contenu();
                    }
                }
                else
                {
                    // Si le caractère n'est pas '<', il fait partie du contenu du document
                    contenu();
                }
            }

            // Si nous atteignons la fin du fichier sans rencontrer la balise de fin du document
            fprintf(stderr, "Erreur : balise de fin manquante pour <document>\n");
            exit(1);
        }
        else
        {
            fprintf(stderr, "Erreur : balise incorrecte pour <document>\n");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "Erreur : balise de début manquante pour <document>\n");
        exit(1);
    }
}

void annexes()
{
    // Lire et analyser les annexes jusqu'à la fin du fichier
    while (caractere != EOF)
    {
        // Vérifier si la balise d'ouverture d'une annexe est présente
        if (caractere == '<')
        {
            lire_caractere(); // Avancer au prochain caractère pour identifier la balise
            if (caractere == 'a')
            {
                // Si la balise d'ouverture d'une annexe est trouvée, la consommer
                consommer('a');
                consommer('n');
                consommer('n');
                consommer('e');
                consommer('x');
                consommer('e');
                consommer('>');
                mon_token.l_etiquette = debut_annexe;
                mon_token.la_valeur[0] = '\0';

                // Lire et analyser le contenu de l'annexe
                contenu();

                // Vérifier si on rencontre la balise de fin de l'annexe
                consommer('<');
                if (caractere == '/')
                {
                    // Consommer la balise de fin de l'annexe
                    consommer('/');
                    if (caractere == 'a')
                    {
                        consommer('a');
                        consommer('n');
                        consommer('n');
                        consommer('e');
                        consommer('x');
                        consommer('e');
                        consommer('>');
                        mon_token.l_etiquette = fin_annexe;
                        mon_token.la_valeur[0] = '\0';
                    }
                    else
                    {
                        fprintf(stderr, "Erreur : balise de fin incorrecte pour <annexe>\n");
                        exit(1);
                    }
                }
                else
                {
                    fprintf(stderr, "Erreur : balise de fin manquante pour <annexe>\n");
                    exit(1);
                }
            }
            else
            {
                fprintf(stderr, "Erreur : balise incorrecte pour <annexe>\n");
                exit(1);
            }
        }
        else
        {
            // Si le caractère n'est pas '<', il fait partie du contenu de l'annexe
            contenu();
        }
    }
}

void contenu()
{
    // Passer les espaces
    passer_espace();

    // Vérifier s'il y a une balise
    if (caractere == '<')
    {
        lire_caractere(); // Avancer au prochain caractère pour identifier la balise

        // Vérifier s'il s'agit de la balise de fin d'un élément
        if (caractere == '/')
        {
            lire_caractere(); // Avancer au prochain caractère pour identifier la balise de fin
            if (caractere == 'd')
            {
                // Balise de fin du document
                consommer('d');
                consommer('o');
                consommer('c');
                consommer('u');
                consommer('m');
                consommer('e');
                consommer('n');
                consommer('t');
                consommer('>');
                mon_token.l_etiquette = fin_doc;
                mon_token.la_valeur[0] = '\0';
                return; // Sortir de la fonction contenu() car le document est terminé
            }
            else if (caractere == 's')
            {
                // Balise de fin de section
                consommer('s');
                consommer('e');
                consommer('c');
                consommer('t');
                consommer('i');
                consommer('o');
                consommer('n');
                consommer('>');
                mon_token.l_etiquette = fin_section;
                mon_token.la_valeur[0] = '\0';
                return; // Sortir de la fonction contenu() car la section est terminée
            }
            else
            {
                fprintf(stderr, "Erreur : balise de fin inattendue\n");
                exit(1);
            }
        }
        else
        {
            // Si ce n'est pas une balise de fin, identifier le contenu
            if (caractere == 's')
            {
                section(); // Lire et étiqueter une section
            }
            else if (caractere == 't')
            {
                titre(); // Lire et étiqueter un titre
            }
            else if (caractere == 'l')
            {
                liste(); // Lire et étiqueter une liste
            }
            else if (strncmp(mon_token.la_valeur, "<strong>", strlen("<strong>")) == 0)
            {
                mot_important(); // Lire et étiqueter un mot important
            }
            else
            {
                mot(); // Lire et étiqueter un mot enrichi
            }
        }
    }
    else
    {
        // Si le caractère n'est pas '<', il fait partie du contenu
        texte(); // Lire et étiqueter du texte
    }
}

void section()
{
    // Balise de début de section
    consommer('s');
    consommer('e');
    consommer('c');
    consommer('t');
    consommer('i');
    consommer('o');
    consommer('n');
    consommer('>');
    mon_token.l_etiquette = debut_section;
    mon_token.la_valeur[0] = '\0';

    contenu(); // Lire et étiqueter le contenu de la section

    // Balise de fin de section
    consommer('<');
    consommer('/');
    consommer('s');
    consommer('e');
    consommer('c');
    consommer('t');
    consommer('i');
    consommer('o');
    consommer('n');
    consommer('>');
    mon_token.l_etiquette = fin_section;
    mon_token.la_valeur[0] = '\0';
}

void titre()
{
    // Balise de début de titre
    consommer('t');
    consommer('i');
    consommer('t');
    consommer('r');
    consommer('e');
    consommer('>');
    mon_token.l_etiquette = debut_titre;
    mon_token.la_valeur[0] = '\0';

    texte(); // Lire et étiqueter le contenu du titre

    // Balise de fin de titre
    consommer('<');
    consommer('/');
    consommer('t');
    consommer('i');
    consommer('t');
    consommer('r');
    consommer('e');
    consommer('>');
    mon_token.l_etiquette = fin_titre;
    mon_token.la_valeur[0] = '\0';
}

void liste()
{
    // Balise de début de liste
    consommer('l');
    consommer('i');
    consommer('s');
    consommer('t');
    consommer('e');
    consommer('>');
    mon_token.l_etiquette = debut_liste;
    mon_token.la_valeur[0] = '\0';

    texte();

    // Balise de fin de liste
    consommer('<');
    consommer('/');
    consommer('l');
    consommer('i');
    consommer('s');
    consommer('t');
    consommer('e');
    consommer('>');
    mon_token.l_etiquette = fin_liste;
    mon_token.la_valeur[0] = '\0';
}

void liste_texte()
{
    if (caractere == 'd' || caractere == 't' || caractere == 'r' || caractere == '\n')
    {                  // Si le prochain caractère est le début d'une liste, d'un titre, un retour à la ligne
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
    lire_caractere();
}

void texte()
{
    char mot[51];  // Variable pour stocker le mot enrichi en cours
    int index = 0; // Index pour parcourir le mot enrichi
    mot[0] = '\0'; // Initialiser la variable

    while (caractere != EOF)
    {
        // Vérifier si le caractère actuel correspond à une balise de début de mot important
        if (caractere == '<' && strncmp(mon_token.la_valeur, "<strong>", strlen("<strong>")) == 0)
        {
            // Si oui, consommer la balise de début de mot important
            while (caractere != '>' && caractere != EOF)
            {
                lire_caractere();
            }
            // Réinitialiser le mot pour commencer à lire le prochain mot
            mot[0] = '\0';
            index = 0;
        }
        // Vérifier si le caractère actuel correspond à une balise de fin de mot important
        else if (caractere == '<' && strncmp(mon_token.la_valeur, "</strong>", strlen("</strong>")) == 0)
        {
            // Si oui, consommer la balise de fin de mot important
            while (caractere != '>' && caractere != EOF)
            {
                lire_caractere();
            }
            // Réinitialiser le mot pour commencer à lire le prochain mot
            mot[0] = '\0';
            index = 0;
        }
        // Vérifier si un espace, un retour à la ligne ou une tabulation est rencontré
        else if (caractere == ' ' || caractere == '\n' || caractere == '\t' || caractere == '\r')
        {
            // Vérifier s'il y a un mot enrichi à étiqueter
            if (index > 0)
            {
                // Terminer le mot enrichi avec un caractère nul
                mot[index] = '\0';
                // Copier le mot enrichi dans le token
                strncpy(mon_token.la_valeur, mot, 81);
                // Étiqueter le mot enrichi
                mon_token.l_etiquette = mot_enrichi;
                // Réinitialiser l'index pour le prochain mot enrichi
                index = 0;
            }
            // Passer les espaces supplémentaires
            while (caractere == ' ' || caractere == '\n' || caractere == '\t' || caractere == '\r')
            {
                lire_caractere();
            }
        }
        // Vérifier si la taille maximale du mot enrichi n'est pas atteinte
        else if (index < 50)
        {
            // Ajouter le caractère au mot enrichi et avancer
            mot[index++] = caractere;
            lire_caractere();
        }
        else
        {
            // Si la taille maximale du mot enrichi est atteinte
            fprintf(stderr, "Erreur : taille maximale du mot enrichi atteinte\n");
            exit(1);
        }
    }
}

void mot()
{
    if (strcmp(mon_token.la_valeur, "\n") == 0)
    {
        // Si c'est un retour à la ligne, ne rien faire et appeler la fonction texte
        texte();
    }
    else if (mon_token.la_valeur[0] == '<' && mon_token.la_valeur[strlen(mon_token.la_valeur) - 1] == '>')
    {
        // Si le mot commence par une balise, appeler la fonction mot important
        mot_important();
    }
    else
    {
        // Sinon, c'est un mot simple, appeler la fonction mot simple
        mon_token.l_etiquette = mot_simple;
    }
}

void mot_important()
{
    // Balise de début de mot important
    consommer('<');
    consommer('s');
    consommer('t');
    consommer('r');
    consommer('o');
    consommer('n');
    consommer('g');
    consommer('>');

    // Lire et étiqueter les mots simples entre les balises
    while (strncmp(mon_token.la_valeur, "</strong>", strlen("</strong>")) != 0)
    {
        mot(); // Lire et étiqueter un mot simple
    }

    // Balise de fin de mot important
    consommer('<');
    consommer('/');
    consommer('s');
    consommer('t');
    consommer('r');
    consommer('o');
    consommer('n');
    consommer('g');
    consommer('>');
}