#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "map.h"
#include "robot.h"
#include "solve.h"
#include <string.h>
#include "sdl.h"

/* Notes */
// i = hauteur (abscisses)
// j = largeur (ordonnees)

int main(int argc, char **argv)
{
    /* Définitions des variables */
    Appart carte;
    Robot robot;

    /* On vérifie que le nom de l'appart est bien fourni en argument */
    if (argc != 2)
    {
        fprintf(stderr, "Argument requis.\n");
        fprintf(stderr, "Nom de fichier manquant !\n");
        return EXIT_FAILURE;
    }

    /* On charge le fichier */
    carte = chargerCarte(argv[1]);

    //printf("%d, %d\n", carte.hauteur, carte.largeur);
    robot = trouverRobot(carte);

    /* Algo */
    int sensInitial;
    int startExplo = 0;

    int continuer = 1;
    Affichage affichage;
    affichage.ecran = initGraphique(carte);
    affichage.sprites = chargerSprites("images/case_vide_17.bmp", "images/case_mur_17.bmp", "images/case_sortie_17.bmp",
    "images/mario_up_17.bmp", "images/mario_down_17.bmp", "images/mario_left_17.bmp", "images/mario_right_17.bmp");
    
    affichage.delai = 10;

    affichage.police = chargerPolice("polices/Outwrite.ttf", 30);

    affichage.musiqueDeFin = chargerMusique("audio/smb_stage_clear.wav");

    lancerMusique(chargerMusique("audio/Super Mario Bros. Music - Ground Theme.mp3"), -1);

    while (continuer)
    {

        while (avancerRobot(carte, &robot) == 1)
        {
            afficherGraphique(carte, robot, affichage);
            affichagePrincipal(carte, robot);
        }

        // On va le plus haut possible
        tournerRobot(&robot, ORIENTATION_HAUT);
        while (avancerRobot(carte, &robot) == 1)
        {
            afficherGraphique(carte, robot, affichage);
            affichagePrincipal(carte, robot);
        }
        tournerRobot(&robot, ORIENTATION_GAUCHE);

        MemoireDuRobot memoire = initialiserMemoire(carte, robot.positionI, robot.positionJ);

        while (!procheSortie(carte, robot))
        {
            // On mémorise le sens initial du robot
            sensInitial = robot.orientation;
            // On regarde si on peut trouver à la droite du robot
            tournerRobot(&robot, droiteDuRobot(robot.orientation));

            if (!faceMur(carte, robot))
            {
                // Si oui on y go
            }
            else
            {
                // Si non on remet le sens initial et on continue
                tournerRobot(&robot, sensInitial);

                if (faceMur(carte, robot))
                {
                    tournerRobot(&robot, gaucheDuRobot(robot.orientation));
                }
            }

            if (startExplo == 0)
            {
                visiterCase(memoire, robot.positionI, robot.positionJ);
            }
            else
            {
                startExplo++;
            }

            avancerRobot(carte, &robot);

            if (startExplo > 0 /*&& caseAlentour(carte, robot, droiteDuRobot(robot.orientation), NULL, NULL) == CASE_MUR*/)
            {
                sensInitial = robot.orientation;
                //On fait tourner le robot à sa droite pour voir si il y a un mur (correction de bug)
                tournerRobot(&robot, droiteDuRobot(robot.orientation));
                int tmp;
                tmp = faceMur(carte, robot);
                tournerRobot(&robot, sensInitial);

                // On récupère la case à gauche du robot
                int coordGaucheI, coordGaucheJ;
                caseAlentour(carte, robot, gaucheDuRobot(robot.orientation), &coordGaucheI, &coordGaucheJ);

                if (caseVisitee(memoire, coordGaucheI, coordGaucheJ))
                {
                    tmp = 0;
                }

                if (tmp)
                {
                    tournerRobot(&robot, gaucheDuRobot(robot.orientation));
                    while (avancerRobot(carte, &robot) == 1)
                    {
                        visiterCase(memoire, robot.positionI, robot.positionJ);
                        // Affichage
                        if (procheSortie(carte, robot))
                        {
                            afficherGraphique(carte, robot, affichage);
                            affichagePrincipal(carte, robot);
                            printf("Nombre de coups: %d\n", robot.compteur);
                            lancerMusique(affichage.musiqueDeFin, 1);
                            afficherImage(affichage.ecran, "images/mario-25th-anniversary.bmp", 120, 100);
                            attendreQuitter();
                            fermerAffichage();
                            return 0;
                        }
                        afficherGraphique(carte, robot, affichage);
                        affichagePrincipal(carte, robot);
                    }
                    tournerRobot(&robot, opposeDuRobot(robot.orientation));
                    while (avancerRobot(carte, &robot) == 1)
                    {
                        // Aff
                        afficherGraphique(carte, robot, affichage);
                        affichagePrincipal(carte, robot);
                    }
                    tournerRobot(&robot, gaucheDuRobot(robot.orientation));
                }
            }

            //affichage etc.
            afficherGraphique(carte, robot, affichage);
            affichagePrincipal(carte, robot);
            // on recommence

            if (memoire.positionInitialeI == robot.positionI && memoire.positionInitialeJ == robot.positionJ)
            {
                startExplo = 1;
            }
        }

        //SDL_BlitSurface(mur, NULL, ecran, &mur->clip_rect);
        // LE robot doit regarder vers le haut
        //tournerRobot(&robot, ORIENTATION_HAUT);

        //afficherGraphique(carte, robot, ecran);
        //SDL_Flip(ecran);

        continuer = 0;
        //SDL_Delay(100);
    }
    printf("Nombre de coups: %d\n", robot.compteur);
    lancerMusique(affichage.musiqueDeFin, 1);
    afficherImage(affichage.ecran, "images/mario-25th-anniversary.bmp", 120, 100);
    attendreQuitter();
    fermerAffichage();
    return EXIT_SUCCESS;
}
