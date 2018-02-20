#ifndef MAP_H
#define MAP_H

/* includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* constantes */
#define CASE_VIDE ' '
#define CASE_MUR 'x'
#define CASE_SORTIE 'S'
#define CASE_ROBOT 'D'
#define CASE_VOID 'z'

/* structure */
typedef struct {
    int hauteur, largeur;
    char **carte;
} Appart;

/* fonctions */

Appart chargerCarte(char* fichier);
void afficherCarte(Appart carte);

#endif