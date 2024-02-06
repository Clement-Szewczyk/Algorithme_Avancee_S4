/*
cd Algorithme_Avancee_S4/code/
make
./grammaire
*/

#include "grammaire.h"
#include <stdio.h>
#include <stdlib.h>

char mon_caractere;

int main()
{
    FILE *mon_fichier = fopen("exemple1.txt", "r");

    if (mon_fichier == NULL)
    {
        printf("Erreur d'ouverture du fichier\n");
        exit(1);
    }

    while ((mon_caractere = fgetc(mon_fichier)) != EOF)
    {
        printf("%c", mon_caractere);
    }

    fclose(mon_fichier);

    return 0;
}
