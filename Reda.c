#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur.h"
#include "arbre.h"

//ouvre le fichier
void ouverture(element* e, char* nom_fichier){
    e->fichier = fopen(nom_fichier, "r");
    if(e->fichier == NULL){
        exit(-1);
    }
    lire_caractere(e);
}

void fermeture(element* e){
    fclose(e->fichier);
}

//lis un caractère
void lire_caractere(element* e){
    e->charac = fgetc(e->fichier);
}

//saute les espaces
void passer_espace(element* e){
    while(e->charac == ' ' || e->charac == '\t' || e->charac == '\n' || e->charac == '\r'){
        lire_caractere(e);
    }
}

void update_balise_actuelle(element* e, char* token, t_noeud* noeud_contenant) {
    if (strcmp(token, "<document>") == 0) {
        e->balise_actuelle = BALISE_DOCUMENT;
    } else if (strcmp(token, "</document>") == 0) {
        e->balise_actuelle = BALISE_FIN_DOCUMENT;
    } else if (strcmp(token, "<annexe>") == 0) {
        e->balise_actuelle = BALISE_ANNEXE;
    } else if (strcmp(token, "</annexe>") == 0) {
        e->balise_actuelle = BALISE_FIN_ANNEXE;
    } else if (strcmp(token, "<titre>") == 0) {
        e->balise_actuelle = BALISE_TITRE;
    } else if (strcmp(token, "</titre>") == 0) {
        e->balise_actuelle = BALISE_FIN_TITRE;
    } else if (strcmp(token, "<liste>") == 0) {
        e->balise_actuelle = BALISE_LISTE;
    } else if (strcmp(token, "</liste>") == 0) {
        e->balise_actuelle = BALISE_FIN_LISTE;
    } else if (strcmp(token, "<mot_important>") == 0) {
        e->balise_actuelle = BALISE_MOT_IMPORTANT;
    } else if (strcmp(token, "</mot_important>") == 0) {
        e->balise_actuelle = BALISE_FIN_MOT_IMPORTANT;
    }else if (strcmp(token, "<section>") == 0) {
        e->balise_actuelle = BALISE_SECTION;
    } else if (strcmp(token, "</section>") == 0) {
        e->balise_actuelle = BALISE_FIN_SECTION;
    } else if (strcmp(token, "<item>") == 0) {
        e->balise_actuelle = BALISE_ITEM;
    } else if (strcmp(token, "</item>") == 0) {
        e->balise_actuelle = BALISE_FIN_ITEM;
    } else if (strcmp(token, "<br/>") == 0) {
        e->balise_actuelle = BALISE_RETOUR_LIGNE;
    }
    else {
        printf("\nERREUR :balise non existante: %s . \n", token);
        libererArbre(remonter_racine(noeud_contenant));
        exit(-1);
    }
}

void fermeture_balise(element* e, int balise_contenante , t_noeud* noeud_contenant) {
    if (e->balise_actuelle != balise_contenante) {
        printf("Erreur probleme fermeture: balise attendue: %d, balise trouvee: %d\n", balise_contenante, e->balise_actuelle);
        libererArbre(remonter_racine(noeud_contenant));
        exit(-1);
    }
}

void lire_token(element* e, t_noeud* noeud_contenant) {
    char token[20];
    int i = 0;
    while (e->charac != '>') {
        token[i] = e->charac;
        lire_caractere(e);
        i++;
    }
    token[i] = '>';
    token[i+1] = '\0';
    consommer(e, '>', noeud_contenant);
    update_balise_actuelle(e, token, noeud_contenant);
}


// Consomme un token et met à jour balise_actuelle
void consommer(element* e, char caractere , t_noeud* noeud_contenant) {
    if (e->charac == caractere) {
        lire_caractere(e);
    }else {
        printf("Erreur: caractere attendu: %c, caractere trouve: %c\n", caractere, e->charac);
        libererArbre(remonter_racine(noeud_contenant));
        exit(-1);
    }
}

void consommer_token(element* e, char* token, t_noeud* noeud_contenant){
    int i = 0;
    while (token[i] != '\0') {
        consommer(e, token[i], noeud_contenant);
        i++;
    }
    update_balise_actuelle(e, token, noeud_contenant);
}

//analyse le texte enrichi
void texte_enrichi(element* e, t_noeud* arbre_enrichi){
    document(e, arbre_enrichi);
    while (e->charac != EOF) {
        annexes(e , arbre_enrichi);
    }
}

