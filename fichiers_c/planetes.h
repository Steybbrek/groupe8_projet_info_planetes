/**
 * @file planetes.h
 * @brief Un ensemble de fonctions pour estimer les trajectiores d'objets dans l'espace
 * @date 2026-06-15
 */

// LIBRAIRIES

#include <stdio.h>
#include <stdlib.h>




// DEFINES 

// bools
#define LOADING_BAR     1                           // Affichage d'une barre de chargement (impact très léger sur les perfs)
#define ENERGIE         0                           // Calcule de l'énergie du système (impact fort sur les perfs)
#define EULER_T         0                           // Euler Terminal
#define EULER           1                           // Euler Interact
#define EULER_ASSYM     1                           // Euler Assymétrique
#define RK4             1                           // RK4

// constantes modifiables / paramètres
#define N_PLANETS                    14             // nombre de planètes / objets
#define PAS_REEL                     300            // fréquence des calculs (sec)
#define PAS_SAUVEGARDE               86400          // fréquence de sauvegarde (sec)
#define PERIODE_ENREGISTREMENT       3652.5         // période de calculs à partir du 10 juin 2026 00:00:00 TDB (jours)
#define MARGE_ENERGIE                1              // marge d'erreur pour l'énergie (en %)

// constantes à ne pas modifier
#define G 6.67408e-11                               // constante de gravitation
#define DAY_TO_SEC 86400                            // jour en secs
#define mS 1.989e30                                 // masse solaire (kg)




// STRUCTS

typedef struct s_vect{
    double x;
    double y;
    double z;
} Vect;

typedef struct s_planet{
    Vect a;
    Vect v;
    Vect pos;
    double m;
} Planet;

typedef struct s_temp_file{
    FILE * file;
    char path[256];
}TempFILE;

typedef struct s_k_liste{
    Vect vect[N_PLANETS];
}KList; // était utile lorsqu'il y avait 2 listes de vect pour RK 4 classique mais est inutile (ou presque) pour RK 4 avec dérivée 2nde




// OUTILS VECT

double norme(Vect vecteur);

Vect sumVect(Vect u, Vect v);

Vect subtractVect(Vect a, Vect b);

Vect multVectScal(Vect vecteur, double scalaire);




// ACCELERATIONS

double acceleration(Planet planet);

Vect accelerationInteract(Planet *planets, int id_target);




// ENERGIES

double Ep(Planet * planets, int id_target);

double Ec(Planet target);

void ESysteme(Planet * planets, double * Esys, int jour, int etape, FILE * fileEnergie);




// METHODES

// Methodes Euler

void euler(Planet * planet, int pas);

void eulerInteract(Planet * planets, int pas);

void eulerAssymInteract(Planet * planets, int pas);

// Methode RK

KList fRK(Planet * planets);

KList calcK1(Planet * planets);

KList calcK2(Planet * planets, KList k1, double h);

KList calcK3(Planet * planets, KList k1, KList k2, double h);

KList calcK4(Planet * planets, KList k2, KList k3, double h);

void rk4(Planet * planets, int pas);




// OUTILS PLANET

Planet initPlanet(double m, double x, double y , double z, double vx, double vy, double vz);

Planet * reset(Planet * planets);




// OUTILS FICHIERS

void initiateFileData(TempFILE * p_file, char * prefixe, char * name);

TempFILE * initFiles(TempFILE * files, char * prefixe);

void saveFile(FILE * file, Planet planet, int t);

void saveToMain(FILE * methodes, TempFILE * listeFiles);

void createTempFiles(Planet * planet_list, TempFILE * files_list, float jours, void (*f)(Planet *, int), FILE * energie);

void exportFile(FILE * mainFile, FILE * fileToPush);




// FONCTION DE DEBUG

void print_loading_bar(double actuel, double total, int * progress);

void print_debug(Planet planet, int t);

void print_options();

void print_file(FILE * file);