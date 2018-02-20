#include "robot.h"

/* =================
* Fonction: trouverRobot()
* Rôle: trouver les coordonnées (i et j) du robot
* Entrées: Appart carte: la carte
* Sorties: si le robot n'est pas trouvé, on affiche une erreur
* return: Robot robot: une structure robot
*/
Robot trouverRobot(Appart carte)
{
    Robot robot;
    int i, j, trouve = 0;

    // On parcours le tableau pour trouver le robot
    for(i = 0; i < carte.hauteur; i++)
    {
        for(j = 0; j < carte.largeur; j++)
        {
            if(carte.carte[i][j] == CASE_ROBOT) {
                robot.positionI = i;
                robot.positionJ = j;
                robot.orientation = ORIENTATION_DROITE;
                robot.compteur = 0;
                trouve = 1;
                break;
            }
        }
    }

    if(!trouve) {
        fprintf(stderr, "Le robot n'a pas été trouvé dans la carte !\n");
        exit(EXIT_FAILURE);
    }

    return robot;
}

/* =================
* Fonction: faceMur()
* Rôle: Savoir si le robot est face à un mur ou pas
* Entrées: Appart carte: la carte
           Robot robot: le robot
* Sorties: non
* return: int : 1 si le robot est face à un mur ou sinon 0
*/
int faceMur(Appart carte, Robot robot)
{
    if(caseAlentour(carte, robot, robot.orientation, NULL, NULL) == CASE_MUR) {
        return 1;
    }
    else {
        return 0;
    }
}

/* =================
* Fonction: procheSortie()
* Rôle: Savoir si le robot est à côté de la sortie
* Entrées: Appart carte: la carte
           Robot robot: le robot
* Sorties: non
* return: int : 1 si le robot est à côté de la sortie ou sinon 0
*/
int procheSortie(Appart carte, Robot robot)
{
    if(caseAlentour(carte, robot, ORIENTATION_HAUT, NULL, NULL) == CASE_SORTIE || 
    caseAlentour(carte, robot, ORIENTATION_BAS, NULL, NULL) ==  CASE_SORTIE ||
    caseAlentour(carte, robot, ORIENTATION_GAUCHE, NULL, NULL) == CASE_SORTIE ||
    caseAlentour(carte, robot, ORIENTATION_DROITE, NULL, NULL) == CASE_SORTIE) {
        return 1;
    }
    else {
        return 0;
    }
}

/* =================
* Fonction: tournerRobot()
* Rôle: Changer l'orientation du robot
* Entrées: Robot* robot: pointeur sur le robot
           int nouvelleOrientation: la nouvelle orientation que le robot doit prendre
* Sorties: non
* return: non
*/
void tournerRobot(Robot *robot, int nouvelleOrientation)
{
    robot->orientation = nouvelleOrientation;
}

/* =================
* Fonction: caseAlentour()
* Rôle: Savoir quelle case se trouve à côté du robot en fonction de son orientation
* Entrées: Appart carte: la carte
           Robot robot: le robot
           int orientation: l'orientation que le robot doit prendre pour trouver la case
           int* pCoordonneeI: si non NULL, la fonction écrit la coordonnée I de la case trouvée
           int* pCoordonneeJ: si non NULL, la fonction écrit la coordonnée J de la case trouvée
* Sorties: si la variable orientation est invalide, affiche une erreur
* return: char: la valeur de la case cherchée
*/
char caseAlentour(Appart carte, Robot robot, int orientation, int* pCoordonneeI, int* pCoordonneeJ)
{
    int cordonneeI, cordonneeJ;
    switch(orientation) {
        case ORIENTATION_HAUT :
            cordonneeI = robot.positionI - 1;
            cordonneeJ = robot.positionJ;
        break;

        case ORIENTATION_BAS :
            cordonneeI = robot.positionI + 1;
            cordonneeJ = robot.positionJ;
        break;

        case ORIENTATION_GAUCHE :
            cordonneeI = robot.positionI;
            cordonneeJ = robot.positionJ - 1;
        break;

        case ORIENTATION_DROITE :
            cordonneeI = robot.positionI;
            cordonneeJ = robot.positionJ + 1;
        break;

        default :
            fprintf(stderr, "Orientation invalide ! \n");
            exit(EXIT_FAILURE);
        break;
    }

    if(pCoordonneeI != NULL && pCoordonneeJ != NULL) {
        *pCoordonneeI = cordonneeI;
        *pCoordonneeJ = cordonneeJ;
    }

    // On vérifie que cette case n'est pas hors tableau
    if(cordonneeI < 0 || cordonneeJ < 0 || cordonneeI >= carte.hauteur || cordonneeJ >= carte.largeur) {
        return CASE_VOID;
    }
    else {
        return carte.carte[cordonneeI][cordonneeJ];
    } 
}

