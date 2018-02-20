#include "map.h"

/* =================
* Fonction: chargerCarte()
* Rôle: charger la carte dans une structure ainsi que sa taille
* Entrées: char* chemin_fichier: le chemin vers le fichier à charger
* Sorties: messages d'erreur si il y a des erreurs
* return: Appart: la structure qui contient la carte et ses dimensions
*/
Appart chargerCarte(char *cheminFichier)
{
    FILE *fichier = fopen(cheminFichier, "r");
    Appart carte;
    char buffer;
    int i = 0, cHauteur = 0, cLargeur = 0;

    /* On vérifie que le fichier a bien été ouvert */
    if (fichier == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier !\n Vérifiez le nom du fichier.\n");
        exit(EXIT_FAILURE);
    }

    /* On récupère la hauteur et la largeur */
    fscanf(fichier, "%d:%d", &carte.largeur, &carte.hauteur);

    /* On alloue la mémoire pour la carte */
    carte.carte = (char **)malloc(sizeof(char *) * carte.hauteur);

    for (i = 0; i < carte.hauteur; i++)
    {
        carte.carte[i] = (char *)malloc(sizeof(char) * carte.largeur);
    }

    /* On charge la carte dans un tableau */
    while (fscanf(fichier, "%c", &buffer) != EOF)
    {
        //printf("%c", buffer);

        // On vérifie que le caractère est correct
        if (buffer == CASE_VIDE || buffer == CASE_MUR || buffer == CASE_SORTIE || buffer == CASE_ROBOT)
        {
            carte.carte[cHauteur][cLargeur] = buffer;
            cLargeur++;

            if (cLargeur >= carte.largeur)
            {
                cHauteur++;
                cLargeur = 0;
            }
        }
        else if ((buffer != '\r' && buffer != '\n'))
        {
            fprintf(stderr, "Le caractère %c n'a pas été reconnu !\n", buffer);
            exit(EXIT_FAILURE);
        }
    }

    // On vérifie que la largeur et la hauteur étaient valides
    if (cHauteur != carte.hauteur || cLargeur != 0)
    {
        fprintf(stderr, "Lecture du fichier incomplète, vérifiez la largeur/hauteur !\n");
        exit(EXIT_FAILURE);
    }

    /* On ferme le fichier */
    fclose(fichier);

    return carte;
}

/* =================
* Fonction: afficherCarte()
* Rôle: affiche la carte dans le terminal
* Entrées: Appart carte: la carte
* Sorties: la carte est affichée
* return: non
*/
void afficherCarte(Appart carte)
{
    int i, j;

    for (i = 0; i < carte.hauteur; i++)
    {
        for (j = 0; j < carte.largeur; j++)
        {
            printf("%c", carte.carte[i][j]);
        }
        printf("\n");
    }
}
