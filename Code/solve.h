#ifndef SOLVE_H
#define SOLVE_H

#include "map.h"
#include "robot.h"
#include <unistd.h>


typedef struct {
    int positionInitialeI, positionInitialeJ;
    int** casesVisitees;
} MemoireDuRobot;

MemoireDuRobot initialiserMemoire(Appart carte, int positionI, int positionJ);
void visiterCase(MemoireDuRobot memoire, int positionI, int positionJ);
int caseVisitee(MemoireDuRobot memoire, int positionI, int positionJ);
void trouverLeMur(Appart carte, Robot* robot);
void affichagePrincipal(Appart carte, Robot robot);

#endif