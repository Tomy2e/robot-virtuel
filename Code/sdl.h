#ifndef SDL_H
#define SDL_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "map.h"
#include "robot.h"

#define COTE_SPRITES 17

typedef struct {
    SDL_Surface* case_vide;
    SDL_Surface* case_mur;
    SDL_Surface* case_sortie;
    SDL_Surface* case_robot_up;
    SDL_Surface* case_robot_down;
    SDL_Surface* case_robot_left;
    SDL_Surface* case_robot_right;
} Sprites;


typedef struct {
    SDL_Surface* ecran;
    Sprites sprites;
    TTF_Font *police;
    Mix_Music *musiqueDeFin;
    int delai;
} Affichage;

Sprites chargerSprites(char* chemin_case_vide, char* chemin_case_mur, char* chemin_case_sortie,
                        char* chemin_robot_up, char* chemin_robot_down, char* chemin_robot_left,
                        char* chemin_robot_right);
SDL_Surface* initGraphique(Appart carte);
void fermerAffichage();
void afficherGraphique(Appart carte,Robot robot, Affichage affichage);
void afficherTexte(Affichage affichage, char* texte, int x, int y, SDL_Color couleur);
TTF_Font* chargerPolice(char* chemin_police, int taille);
Mix_Music* chargerMusique(char* chemin_musique);
void lancerMusique(Mix_Music* musique, int repetition);
void attendreQuitter();
void afficherImage(SDL_Surface *ecran, char* chemin_image, int x, int y);
int pointInRect(int x, int y, int rectx, int recty, int rectwidth, int rectheight);

#endif