#include "sdl.h"

/* =================
* Fonction: initGraphique()
* Rôle: Initialise les bibliothèques graphiques utilisées par le programme (SDL, TTF, Audio)
* Entrées: Appart carte: la carte
* Sorties: Des erreurs si l'initialisation échoue
* return: La surface écran
*/
SDL_Surface* initGraphique(Appart carte)
{
    // Initialiser la SDL
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "erreur d'initialisation de la SDL %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());

        exit(EXIT_FAILURE);
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        fprintf(stderr, "Impossible d'initialiser OpenAudio : %s\n", Mix_GetError());
        exit(EXIT_FAILURE);
    }

    // Créer la fenêtre mère
    SDL_Surface *ecran = NULL;
    
    ecran = SDL_SetVideoMode(carte.largeur * COTE_SPRITES,30 +  carte.hauteur * COTE_SPRITES, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    SDL_WM_SetCaption("Robot qui trouve la sortie", NULL);

    //SDL_BlitSurface(SDL_LoadBMP("images/case_vide.bmp"), NULL, ecran, NULL);

    // Initialiser rand

    return ecran;
}

/* =================
* Fonction: afficherGraphique()
* Rôle: Met à jour la fenêtre graphique en fonction de la carte
* Entrées: Appart carte: la carte
           Robot robot: le robot
           Affichage affichage: la fenêtre graphique
* Sorties: La nouvelle fenêtre SDL mise à jour
* return: non
*/
void afficherGraphique(Appart carte,Robot robot, Affichage affichage)
{
    int i, j;
    char affPas[30];

    SDL_FillRect(affichage.ecran, NULL, SDL_MapRGB(affichage.ecran->format, 0, 0, 0));

    int paused = 0;
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    fermerAffichage();
                    exit(EXIT_SUCCESS);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT && pointInRect(event.button.x, event.button.y, 0, 0, COTE_SPRITES * carte.largeur, 30))
                    {
                        paused = 1;

                        while (paused)
                        {
                            SDL_WaitEvent(&event);

                            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && pointInRect(event.button.x, event.button.y, 0, 0, COTE_SPRITES * carte.largeur, 30))
                            {
                                paused = 0;
                            }
                            else if (event.type == SDL_QUIT)
                            {
                                fermerAffichage();
                                exit(EXIT_SUCCESS);
                            }
                        }
                    }
                }
            }
    for (i = 0; i < carte.hauteur; i++)
    {
        for (j = 0; j < carte.largeur; j++)
        {
            SDL_Surface* lacase;
            switch(carte.carte[i][j])
            {
                case CASE_VIDE:
                    lacase = affichage.sprites.case_vide;
                    break;

                case CASE_MUR:
                    lacase = affichage.sprites.case_mur;
                    break;

                case CASE_SORTIE:
                    lacase = affichage.sprites.case_sortie;
                    break;

                case CASE_ROBOT:
                        switch(robot.orientation)
                        {
                            case ORIENTATION_HAUT:
                                lacase = affichage.sprites.case_robot_up;
                                break;
                            case ORIENTATION_BAS:
                                lacase = affichage.sprites.case_robot_down;
                                break;
                            case ORIENTATION_GAUCHE:
                                lacase = affichage.sprites.case_robot_left;
                                break;
                            case ORIENTATION_DROITE:
                                lacase = affichage.sprites.case_robot_right;
                                break;
                        }
                    break;
                }

            if(lacase == NULL) {
                fprintf(stderr, "Image non chargée !\n");
                exit(EXIT_FAILURE);
            }
            lacase->clip_rect.y = 30 + i * COTE_SPRITES;
            lacase->clip_rect.x = j * COTE_SPRITES;
            //printf("x : %d  / y : %d \n", i * 10)

            SDL_BlitSurface(lacase, NULL, affichage.ecran, &lacase->clip_rect);
            
        }
    }
    sprintf(affPas, "compteur : %d", robot.compteur);
    SDL_Color couleurBlanche = {255, 255, 255};
    afficherTexte(affichage, affPas, (carte.largeur / 2 * COTE_SPRITES) / 2, 0, couleurBlanche);
    afficherTexte(affichage, "Pause", 0, 0, couleurBlanche);
    SDL_Flip(affichage.ecran);
    SDL_Delay(affichage.delai);
    
    
}

/* =================
* Fonction: chargerSprites()
* Rôle: Charger les images utilisées pour afficher la carte
* Entrées: char* chemin_*: le chemin vers les images à charger
* Sorties: non
* return: Sprites: une structure qui contient les sprites
*/
Sprites chargerSprites(char* chemin_case_vide, char* chemin_case_mur, char* chemin_case_sortie,
                        char* chemin_robot_up, char* chemin_robot_down, char* chemin_robot_left,
                        char* chemin_robot_right)
{
    Sprites sprites;
    sprites.case_vide = SDL_LoadBMP(chemin_case_vide);
    sprites.case_mur = SDL_LoadBMP(chemin_case_mur);
    sprites.case_sortie = SDL_LoadBMP(chemin_case_sortie);
    sprites.case_robot_up = SDL_LoadBMP(chemin_robot_up);
    sprites.case_robot_down = SDL_LoadBMP(chemin_robot_down);
    sprites.case_robot_left = SDL_LoadBMP(chemin_robot_left);
    sprites.case_robot_right = SDL_LoadBMP(chemin_robot_right);
    return sprites;
}

