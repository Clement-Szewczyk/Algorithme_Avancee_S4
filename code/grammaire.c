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
    lire_token();
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
        lire_caractere(); // Lire le prochain caractère pour identifier le token

        // Identifier et mettre à jour le token en fonction du caractère courant
        switch (mon_caractere)
        {
        case 'd':
            mon_token.l_etiquette = debut_doc; // Balise <document>
            break;
        case 's':
            lire_caractere();
            if (mon_caractere == 'e')
            {
                mon_token.l_etiquette = debut_section; // Balise <section>
            }
            else
            {
                mon_token.l_etiquette = debut_important; // Balise <strong>
            }
            break;
        case 'a':
            mon_token.l_etiquette = debut_annexe; // Balise <annexe>
            break;
        case 't':
            mon_token.l_etiquette = debut_titre; // Balise <titre>
            break;
        case 'l':
            mon_token.l_etiquette = debut_liste; // Balise <liste>
            break;
        case 'i':
            mon_token.l_etiquette = debut_item; // Balise <item>
            break;
        case '/':
            lire_caractere();
            // Cas des balises de fermeture
            switch (mon_caractere)
            {
            case 'd':
                mon_token.l_etiquette = fin_doc; // Balise </document>
                break;
            case 's':
                lire_caractere();
                if (mon_caractere == 'e')
                {
                    mon_token.l_etiquette = fin_section; // Balise </section>
                }
                else
                {
                    mon_token.l_etiquette = fin_important; // Balise </strong>
                }
                break;
            case 'a':
                mon_token.l_etiquette = fin_annexe; // Balise </annexe>
                break;
            case 't':
                mon_token.l_etiquette = fin_titre; // Balise </titre>
                break;
            case 'l':
                mon_token.l_etiquette = fin_liste; // Balise </liste>
                break;
            case 'i':
                mon_token.l_etiquette = fin_item; // Balise </item>
                break;
            default:
                // Autres cas à traiter...
                break;
            }
            lire_token(); // Ajout de cet appel pour avancer au prochain token après avoir identifié la balise de fermeture
            break;
        default:
            mon_token.l_etiquette = mot_enrichi;
            break;
        }
    }

    // Lire le prochain caractère pour avancer dans le fichier
    lire_caractere();
}

void consommer(t_etiq terminal)
{
    if (mon_caractere == EOF)
    {
        // Si nous atteignons la fin du fichier, ne pas afficher de message d'erreur
        return;
    }
    if (mon_token.l_etiquette != terminal)
    {
        fprintf(stderr, "Erreur : caractère attendu : %d, caractère trouvé : %d\n", terminal, mon_token.l_etiquette);
        exit(1);
    }
    lire_token(); // Consommer le caractère terminal
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
    if (mon_token.l_etiquette != debut_doc)
    {
        fprintf(stderr, "Erreur : balise d'ouverture du document manquante\n");
        exit(1);
    }
    consommer(debut_doc);
    contenu();
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

    consommer(debut_item);
    if (mon_token.l_etiquette == item_liste)
    {
        liste_texte();
    }
    if (mon_token.l_etiquette == item_texte)
    {
        texte_liste();
    }
    consommer(fin_item);
}

void liste_texte()
{

    liste();
    texte_liste();
}

void texte_liste()
{

    texte();
    liste_texte();
}

void texte()
{
    mot_riche();
}

void mot_riche()
{

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
}