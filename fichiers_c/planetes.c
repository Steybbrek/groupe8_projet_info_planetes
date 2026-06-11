#include "planetes.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * @brief Renvoie la norme
 * @param vecteur Vecteur sur R³ 
 */
double norme(Vect vecteur){
    return sqrt(vecteur.x*vecteur.x + vecteur.y*vecteur.y + vecteur.z*vecteur.z);
}

/**
 * @brief Fait la somme de 2 vecteurs
 * @param a Premier vecteur
 * @param b Deuxième vecteur
 * @return Le vecteur résultant
 */
Vect combVect(Vect a, Vect b){
    Vect ab;
    ab.x = a.x - b.x;
    ab.y = a.y - b.y;
    ab.z = a.z - b.z;
    return ab;
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
 * @brief Calcule la variable à multiplier par l'accélération en x, y et z quand il y a plusieurs planètes
 * @param planets Liste des planètes
 * @param id_target Id de la planète que l'on étudie
 */
double accelerationInteract(Planet *planets, char id_target){
    double a = 0;
    double r;
    for (int i = 0 ; i < N_PLANETS ; i++){
        if (i != id_target){
            r = norme(combVect(planets[i].pos,planets[id_target].pos));
            a += (-G * planets[i].m / (r*r*r));
        }
    }
    return a;
}

/**
 * @brief Calcule les informations d'une planète après T secondes
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
 * @brief Calcule 
 * @param planets Liste des planètes
 */
void eulerInteract(Planet * planets){
    double a_list[N_PLANETS];
    Planet * planet;

    for (int i = 0; i < N_PLANETS ; i++){
        a_list[i] = acceleration(planets[i]) + accelerationInteract(planets,i);
    }

    for (int i = 0 ; i < N_PLANETS ; i++){
        planet = &(planets[i]);
        // Calcule de l'accélération à t-1
        planet->a.x = a_list[i] * planet->pos.x;
        planet->a.y = a_list[i] * planet->pos.y;
        planet->a.z = a_list[i] * planet->pos.z;

        // Calcule de la position en t

        planet->pos.x += planet->v.x * T;
        planet->pos.y += planet->v.y * T;
        planet->pos.z += planet->v.z * T;

        // Calcule de la vitesse en t

        planet->v.x += planet->a.x * T;
        planet->v.y += planet->a.y * T;
        planet->v.z += planet->a.z * T;
    }
}

/**
 * @brief Créer un struct planète et l'initialise
 * @param m Masse de la planète en kg
 * @param x Position en x de la planète en m
 * @param y Position en y de la planète en m
 * @param z Position en z de la planète en m
 * @param vx Vitesse en x de la planète en m/s
 * @param vy Vitesse en y de la planète en m/s
 * @param vz Vitesse en z de la planète en m/s
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
 * @brief Remet toutes les valeurs des planètes à leur valeurs initiales
 * @param planets Liste des planète
 * @return Addresse de la liste de planètes ou NULL
 */
Planet * reset(Planet * planets){
    planets = realloc(planets,sizeof(Planet)*N_PLANETS);
    // Mercure
    planets[0] = initPlanet(3.302e23, -5.939249500394277E+10, -1.424142594711035E+10, 4.338754675023021E+09, 6.939545151869151E+02, -4.540444941343805E+04, -3.773696839065323E+03); 
    // Venus
    planets[1] = initPlanet(48.685e23, -1.070725166099876E+11, 1.139346175891170E+10, 6.345074050304741E+09, -4.155817640337680E+03, -3.494715982962389E+04, -2.399645931443928E+02); 
    // Terre
    planets[2] = initPlanet(5.97219e24, -2.982262782007100E+10, -1.497712589089212E+11, 2.524112971142679E+07, 2.875115297834278E+04, -5.912322191541242E+03, -9.010199215850889E-01);
    // Mars
    planets[3] = initPlanet(6.4171e23, 1.948457196881133E+11, 8.312623598954162E+10, -3.009652509615280E+09, -8.633422290015989E+03, 2.433146747698761E+04, 7.215861409413744E+02); 
    
    return planets;
}

/**
 * @brief Créer une liste  de fichiers json pour sauvegarder les données de chaque planète (également lisibles)
 * @param files L'adresse de la liste d'adresse de fichiers
 * @param prefixe Le prefixe des fichiers json
 * @return Adresse de la liste ou NULL
 * @warning Il faut modifier manuellement le code en cas d'ajout 
 */
TempFILE * initFiles(TempFILE * files, char * prefixe){
    files = realloc(files, N_PLANETS * sizeof(TempFILE));

    // Mercure
    strcpy(files[0].path,"./temp_json_files/");
    strcat(files[0].path, prefixe);
    strcat(files[0].path,"_mercure.json");
    files[0].file = fopen(files[0].path,"w+");
    fprintf(files[0].file,"    \"%s%s\":","mercure_",prefixe);

    // Venus
    strcpy(files[1].path,"./temp_json_files/");
    strcat(files[1].path, prefixe);
    strcat(files[1].path,"_venus.json");
    files[1].file = fopen(files[1].path,"w+");
    fprintf(files[1].file,"    \"%s%s\":","venus_",prefixe);

    // Terre
    strcpy(files[2].path,"./temp_json_files/");
    strcat(files[2].path, prefixe);
    strcat(files[2].path,"_terre.json");
    files[2].file = fopen(files[2].path,"w+");
    fprintf(files[2].file,"    \"%s%s\":","terre_",prefixe);

    // Mars
    strcpy(files[3].path,"./temp_json_files/");
    strcat(files[3].path, prefixe);
    strcat(files[3].path,"_mars.json");
    files[3].file = fopen(files[3].path,"w+");
    fprintf(files[3].file,"    \"%s%s\":","mars_",prefixe);

    return files;

}

void saveFile(FILE * file, Planet planet, int t){
    fprintf(file, ",\n      [[%lf,%lf,%lf], [%lf,%lf,%lf], %d]", planet.pos.x , planet.pos.y , planet.pos.z , planet.v.x , planet.v.y , planet.v.z , t);
}

/**
 * @brief Copie le contenue d'un fichier dans un autre (cat)
 * @param mainFile Fichier destination
 * @param fileToPush Fichier origine
 * @warning Pas de check de pointeur vide
 */
void exportFile(FILE * mainFile, FILE * fileToPush){
    char c[1];
    fseek(fileToPush,0,SEEK_SET);
    c[0] = fgetc(fileToPush);
    while (c[0]!=EOF){
        fprintf(mainFile,c);
        c[0] = fgetc(fileToPush);
    }
}

/**
 * @brief Affiche les infos de la planète au même format que le fichier json
 * @param planet Planète à afficher
 * @param t L'instant t à laquelle les infos sont calculées
 */
void print_debug(Planet planet, int t){
    printf("[[%lf,%lf,%lf], [%lf,%lf,%lf], %d]\n", planet.pos.x , planet.pos.y , planet.pos.z , planet.v.x , planet.v.y , planet.v.z , t);
    // [[x,y,z], [vx,vy,vz], t]
}