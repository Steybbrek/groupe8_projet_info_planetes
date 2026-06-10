#define G 6.674e-11
#define T 8640 // temps en seconde (delta t)
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


void euler(Planet * planet);

double norme(Vect vecteur);

double acceleration(Planet planet);

Planet initPlanet(double m, double x, double y , double z, double vx, double vy, double vz);

void print_debug(Planet planet, int t);