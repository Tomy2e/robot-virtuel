#include "solve.h"

/* =================
* Fonction: initialiserMemoire()
* Rôle: initialise la structure MemoireDuRobot
* Entrées: Appart carte: la carte
           int positionI: la position I de départ du robot
           int positionJ: la position J de départ du robot
* Sorties: non
* return: memoire: la structure MemoireDuRobot
*/
MemoireDuRobot initialiserMemoire(Appart carte, int positionI, int positionJ)
{
    MemoireDuRobot memoire;
    int i, j;

    memoire.positionInitialeI = positionI;
    memoire.positionInitialeJ = positionJ;

    memoire.casesVisitees = (int**)malloc(sizeof(int*) * carte.hauteur);

    for(i = 0; i < carte.hauteur; i++)
    {
        memoire.casesVisitees[i] = (int*)malloc(sizeof(int) * carte.largeur);
    }

    // On met toutes les cases à 0
    for(i = 0; i < carte.hauteur; i++)
    {
        for(j = 0; j < carte.largeur; j++)
        {
            memoire.casesVisitees[i][j] = 0;
        }
    }

    return memoire;
}

/* =================
* Fonction: visiterCase()
* Rôle: marque la case comme visitée dans la mémoire du robot
* Entrées: MemoireDuRobot memoire: la mémoire du robot
           int positionI: la position I visitée par le robot
           int positionJ: la position J visitée par le robot
* Sorties: non
* return: non
*/
void visiterCase(MemoireDuRobot memoire, int positionI, int positionJ)
{
    memoire.casesVisitees[positionI][positionJ] = 1;
}

/* =================
* Fonction: caseVisitee()
* Rôle: Savoir si la case (i, j) a déjà été visitée
* Entrées: MemoireDuRobot memoire: la mémoire du robot
           int positionI: la position I
           int positionJ: la position J
* Sorties: non
* return: int: 1 si la case a déjà été visitée ou 0 sinon
*/
int caseVisitee(MemoireDuRobot memoire, int positionI, int positionJ)
{
    if(memoire.casesVisitees[positionI][positionJ] == 1) {
        return 1;
    } else
    {
        return 0;
    }
}

/* =================
* Fonction: affichagePrincipal()
* Rôle: Afficher la carte avec plusieurs informations sur le robot
* Entrées: Appart carte: la carte
           Robot robot: le robot
* Sorties: La carte + des informations sur le robot
* return: non
*/
void affichagePrincipal(Appart carte, Robot robot)
{
    system("clear");
    afficherCarte(carte);
    printf("Position I : %d, Position Y : %d, Orientation : %d Face à un mur ? %d\n", robot.positionI, robot.positionJ, robot.orientation, faceMur(carte, robot));
    printf("Proche de la sortie ? %d Compteur de pas : %d\n", procheSortie(carte, robot), robot.compteur);
    //usleep(10000);
}