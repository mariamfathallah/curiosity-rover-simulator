#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "generation_terrains.h"

#include <stdio.h>
#include <stdlib.h>

void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  case ERREUR_PLUSIEURS_ROBOTS:
    printf("Erreur lecture du terrain : plusieurs robots existent!\n");
    exit(1);
  case ERREUR_TERRAIN:
    printf("Erreur lecture du terrain");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}

int main(int argc, char **argv) {
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;
  Terrain T;

  int N, i, l, h,  graine, nb_step_max, x, y;
  int nb_step_effectues, nb_terrains_robot_sorti, nb_terrains_robot_bloque, nb_terrains_robot_obstacle, nb_step_total;
  float dObst, pourcentageRobotSorti, pourcentageRobotBloque, pourcentageRobotObstacle, nb_step_moyen;
  FILE* resFile;
  FILE* fichTerrain;



  if (argc < 9) {
    printf("Usage: %s <nombre de terrains> <largeur> <hauteur> <densite_obstacle> <graine> <programme> <nombre de pas max> <fichier resultat> \n", argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  l = strtol(argv[2], NULL, 10);
  h = strtol(argv[3], NULL, 10);
  dObst = strtof(argv[4], NULL);
  graine = strtol(argv[5], NULL, 10);
  nb_step_max = strtol(argv[7], NULL, 10);

  // test de l et h
  if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h > DIM_MAX || h % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((dObst > 1) || (dObst < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }

  /* Lecture du programme */
  errp = lire_programme(&prog, argv[6]);
  gestion_erreur_programme(errp);

  // Ouverture du fichier résultat
  resFile = fopen(argv[8], "w");
  // Écriture du nombre de terrains
  fprintf(resFile, "%d\n", N);

  //Initialisation des accumulateurs pour calculer les moyennes
  nb_terrains_robot_bloque = 0;
  nb_terrains_robot_obstacle = 0;
  nb_terrains_robot_sorti = 0;
  nb_step_total = 0;

  //Initialisation de la graine
  srand(graine);

  //Initialisation de la position du robot
  x = l/2;
  y = h/2;
  printf("Posiition du robot : x = %d et y = %d", x, y);

  for(i = 0; i < N; i++){
    // Génération aléatoire des terrains
    generation_aleatoire(&T,l,h,dObst);
    while(existe_chemin_vers_sortie(&T) == 0){
      generation_aleatoire(&T,l,h,dObst);
    }
    // Écriture du terrain généré dans le fichier terrain_courant.txt
    /*Ouverture du fichier terrain en écriture*/
    fichTerrain = fopen("terrain_courant.txt","w");
    ecrire_terrain(fichTerrain,&T,x,y);
    fclose(fichTerrain);

    /* Initialisation de l'environnement : lecture du terrain,
     initialisation de la position du robot */
    errt = initialise_environnement(&envt, "terrain_courant.txt");
    gestion_erreur_terrain(errt);

    /* Initialisation de l'état */
    init_etat(&etat);
    nb_step_effectues = 0;
    do {
        res = exec_pas(&prog, &envt, &etat);
        /* Affichage du terrain et du robot */
        afficher_envt(&envt);
        nb_step_effectues++;
    } while (res == OK_ROBOT && nb_step_effectues < nb_step_max);
        if(nb_step_effectues >= nb_step_max){
        nb_terrains_robot_bloque++;
        fprintf(resFile,"%d\n",-1);
        }else{
            /* Affichage du résultat */
            switch (res) {
            case OK_ROBOT:
                printf("Robot sur une case libre, programme non terminé (ne devrait pas "
                    "arriver)\n");
                break;
            case SORTIE_ROBOT:
                nb_terrains_robot_sorti++;
                fprintf(resFile,"Nombre de pas effectues : %d\n",nb_step_effectues);
                nb_step_total = nb_step_total + nb_step_effectues;
                break;
            case ARRET_ROBOT:
                nb_terrains_robot_bloque++;
                fprintf(resFile,"%d\n",-1);
                break;
            case PLOUF_ROBOT:
                nb_terrains_robot_obstacle++;
                fprintf(resFile,"%d\n",-2);
                break;
            case CRASH_ROBOT:
                nb_terrains_robot_obstacle++;
                fprintf(resFile,"%d\n",-3);
                break;
            case ERREUR_PILE_VIDE:
                printf("ERREUR : pile vide\n");
                break;
            case ERREUR_ADRESSAGE:
                printf("ERREUR : erreur d'adressage\n");
                break;
            case ERREUR_DIVISION_PAR_ZERO:
                printf("ERREUR : division par 0\n");
                break;
            }
        }
  }

  // Écriture/Affichage des statistiques
  pourcentageRobotBloque=((float)nb_terrains_robot_bloque/N)*100;
  pourcentageRobotObstacle=((float)nb_terrains_robot_obstacle/N)*100;
  pourcentageRobotSorti=((float)nb_terrains_robot_sorti/N)*100;
  
  printf("Pourcentage de terrains ou le robot est bloqué : %f%% \n",pourcentageRobotBloque);
  printf("Pourcentage de terrains ou le robot est sorti : %f%% \n",pourcentageRobotSorti);
  printf("Pourcentage de terrains ou le robot a rencontré un obstacle : %f%% \n",pourcentageRobotObstacle);
  if(nb_terrains_robot_sorti > 0){
    nb_step_moyen = nb_step_total/nb_terrains_robot_sorti;
    printf("Nombre de pas moyen pour les sorties: %f \n",nb_step_moyen);
  }
  // fermeture des fichiers
  fclose(resFile);
  return 0;
}