//analyse le document
void document(element* e , t_noeud* arbre_enrichi){
    consommer_token(e, "<document>", arbre_enrichi);
    t_noeud* noeud_document = creerArbre("<document>", 0);
    insererFils(arbre_enrichi, noeud_document);
    passer_espace(e);
    contenu(e, BALISE_FIN_DOCUMENT, noeud_document);
    passer_espace(e);
}

//analyse le contenu
void contenu(element* e , int balise_contenante , t_noeud* noeud_contenant){
    while (e->balise_actuelle != balise_contenante) {
        if (e->charac == EOF)
        {
            printf("Erreur: balise de fin manquante\n");
            libererArbre(remonter_racine(noeud_contenant));
            exit(-1);
        }
        if (e->charac == '<') {
            int balise_ancienne = e->balise_actuelle;
            lire_token(e, noeud_contenant);
            switch (e->balise_actuelle) {
                case BALISE_FIN_DOCUMENT:
                    fermeture_balise(e, balise_contenante , noeud_contenant);
                    break;
                case BALISE_FIN_ANNEXE:
                    fermeture_balise(e, balise_contenante , noeud_contenant);
                    break;
                case BALISE_SECTION:
                    t_noeud* noeud_section = creerArbre("<section>", noeud_contenant->profondeur + 1);
                    insererFils(noeud_contenant, noeud_section);
                    section(e, noeud_section);
                    break;
                case BALISE_FIN_SECTION:
                    fermeture_balise(e, balise_contenante, noeud_contenant);
                    break;
                case BALISE_TITRE:
                    t_noeud* noeud_titre = creerArbre("<titre>", noeud_contenant->profondeur);
                    insererFils(noeud_contenant, noeud_titre);
                    titre(e, noeud_titre);
                    break;
                case BALISE_FIN_TITRE:
                    fermeture_balise(e, balise_contenante, noeud_contenant);
                    break;
                case BALISE_LISTE:
                    t_noeud* noeud_liste = creerArbre("<liste>", noeud_contenant->profondeur + 1);
                    insererFils(noeud_contenant, noeud_liste);
                    liste(e, noeud_liste);
                    passer_espace(e);
                    break;
                case BALISE_FIN_LISTE:
                    fermeture_balise(e, balise_contenante, noeud_contenant);
                    break;
                case BALISE_MOT_IMPORTANT:
                    mot_enrichi(e, noeud_contenant);
                    break;
                case BALISE_FIN_MOT_IMPORTANT:
                    break;
                case BALISE_RETOUR_LIGNE:
                    mot_enrichi(e, noeud_contenant);
                    e->balise_actuelle = balise_ancienne;
                    break;
                default:
                    printf("Erreur: balise non reconnue ou incorrecte\n");
                    libererArbre(remonter_racine(noeud_contenant));
                    exit(-1);
                    break;
            }
        }
        else {
            mot_enrichi(e , noeud_contenant);
        }
    }
    passer_espace(e);
}

//analyse la section
void section(element* e, t_noeud* noeud_section){
    passer_espace(e);
    contenu(e, BALISE_FIN_SECTION , noeud_section);
    passer_espace(e);
}

//analyse le titre
void titre(element* e, t_noeud* noeud_titre){
    passer_espace(e);
    texte(e , noeud_titre);
    passer_espace(e);
}

void texte(element* e, t_noeud* noeud_contenant){
    while(e->balise_actuelle != BALISE_FIN_TITRE) {
        if (e->charac == '<') {
            int balise_ancienne = e->balise_actuelle;
            lire_token(e , noeud_contenant);
            switch (e->balise_actuelle) {
                case BALISE_FIN_TITRE:
                    fermeture_balise(e, BALISE_FIN_TITRE, noeud_contenant);
                    break;
                case BALISE_MOT_IMPORTANT:
                    mot_enrichi(e , noeud_contenant);
                    break;
                case BALISE_RETOUR_LIGNE:
                    mot_enrichi(e , noeud_contenant);
                    e->balise_actuelle = balise_ancienne;
                    break;
                default:
                    printf("Erreur: balise non reconnue ou incorrecte\n");
                    libererArbre(remonter_racine(noeud_contenant));
                    exit(-1);
                    break;
            }
        }
        else {
            mot_enrichi(e, noeud_contenant);
        }
    }
}

