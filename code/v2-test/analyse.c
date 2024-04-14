#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "analyse.h"

// Variables globales
#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];
int buffer_index = 0;
FILE *fichier;
char mon_caractere = '\0';
t_token token_courant;
bool fin_fichier = false;

// Tableau image des étiquettes
const char *etiq_str[] = {
    "<document>",
    "</document>",
    "<annexe>",
    "</annexe>",
    "<titre>",
    "</titre>",
    "<section>",
    "</section>",
    "<liste>",
    "</liste>",
    "<item>",
    "</item>",
    "<debut_important>",
    "</fin_important>",
    "</br>",
    "mot"    
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
    printf("FIN\n");
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
    if(mon_caractere == EOF){
        return;
    }
    
        
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
    //printf("passe par creation item\n");
    memset(buffer, 0, BUFFER_SIZE);
    buffer_index = 0;

    while (verification() == 1){
        buffer[buffer_index] = mon_caractere;
        buffer_index++;
        lire_caractere();
    }
    if(strlen(buffer)>=1){
        creation_token();
        affichage_buffer();
        
        Text_enrichi();
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
    printf("Buffer : ");
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



void consommer_token_type(t_etiq attendu){
    if (token_courant.type != attendu){
        fprintf(stderr, "Erreur : balise attendue %s\n", etiq_str[attendu]);
    }
    if(token_courant.type == attendu){
        printf("J'ai consommé %s\n", etiq_str[attendu]);
        printf("-----------\n");
        creation_item();
        if(strlen(buffer) == 0){
           
            terminer();
        }
        
    }
}




// GRAMMAIRE : 

void Text_enrichi(){
    //printf("passe par Text_enrichi\n");
    if(fin_fichier == true || token_courant.type == DEBUT_ANNEXE){
        if(strlen(buffer) == 0){
            fin_fichier = false;
        }
        Annexe();
    }
    Document();
    
    
    //exit(EXIT_SUCCESS);
}

void Document(){
   // printf("passe par document\n");
    // Consomme début document
    
    consommer_token_type(DEBUT_DOC);

    contenu();
    //Consomme fin document
    
    consommer_token_type(FIN_DOC);

}


void Annexe(){
    
    while (token_courant.type == DEBUT_ANNEXE||token_courant.type == FIN_ANNEXE){
        fin_fichier = true;
        consommer_token_type(DEBUT_ANNEXE);
        
        contenu();
        consommer_token_type(FIN_ANNEXE); 
        
        
        
    }
}

void contenu(){
    //printf("passe par contenu\n");
    while (token_courant.type == DEBUT_SECTION || token_courant.type == DEBUT_TITRE || token_courant.type == MOT || token_courant.type == DEBUT_LISTE ||token_courant.type == FIN_SECTION || token_courant.type == FIN_TITRE || token_courant.type == FIN_LISTE || token_courant.type == DEBUT_ITEM || token_courant.type == FIN_ITEM || token_courant.type == DEBUT_IMPORTANT || token_courant.type == FIN_IMPORTANT || token_courant.type == SAUT_LIGNE){                                         
        if (token_courant.type == DEBUT_SECTION || token_courant.type == FIN_SECTION){
            section();}
        else if (token_courant.type == DEBUT_TITRE || token_courant.type == FIN_TITRE){
            titre();}
        else if (token_courant.type == MOT ){
            mot_enrichi();}
        else if (token_courant.type == DEBUT_LISTE || token_courant.type == FIN_LISTE || token_courant.type == DEBUT_ITEM || token_courant.type == FIN_ITEM){
            liste();}      
    }
    
}

void section(){
    //printf("passe par section\n");
    if(token_courant.type == FIN_SECTION){
        consommer_token_type(FIN_SECTION);
        return;
    }
    consommer_token_type(DEBUT_SECTION);
    contenu();
    
}

void titre(){
    //printf("passe par titre\n");
    consommer_token_type(DEBUT_TITRE);
    texte();
    consommer_token_type(FIN_TITRE);
}

void mot_enrichi(){
    //printf("passe par mot_enrichi\n");
    if (token_courant.type == MOT){
        mot_simple();
    }
    else if (token_courant.type == DEBUT_IMPORTANT){
        mot_important();
    }
    else if (token_courant.type == SAUT_LIGNE){
        consommer_token_type(SAUT_LIGNE);
    }
}

void liste(){
    //printf("passe par liste\n");
    consommer_token_type(DEBUT_LISTE);
    while (token_courant.type == DEBUT_ITEM || token_courant.type == FIN_ITEM){
        item();
    }
    consommer_token_type(FIN_LISTE);
}

void item(){
    //printf("passe par item\n");
    consommer_token_type(DEBUT_ITEM);
    if (token_courant.type == DEBUT_LISTE)
        liste_texte();
    else if (token_courant.type == MOT)
        texte_liste();
    consommer_token_type(FIN_ITEM);
}

void liste_texte(){
    //printf("passe par liste_texte\n");
    liste();
    texte_liste();
}

void texte_liste(){
    //printf("passe par texte_liste\n");
    texte();
    liste_texte();
}

void texte(){
    //printf("passe par texte\n");
    while (token_courant.type == MOT){
        mot_enrichi();
    }
}


void mot_simple(){
    //printf("passe par mot_simple\n");
    if (token_courant.type == MOT){
        
        consommer_token_type(MOT);
    }
}

void mot_important(){
    consommer_token_type(DEBUT_IMPORTANT);
    while (token_courant.type == MOT){
        mot_simple();
            
    }
    consommer_token_type(FIN_IMPORTANT);
}