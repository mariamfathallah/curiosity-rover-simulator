#include "terrain.h"
#include <stdio.h>

#define TAILLE_NOM_FICHIER_MAX 255

void afficher_erreur_terrain (erreur_terrain errt) {
  if (errt == OK) {
    return;
  }
  printf("Erreur ");
  switch (errt)
  {
  case ERREUR_FICHIER:
    printf("durant la lecture du fichier\n");
    break;
  case ERREUR_TERRAIN:
    printf("variable terrain invalide\n");
    break;
  case ERREUR_LECTURE_LARGEUR:
    printf("durant la lecture de largeur\n");
    break;
  case ERREUR_LECTURE_HAUTEUR:
    printf("durant la lecture de la hauteur\n");
    break;
  case ERREUR_LARGEUR_INCORRECTE:
    printf(": largeur doit etre comprise entre 0 et %d\n", DIM_MAX);
    break;
  case ERREUR_HAUTEUR_INCORRECTE:
    printf(": hauteur doit etre comprise entre 0 et %d\n", DIM_MAX);
    break;
  case ERREUR_CARACTERE_INCORRECT:
    printf(": charactere incorrect!\n");
    break;
  case ERREUR_LIGNE_TROP_LONGUE:
    printf(": ligne trop longue!\n");
    break;
  case ERREUR_LIGNE_TROP_COURTE:
    printf(": ligne trop courte!\n");
    break;
  case ERREUR_LIGNES_MANQUANTES:
    printf(": ligne manquantes!\n");
    break;
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(": position robot manquante!\n");
    break;
  case ERREUR_PLUSIEURS_ROBOTS:
    printf(": plusieurs robots existent!\n");
    break;
  default : 
    return;
  }
}

int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  int x, y;
  erreur_terrain errt;
  char nomFichier[TAILLE_NOM_FICHIER_MAX];

  if (argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "r");
  while (f == NULL) {
    printf("Erreur durant l'ouverture du fichier donee !\n");
    fscanf(stdin, "%s", nomFichier);
    f = fopen(nomFichier, "r");
  }
  errt = lire_terrain(f, &t, &x, &y);
  afficher_erreur_terrain(errt);
  fclose(f);
  if (OK == errt) {
    afficher_terrain(&t);
    printf("Position initiale du robot : (%d, %d)\n", x, y);
  }
}
