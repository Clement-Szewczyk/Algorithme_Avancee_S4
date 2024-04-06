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
    char *nom_fichier = "exemple1.txt";
    amorcer(nom_fichier);
    text_enrichi();
    terminer();
    return 0;
}