/* =================
* Fonction: avancerRobot()
* Rôle: Faire avance le robot de 1 case dans son orientation actuelle
* Entrées: Appart carte: la carte
           Robot* robot: pointeur sur le robot
* Sorties: non
* return: int: 1 si le robot a pu avancer ou 0 sinon (par exemple si il a croisé un mur)
*/
int avancerRobot(Appart carte, Robot* robot)
{
    int nouveauI, nouveauJ;

    if(caseAlentour(carte, *robot, robot->orientation, &nouveauI, &nouveauJ) == CASE_VIDE) {
        carte.carte[robot->positionI][robot->positionJ] = CASE_VIDE;
        carte.carte[nouveauI][nouveauJ] = CASE_ROBOT;

        robot->positionI = nouveauI;
        robot->positionJ = nouveauJ;

        robot->compteur++;

        return 1;
    }
    else {
        return 0;
    }
}

/* =================
* Fonction: droiteDuRobot()
* Rôle: connaître la droite relative du robot
* Entrées: int orientation: une direction
* Sorties: message d'erreur si l'orientation n'est pas valide
* return: int: la nouvelle orientation
*/
int droiteDuRobot(int orientation)
{
    switch (orientation)
    {
    case ORIENTATION_HAUT:
        return ORIENTATION_DROITE;
        break;

    case ORIENTATION_GAUCHE:
        return ORIENTATION_HAUT;
        break;

    case ORIENTATION_DROITE:
        return ORIENTATION_BAS;
        break;

    case ORIENTATION_BAS:
        return ORIENTATION_GAUCHE;
        break;

    default :
        fprintf(stderr, "une erreur s'est produite dans droiteDuRobot\n");
        exit(EXIT_FAILURE);
        return 0;
        break;
    }
}

/* =================
* Fonction: gaucheDuRobot()
* Rôle: connaître la gauche relative du robot
* Entrées: int orientation: une direction
* Sorties: message d'erreur si l'orientation n'est pas valide
* return: int: la nouvelle orientation
*/
int gaucheDuRobot(int orientation)
{
    switch(orientation)
    {
        case ORIENTATION_HAUT :
            return ORIENTATION_GAUCHE;
            break;
        case ORIENTATION_GAUCHE :
            return ORIENTATION_BAS;
            break;

        case ORIENTATION_DROITE :
            return ORIENTATION_HAUT;
            break;

        case ORIENTATION_BAS :
            return ORIENTATION_DROITE;
            break;
        default :
            fprintf(stderr, "une erreur s'est produite dans gaucheDuRobot\n");
            exit(EXIT_FAILURE);
            return 0;
            break;
    }
}

/* =================
* Fonction: opposeDuRobot()
* Rôle: connaître l'opposé relative du robot
* Entrées: int orientation: une direction
* Sorties: message d'erreur si l'orientation n'est pas valide
* return: int: la nouvelle orientation
*/
int opposeDuRobot(int orientation)
{
    switch(orientation)
    {
        case ORIENTATION_HAUT :
            return ORIENTATION_BAS;
            break;

        case ORIENTATION_BAS :
            return ORIENTATION_HAUT;
            break;

        case ORIENTATION_GAUCHE :
            return ORIENTATION_DROITE;
            break;

        case ORIENTATION_DROITE :
            return ORIENTATION_GAUCHE;
            break;
        default :
            fprintf(stderr, "une erreur s'est produite dans opposeDuRobot\n");
            exit(EXIT_FAILURE);
            return 0;
            break;
    }
}