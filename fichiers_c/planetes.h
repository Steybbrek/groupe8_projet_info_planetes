#include <stdio.h>
#include <stdlib.h>
#define N_PLANETS 9
#define G 6.67408e-11
#define T 8640 // temps en seconde (delta t)
#define S_TO_D 86400
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

Vect combVect(Vect a, Vect b);

double acceleration(Planet planet);

double accelerationInteract(Planet *planets, char id_target);

void euler(Planet * planet);

void eulerInteract(Planet * planets);

void eulerAsymInteract(Planet * planets);

Planet initPlanet(double m, double x, double y , double z, double vx, double vy, double vz);

Planet * reset(Planet * planets);

TempFILE * initFiles(TempFILE * files, char * prefixe);

void saveFile(FILE * file, Planet planet, int t);

void saveToMain(FILE * methodes, TempFILE * listeFiles);

void createTempFiles(Planet * planet_list, TempFILE * files_list, float jours, void (*f)(Planet *));

void exportFile(FILE * mainFile, FILE * fileToPush);

void print_debug(Planet planet, int t);