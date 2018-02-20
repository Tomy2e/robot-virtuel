#ifndef ROBOT_H
#define ROBOT_H

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include "map.h"

/* constantes */
#define ORIENTATION_HAUT 1
#define ORIENTATION_BAS 2
#define ORIENTATION_GAUCHE 3
#define ORIENTATION_DROITE 4

/* structure */
typedef struct {
    // Position
    int positionI, positionJ;
    int compteur;
    // Orientation
    int orientation;
} Robot;

/* fonctions */
Robot trouverRobot(Appart carte);
int procheSortie(Appart carte, Robot robot);
int faceMur(Appart carte, Robot robot);
char caseAlentour(Appart carte, Robot robot, int orientation, int* pCoordonneeI, int* pCoordonneeJ);
int avancerRobot(Appart carte, Robot* robot);
void tournerRobot(Robot *robot, int nouvelleOrientation);
int gaucheDuRobot(int orientation);
int droiteDuRobot(int orientation);
int opposeDuRobot(int orientation);

#endif