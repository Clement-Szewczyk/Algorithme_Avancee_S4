/*
cd Algorithme_Avancee_S4/code/
make
./grammaire
*/

#include <stdio.h>
#include <stdlib.h>
#include "grammaire.h"

int main()
{
    char *nom_fichier = "exemple1.txt"; // Nom du fichier à tester
    amorcer(nom_fichier);               // Appel de la fonction amorcer avec le nom du fichier
    text_enrichi();

    return 0;
}
