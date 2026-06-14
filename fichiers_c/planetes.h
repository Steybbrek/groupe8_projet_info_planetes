/**
 * @file planetes.h
 * @brief Un ensemble de fonctions pour estimer les trajectiores d'objets dans l'espace
 * @authors SALMON RADENAC Théo, LAMOUR Briec
 * @date 2026-06-14
 */

// LIBRAIRIES

#include <stdio.h>
#include <stdlib.h>




// DEFINES 

#define N_PLANETS 10
#define G 6.67408e-11
#define PAS_REEL                     60           // fréquence des calcules (sec)
#define PAS_SAUVEGARDE               86400           // fréquence de sauvegarde (sec)
#define PERIODE_ENREGISTREMENT       365.25           // période de calcule à partir du 10 juin 2026 00:00:00 TDB (jours)
#define DAY_TO_SEC 86400        // jour en secs
#define mS 1.989e30
#define LOADING_BAR 0           // bool pour rajouter une barre de chargement (plus long, donc réfléchir avant de l'utiliser)




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
    Vect * vect;
}KList;




// OUTILS VECT

double norme(Vect vecteur);

Vect sumVect(Vect u, Vect v);

Vect combVect(Vect a, Vect b);

Vect multVectScal(Vect vecteur, double scalaire);




// ACCELERATIONS

double acceleration(Planet planet);

Vect accelerationInteract(Planet *planets, int id_target);




// METHODES

// Methodes Euler

void euler(Planet * planet, int pas);

void eulerInteract(Planet * planets, int pas);

void eulerAsymInteract(Planet * planets, int pas);

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

void createTempFiles(Planet * planet_list, TempFILE * files_list, float jours, void (*f)(Planet *, int));

void exportFile(FILE * mainFile, FILE * fileToPush);




// FONCTION DE DEBUG

void print_loading_bar(double actuel, double total);

void print_debug(Planet planet, int t);