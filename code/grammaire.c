#include <stdio.h>
#include <stdlib.h>
#include "grammaire.h"

// Méthode permettant l'analyse d'un fichier
void amorcer(){
    fichier = fopen("fichier.txt", "r");
    if (fichier == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier\n");
        exit(-1);
    }else{
        printf("Fichier ouvert avec succès\n");
    }
    printf("FIn Amorce\n");
    lire_caractere();
}

// Méthode permettant de lire un caractère
void lire_caractere(){
    //printf("Lecture\n");
    caractere = fgetc(fichier);
    printf("%c\n", caractere);
    passer_espace();
    
}   

void consommer(char terminal){
    printf("Consommer\n");
    if(caractere != terminal){
        printf("Erreur : caractère attendu : %c, caractère trouvé : %c", terminal, caractere);
        // message erreur
        exit(1);
    }
    lire_caractere();
}

void passer_espace(){
    
    while(caractere == ' ' || caractere == '\n' || caractere == '\t'|| caractere == '\r'){
        printf("Passe espace\n");
        lire_caractere();
    }
}

void text_enrichi(){
    document();
    annexes();
}

void annexes(){
    while(){
        // Doit consommer début annexe
        contenu();
        // Doit consommer fin annexe
    }
}

void document(){
    // Doit consommer début doc
    contenu();
    // Doit consommer fin de doc
}

void contenu(){
    if(/*section*/){
        section();
    }else if(/*titre*/){
        titre();
    }else if(/*mot_enrichi*/){
        mot_enrichi();
    }
    else{
        liste();
    }
}

void section(){
    // Doit consommer début de section
    contenu();
    // Doit consommer fin de section
}

void titre(){
    // Doit consommer début titre
    texte();
    // Doit consommer fin titre
}

void liste(){
    // Doit consommer début de liste
    while(){
        item()
    }
    // Doit consommer fin de liste
}

void item(){
    // Début item
    while(){
        if(){
            liste_texte()
        }else{
            texte_liste()
        }
    }
}

void liste_texte(){
    if(/*Pas vide*/){
        liste();
        texte_liste();
    }
    else{
        return 0;
    }

}

void texte_liste(){
    if(/*Pas vide*/){
        texte();
        liste_texte();
    }else{
        return 0;
    }
}

void texte(){
    while(){
        mot_enrichi();
    }
}

void mot_enrichi(){
    if(){
        mot_simple()
    }else if(){
        mot_important();
    }else{
        consommer("\n\r");
    }
}

void mot_important(){
    // DOit consommer début important
    while(){
        mot_simple();
    }
    // Doit consommer fin_important
}