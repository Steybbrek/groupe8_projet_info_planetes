#include "planetes.h"
#include <math.h>
#include <stdio.h>

/**
 * @brief Renvoie la norme
 * @param vecteur Vecteur sur R³ 
 */
double norme(Vect vecteur){
    return sqrt(vecteur.x*vecteur.x + vecteur.y*vecteur.y + vecteur.z*vecteur.z);
}

/**
 * @brief Calcule la variable à multiplier par l'accélération en x, y et z
 * @param planet La planète dont on cherche l'accélération
 */
double acceleration(Planet planet){
    double r = norme(planet.pos);
    return (-G * mS / (r*r*r));
}

/**
 * @brief Calcule la vitesse a un temps après un temps T
 * @param planet La planète dont on cherche les infos.
 */
void euler(Planet * planet){

    // Calcule de l'accélération à t-1
    double a = acceleration(*planet);

    planet->a.x = a * planet->pos.x;
    planet->a.y = a * planet->pos.y;
    planet->a.z = a * planet->pos.z;

    // Calcule de la position en t

    planet->pos.x += planet->v.x * T;
    planet->pos.y += planet->v.y * T;
    planet->pos.z += planet->v.z * T;

    // Calcule de la vitesse en t

    planet->v.x += planet->a.x * T;
    planet->v.y += planet->a.y * T;
    planet->v.z += planet->a.z * T;

}

/**
 * @brief Créer un struct planète et l'initialise
 * @param m Masse de la planète
 * @param x Position en x de la planète
 * @param y Position en y de la planète
 * @param z Position en z de la planète
 * @param vx Vitesse en x de la planète
 * @param vy Vitesse en y de la planète
 * @param vz Vitesse en z de la planète
 * @return Struct Planet initialisé
 */
Planet initPlanet(double m, double x, double y , double z, double vx, double vy, double vz){
    Planet planet;

    // Initialise la masse
    planet.m = m;
    
    // Initialise la position
    planet.pos.x = x;
    planet.pos.y = y;
    planet.pos.z = z;
    
    // Initialise la vitesse
    planet.v.x = vx;
    planet.v.y = vy;
    planet.v.z = vz;

    // Initialise l'accélération
    planet.a.x = 0;
    planet.a.y = 0;
    planet.a.z = 0;

    return planet;
}

/**
 * @brief Affiche les infos de la planète au même format que le fichier json
 * @param planet Planète à afficher
 * @param t L'instant t à laquelle les infos sont calculées
 */

 void print_debug(Planet planet, int t){
    printf("[[%f,%f,%f], [%f,%f,%f], %d]\n", planet.pos.x , planet.pos.y , planet.pos.z , planet.v.x , planet.v.y , planet.v.z , t);
    // [[x,y,z], [vx,vy,vz], t]
 }