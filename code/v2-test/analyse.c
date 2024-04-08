#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyse.h"

// Variables globales
#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];
int buffer_index = 0;
FILE *fichier;
char mon_caractere = ' ';


void ouverture_fichier(char *nom_fichier)
{   
    //printf("passe par amorcer\n");
    fichier = fopen(nom_fichier, "r");
    if (fichier == NULL)
    {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    }
    creation_item();
}

void terminer()
{
    //printf("passe par terminer\n");
    fclose(fichier);
    printf("ADIEU\n");
    
    exit(EXIT_SUCCESS);
}

void passer_espace(){
    //Passe les espaces
    //printf("passe par passer espace\n");
    creation_item();
    
}


void lire_caractere(){
    //Lit un caractère du fichier
    //printf("passe par lire caractere\n");
    //printf("%c", fgetc(fichier));
    mon_caractere = fgetc(fichier);
    //printf("Lecture du caractère : %c\n", mon_caractere);

    if(mon_caractere == EOF)
    {   
        
        //affichage_buffer();
        creation_token();
        printf("\n");
        terminer();
    }
    else{
        //mon_caractere = fgetc(fichier);
        creation_item();
        //printf("Lecture du caractère : %c\n", mon_caractere);
    }
    
}

void creation_item(){
    
    //Création d'un mot
    if(mon_caractere == ' ' || mon_caractere == '\n' || mon_caractere == '\t' ){//New mot
        //affichage_buffer();
        printf("\n");
        creation_token();
        //printf("\n");
        buffer_index = 0;
        //printf("Nouveau mot\n");
        memset(buffer, 0, BUFFER_SIZE);
        buffer[buffer_index] = mon_caractere;
        buffer_index++;
        lire_caractere();
    }
    else{
        
        buffer[buffer_index] = mon_caractere;
        buffer_index++;
        lire_caractere();
    }   
}


void creation_token(){
    if(buffer[1] == '<'){
        printf("Token : %s\n", buffer);
    }
    else{
        printf("mot : %s\n", buffer);
    }
   
}

void affichage_buffer(){
    //printf("affichage buffer\n");
    //Affiche le buffer
    //printf("passe par affichage buffer\n");
    //printf("Buffer : %s\n", buffer);
    
    for(int i = 0; i < BUFFER_SIZE; i++){
        //printf("%d", i);
        printf("%c", buffer[i]);
    }
   
}