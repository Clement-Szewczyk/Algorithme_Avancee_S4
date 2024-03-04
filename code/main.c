/*
cd Algorithme_Avancee_S4/code/
make
./grammaire
*/

#include "grammaire.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    amorcer(); // Test de la fonction amorcer() pour ouvrir et lire le fichier
    printf("\nTest de la fonction lire_caractere() :\n");
    lire_caractere(); // Test de la fonction lire_caractere() pour lire un caractère à la fois

    printf("\nTest de la fonction consommer() :\n");
    consommer('d'); // Test de la fonction consommer() avec un caractère attendu 'd'

    printf("\nTest de la fonction passer_espace() :\n");
    passer_espace(); // Test de la fonction passer_espace() pour ignorer les espaces, tabulations et retours à la ligne

    // PROBLEME ICI
    printf("\nTest de la fonction text_enrichi() :\n");
    text_enrichi(); // Test de la fonction text_enrichi()

    printf("\nTest de la fonction document() :\n");
    document(); // Test de la fonction document()

    printf("\nTest de la fonction annexes() :\n");
    annexes(); // Test de la fonction annexes()

    printf("\nTest de la fonction contenu_annexe() :\n");
    contenu_annexe(); // Test de la fonction contenu_annexe()

    printf("\nTest de la fonction contenu() :\n");
    contenu(); // Test de la fonction contenu()

    printf("\nTest de la fonction section() :\n");
    section(); // Test de la fonction section()

    printf("\nTest de la fonction titre() :\n");
    titre(); // Test de la fonction titre()

    printf("\nTest de la fonction liste() :\n");
    liste(); // Test de la fonction liste()

    printf("\nTest de la fonction item() :\n");
    item(); // Test de la fonction item()

    printf("\nTest de la fonction liste_texte() :\n");
    liste_texte(); // Test de la fonction liste_texte()

    printf("\nTest de la fonction texte_liste() :\n");
    texte_liste(); // Test de la fonction texte_liste()

    printf("\nTest de la fonction texte() :\n");
    texte(); // Test de la fonction texte()

    printf("\nTest de la fonction mot_enrichi() :\n");
    mot_enrichi(); // Test de la fonction mot_enrichi()

    printf("\nTest de la fonction mot_simple() :\n");
    mot_simple(); // Test de la fonction mot_simple()

    printf("\nTest de la fonction mot_important() :\n");
    mot_important(); // Test de la fonction mot_important()

    return 0;
}
