#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyse.h"

// Variables globales
#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];
int buffer_index = 0;
FILE *fichier;
char mon_caractere = '\0';
char caractere_avant = '\0';
t_token token_courant;

// Tableau image des étiquettes
const char *etiq_str[] = {
    "DEBUT_DOC",
    "FIN_DOC",
    "DEBUT_ANNEXE",
    "FIN_ANNEXE",
    "DEBUT_TITRE",
    "FIN_TITRE",
    "DEBUT_SECTION",
    "FIN_SECTION", 
    "DEBUT_LISTE",
    "FIN_LISTE",
    "DEBUT_ITEM",
    "FIN_ITEM",
    "SAUT_LIGNE",
    "MOT"
};

void ouverture_fichier(char *nom_fichier)
{   
    buffer_index = 0;
    memset(buffer, 0, BUFFER_SIZE); // On met le buffer à zéro

    fichier = fopen(nom_fichier, "r");
    if (fichier == NULL)
    {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    }

    lire_caractere();
}

void terminer()
{
    printf("passe par terminer\n");
    fclose(fichier);
    exit(EXIT_SUCCESS);
}

void passer_espace(){
    if(mon_caractere == ' ' || mon_caractere == '\n' ||  mon_caractere == '\r' || mon_caractere == '\t'){
        lire_caractere();
        creation_item();
    }
    
}


void lire_caractere(){
    mon_caractere = fgetc(fichier);
    
        
}


int verification(){
    if(mon_caractere != ' ' && mon_caractere != '\n'&& mon_caractere != '\r' && mon_caractere != '\t' && mon_caractere != EOF){
        return 1;
    }
    else{
        return 0;
    }
}

void creation_item() {
    
    memset(buffer, 0, BUFFER_SIZE);
    buffer_index = 0;

    while (verification() == 1){
        buffer[buffer_index] = mon_caractere;
        buffer_index++;
        caractere_avant = mon_caractere;
        lire_caractere();
    }
    if(strlen(buffer)>1 ){
        creation_token();
        afficher_token();
    }
    
    passer_espace();

}




void creation_token() //création token
{
    // On vérifie si le token_courant.la_valeur est plein
    if(token_courant.la_valeur != NULL){
        free(token_courant.la_valeur);
        token_courant.la_valeur = NULL;
    }
    
    // Association des types de token
    if (strcmp(buffer, "<document>") == 0)
    {
        token_courant.type = DEBUT_DOC; 
    }
    else if(strcmp(buffer, "</document>") == 0)
    {
        token_courant.type = FIN_DOC;
    }
    else if(strcmp(buffer, "<annexe>") == 0){
        token_courant.type = DEBUT_ANNEXE;
    }
    else if(strcmp(buffer, "</annexe>") == 0){
        token_courant.type = FIN_ANNEXE;
    }
    else if(strcmp(buffer, "<titre>") == 0){
        token_courant.type = DEBUT_TITRE;
    }
    else if(strcmp(buffer, "</titre>") == 0){
        token_courant.type = FIN_TITRE;
    }
    else if(strcmp(buffer, "<section>") == 0){
        token_courant.type = DEBUT_SECTION;
    }
    else if(strcmp(buffer, "</section>") == 0){
        token_courant.type = FIN_SECTION;
    }
    else if(strcmp(buffer, "<liste>") == 0){
        token_courant.type = DEBUT_LISTE;
    }
    else if(strcmp(buffer, "</liste>") == 0){
        token_courant.type = FIN_LISTE;
    }
    else if(strcmp(buffer, "<item>") == 0){
        token_courant.type = DEBUT_ITEM;
    }
    else if(strcmp(buffer, "</item>") == 0){
        token_courant.type = FIN_ITEM;
    }
    else if(strcmp(buffer, "</br>") == 0){
        token_courant.type = SAUT_LIGNE;
    }
    else{
        token_courant.type = MOT;
        token_courant.la_valeur = malloc(sizeof(char) * (strlen(buffer) + 1));
        sprintf(token_courant.la_valeur, "%s", buffer);// strdup(buffer);
    }

    
}

void affichage_buffer(){
    int taille = strlen(buffer);
    for (int i = 0; i < taille; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}

void afficher_token(){
    printf("Type : %s", etiq_str[token_courant.type]);
    if(token_courant.la_valeur != NULL){
        printf("  -> Valeur : %s", token_courant.la_valeur);
    }
    printf("\n");

}




