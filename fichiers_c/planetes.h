#include <stdio.h>
#include <stdlib.h>
#define N_PLANETS 9
#define G 6.67408e-11
#define PAS_REEL                     10           // fréquence des calcules (sec)
#define PAS_SAUVEGARDE               86400           // fréquence de sauvegarde (sec)
#define PERIODE_ENREGISTREMENT       3652.5           // période de calcule à partir du 10 juin 2026 00:00:00 TDB (jours)
#define DAY_TO_SEC 86400        // jour en secs
#define mS 1.989e30

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



double norme(Vect vecteur);

Vect sumVect(Vect u, Vect v);

Vect combVect(Vect a, Vect b);

Vect multVectScal(Vect vecteur, double scalaire);

double acceleration(Planet planet);

Vect accelerationInteract(Planet *planets, char id_target);

void euler(Planet * planet, int pas);

void eulerInteract(Planet * planets, int pas);

void eulerAsymInteract(Planet * planets, int pas);

Planet initPlanet(double m, double x, double y , double z, double vx, double vy, double vz);

Planet * reset(Planet * planets);

TempFILE * initFiles(TempFILE * files, char * prefixe);

void saveFile(FILE * file, Planet planet, int t);

void saveToMain(FILE * methodes, TempFILE * listeFiles);

void createTempFiles(Planet * planet_list, TempFILE * files_list, float jours, void (*f)(Planet *, int));

void exportFile(FILE * mainFile, FILE * fileToPush);

void print_debug(Planet planet, int t);