/* =================
* Fonction: chargerPolice()
* Rôle: Charge une police pour afficher du texte
* Entrées: char* chemin_police: le chemin vers la police
           int taille: la taille pour la police
* Sorties: un message d'erreur si le chargement échoue
* return: TTF_Font*: la police chargée
*/
TTF_Font* chargerPolice(char* chemin_police, int taille)
{
    TTF_Font* police = TTF_OpenFont(chemin_police, taille);

    if(police == NULL) {
        fprintf(stderr, "Impossible de charger la police !\n");
        exit(0);
    }

    return police;
}

/* =================
* Fonction: fermerAffichage()
* Rôle: Fermer la SDL et les autres bibliothèques
* Entrées: non
* Sorties: non
* return: non
*/
void fermerAffichage()
{
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

/* =================
* Fonction: afficherTexte()
* Rôle: Affiche du texte dans la fenêtre SDL
* Entrées: Affichage affichage: la structure qui contient l'écran
           char* texte: le texte à afficher
           int x: la position x pour le texte
           int y: la position y pour le texte
* Sorties: non
* return: non
*/
void afficherTexte(Affichage affichage, char* texte, int x, int y, SDL_Color couleur)
{
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */

    SDL_Surface* texte2 = TTF_RenderText_Blended(affichage.police, texte, couleur);
    texte2->clip_rect.x = x;
    texte2->clip_rect.y = y;

    SDL_BlitSurface(texte2, NULL,affichage.ecran, &texte2->clip_rect);

    SDL_FreeSurface(texte2);

}

/* =================
* Fonction: chargerMusique()
* Rôle: Charge une musique en mémoire
* Entrées: char* chemin_musique: le chemin vers la musique 
* Sorties: une erreur si le chargement échoue
* return: Mix_Music*: la musique
*/
Mix_Music* chargerMusique(char* chemin_musique)
{
   Mix_Music *musique; //Création du pointeur de type Mix_Music

   musique = Mix_LoadMUS(chemin_musique); //Chargement de la musique

   if(musique == NULL)
   {
       fprintf(stderr, "Impossible de charger la musique\n");
       exit(EXIT_FAILURE);
   }

   return musique;
}

/* =================
* Fonction: lancerMusique()
* Rôle: Lancer la lecture de la musique 
* Entrées: char* chemin_musique: le chemin vers la musique 
* Sorties: une erreur si le chargement échoue
* return: Mix_Music*: la musique
*/
void lancerMusique(Mix_Music* musique, int repetition)
{
    Mix_PlayMusic(musique, repetition);
}

/* =================
* Fonction: attendreQuitter()
* Rôle: Attend que l'utilisateur quitte le programme
* Entrées: non
* Sorties: non
* return: non
*/
void attendreQuitter()
{
    for (;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return;
                break;
            }
        }
    }
}

/* =================
* Fonction: afficherImage()
* Rôle: Charge une image puis l'affiche
* Entrées: SDL_Surface* ecran: l'écran
           char* chemin_image: le chemin vers l'image à afficher
           int x: coordonnée x
           int y: coordonnée y
* Sorties: une erreur si l'image n'existe pas
* return: non
*/
void afficherImage(SDL_Surface *ecran, char* chemin_image, int x, int y)
{
    SDL_Surface* image = SDL_LoadBMP(chemin_image);

    if(image == NULL) {
        fprintf(stderr, "Impossible d'ouvrir l'image!\n");
        exit(EXIT_FAILURE);
    }
    
    SDL_SetColorKey(image, SDL_SRCCOLORKEY, 0xffffff);

    image->clip_rect.x = x;
    image->clip_rect.y = y;

    SDL_BlitSurface(image, NULL, ecran, &image->clip_rect);
    SDL_Flip(ecran);
    SDL_FreeSurface(image);
}

/* =================
* Fonction: pointInRect()
* Rôle: Vérifie si un point se trouve dans un cadre
* Entrées: int x, y: les coordonnées du point à vérifier
           int rectx, recty: le point supérieur gauche du cadre
           int rectwidth, rectheight: la largeur et hauteur du cadre
* Sorties: 1 si le point est dans le rectangle ou 0 sinon
* return: non
*/
int pointInRect(int x, int y, int rectx, int recty, int rectwidth, int rectheight)
{
    if(x < rectx) return 0;
    if(x > (rectx + rectwidth)) return 0;
    if(y < recty) return 0;
    if(y > (recty + rectheight)) return 0;

    return 1;
}