//analyse la liste
void liste(element* e, t_noeud* noeud_liste){
    while(e->balise_actuelle != BALISE_FIN_LISTE) {
        passer_espace(e);
        if (e->charac == '<') {
            lire_token(e, noeud_liste);
            if (e->balise_actuelle == BALISE_FIN_LISTE) {
                fermeture_balise(e, BALISE_FIN_LISTE, noeud_liste);
                break;
            }
            else if(e->balise_actuelle == BALISE_ITEM){
                t_noeud* noeud_item = creerArbre("<item>", noeud_liste->profondeur + 1);
                insererFils(noeud_liste, noeud_item);
                item(e, noeud_item);
                passer_espace(e);
            }
            else {
                printf("Erreur 1: balise liste non fermée\n");
                libererArbre(remonter_racine(noeud_liste));
                exit(-1);
            }
        }
    }
}

void item(element* e , t_noeud* noeud_item){
    while(e->balise_actuelle != BALISE_FIN_ITEM) {
        if (e->charac == '<') {
            lire_token(e, noeud_item);
            if (e->balise_actuelle == BALISE_FIN_ITEM) {
                fermeture_balise(e, BALISE_FIN_ITEM, noeud_item);
                break;
            }
            else if(e->balise_actuelle == BALISE_LISTE){
                t_noeud* noeud_liste = creerArbre("<liste>", noeud_item->profondeur + 1);
                insererFils(noeud_item, noeud_liste);
                liste(e, noeud_liste);
            }
            else if(e->balise_actuelle == BALISE_FIN_LISTE){
                break;
            }
            else {
                printf("Erreur 2: balise item non fermée\n");
                libererArbre(remonter_racine(noeud_item));
                exit(-1);
            }
        }
        else {
            mot_enrichi(e , noeud_item);
        }
    }
}

//analyse les annexes
void annexes(element* e, t_noeud* noeud_enrichi){
    consommer_token(e, "<annexe>", noeud_enrichi);
    t_noeud* noeud_annexe = creerArbre("<annexe>", 0);
    insererFils(noeud_enrichi, noeud_annexe);
    passer_espace(e);
    contenu(e, BALISE_FIN_ANNEXE, noeud_annexe);
    passer_espace(e);
}

//analyse le mot enrichi
void mot_enrichi(element* e, t_noeud* noeud_contenant){
    if (e->balise_actuelle == BALISE_MOT_IMPORTANT) {
        t_noeud* noeud_mot_important = creerArbre("<mot_important>", noeud_contenant->profondeur + 1);
        insererFils(noeud_contenant, noeud_mot_important);
        passer_espace(e);
        mot_important(e , noeud_mot_important);
        passer_espace(e);
    } else if(e->balise_actuelle == BALISE_RETOUR_LIGNE) {
        t_noeud* noeud_retour_ligne = creerArbre("<br/>", noeud_contenant->profondeur + 1);
        insererFils(noeud_contenant, noeud_retour_ligne);
        passer_espace(e);
    } else {
        mot_simple(e , noeud_contenant);
    }
}

//analyse le mot important
void mot_important(element* e , t_noeud* noeud_mot_important){
    while (e->balise_actuelle != BALISE_FIN_MOT_IMPORTANT) {
        if(e->charac == '<') {
            lire_token(e, noeud_mot_important);
            if (e->balise_actuelle == BALISE_FIN_MOT_IMPORTANT) {
                fermeture_balise(e, BALISE_FIN_MOT_IMPORTANT, noeud_mot_important);
                break;
            }
            else {
                printf("Erreur: balise important non fermée\n");
                libererArbre(remonter_racine(noeud_mot_important));
                exit(-1);
            }
        }
        else {
            mot_simple(e , noeud_mot_important);
        }
    }
}


//analyse le mot simple
void mot_simple(element* e , t_noeud* noeud_contenant){
    char mot[100];
    int i = 0;
    while (e->charac != ' ' && e->charac != '\n' && e->charac != '<' && e->charac != EOF) {
        mot[i] = e->charac;
        lire_caractere(e);
        i++;
    }
    mot[i] = '\0';
    t_noeud* noeud_mot = creerArbre(mot, noeud_contenant->profondeur + 1);
    insererFils(noeud_contenant, noeud_mot);
    passer_espace(e);
}