/*
cd Algorithme_Avancee_S4/code/
make
./grammaire
*/

#include <stdio.h>
#include <stdlib.h>
#include "grammaire.h"

// Fonction de test pour la fonction amorcer()
void test_amorcer()
{
    amorcer();
}

// Fonction de test pour la fonction lire_caractere()
void test_lire_caractere()
{
    printf("\nTest de la fonction lire_caractere() :\n");
    lire_caractere();
}

// Fonction de test pour la fonction consommer()
void test_consommer()
{
    printf("\nTest de la fonction consommer() :\n");
    consommer('d');
}

// Fonction de test pour la fonction passer_espace()
void test_passer_espace()
{
    printf("\nTest de la fonction passer_espace() :\n");
    passer_espace();
}

// Fonction de test pour la fonction text_enrichi()
void test_text_enrichi()
{
    printf("\nTest de la fonction text_enrichi() :\n");
    text_enrichi();
}

// Fonction de test pour la fonction document()
void test_document()
{
    printf("\nTest de la fonction document() :\n");
    document();
}

// Fonction de test pour la fonction contenu()
void test_contenu()
{
    printf("\nTest de la fonction contenu() :\n");
    contenu();
}

// Fonction de test pour la fonction section()
void test_section()
{
    printf("\nTest de la fonction section() :\n");
    section();
}

// Fonction de test pour la fonction titre()
void test_titre()
{
    printf("\nTest de la fonction titre() :\n");
    titre();
}

// Fonction de test pour la fonction liste()
void test_liste()
{
    printf("\nTest de la fonction liste() :\n");
    liste();
}

// Fonction de test pour la fonction texte()
void test_texte()
{
    printf("\nTest de la fonction texte() :\n");
    texte();
}

// Fonction de test pour la fonction mot_important()
void test_mot_important()
{
    printf("\nTest de la fonction mot_important() :\n");
    mot_important();
}

// Fonction principale de test
int main()
{
    test_amorcer();
    test_lire_caractere();
    test_consommer();
    test_passer_espace();
    test_text_enrichi();
    test_document();
    test_contenu();
    test_section();
    test_titre();
    test_liste();
    test_texte();
    test_mot_important();

    return 0;
}
