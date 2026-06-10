
#define T 1 // en dixième de jour 

typedef struct s_vect{
    float x;
    float y;
    float z;
} Vect;

typedef struct s_planet{
    Vect a;
    Vect v;
    Vect pos;
    float m;
} Planet;

/**
 * @brief Calcule la vitesse a un temps après un temps T
 * @param v Vitesse à l'instant T-1
 * @param a Accélération
 */
float euler_vitesse(float v, float a);

/**
 * @brief Calcule l'accélération de la planète
 * @param 
 */

 float accel();

 /**
  * @brief Calcule norme;
  */
 float norme(Vect vecteur);