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
 * @brief Calcule les forces appliquer sur l'objet par les autres planètes
 * @param planets Liste des planètes
 * @param id_target Id de la planète que l'on étudie
 */
Vect accelerationInteract(Planet *planets, char id_target){
    Vect a;
    Vect r;
    a.x = 0;
    a.y = 0;
    a.z = 0;
    double norme_r;
    double force;
    for (int i = 0 ; i < N_PLANETS ; i++){
        if (i != id_target){
            r = combVect(planets[i].pos,planets[id_target].pos);
            norme_r = norme(r);
            force = (-G * planets[i].m / (norme_r * norme_r * norme_r));
            a.x += force * r.x;
            a.y += force * r.y;
            a.z += force * r.z;
        }
    }
    return a;
}

/**
 * @brief Calcule les informations d'une planète après pas secondes
 * @param planet La planète dont on cherche les infos.
 * @param pas Pas en secondes
 */
void euler(Planet * planet, int pas){

    // Calcule de l'accélération à t-1
    double a = acceleration(*planet);

    planet->a.x = a * planet->pos.x;
    planet->a.y = a * planet->pos.y;
    planet->a.z = a * planet->pos.z;

    // Calcule de la position en t

    planet->pos.x += planet->v.x * pas;
    planet->pos.y += planet->v.y * pas;
    planet->pos.z += planet->v.z * pas;

    // Calcule de la vitesse en t

    planet->v.x += planet->a.x * pas;
    planet->v.y += planet->a.y * pas;
    planet->v.z += planet->a.z * pas;

}

/**
 * @brief Calcule les informations avec la méthode d'Euler et des intéraction entre planètes
 * @param planets Liste des planètes
 * @param pas Pas en secondes
 */
void eulerInteract(Planet * planets, int pas){
    Vect  a_list[N_PLANETS];
    Planet * planet;
    double a_sol;

    for (int i = 0; i < N_PLANETS ; i++){
        a_list[i] = accelerationInteract(planets,i);
        a_sol = acceleration(planets[i]);
        a_list[i].x += a_sol;
        a_list[i].y += a_sol;
        a_list[i].z += a_sol;

    }

    for (int i = 0 ; i < N_PLANETS ; i++){
        planet = &(planets[i]);

        // Calcule de l'accélération à t-1
    
        planet->a.x = a_list[i].x * planet->pos.x;
        planet->a.y = a_list[i].y * planet->pos.y;
        planet->a.z = a_list[i].z * planet->pos.z;

        // Calcule de la position en t

        planet->pos.x += planet->v.x * pas;
        planet->pos.y += planet->v.y * pas;
        planet->pos.z += planet->v.z * pas;

        // Calcule de la vitesse en t

        planet->v.x += planet->a.x * pas;
        planet->v.y += planet->a.y * pas;
        planet->v.z += planet->a.z * pas;
    }
}

/**
 * @brief Calcule les informations avec la méthode d'Euler et des intéraction avec les autres planètes
 * @param planets Liste des planètes
 * @param pas Pas en secondes
 */
void eulerAsymInteract(Planet * planets, int pas){
    Vect  a_list[N_PLANETS];
    Planet * planet;
    double a_sol;

    for (int i = 0; i < N_PLANETS ; i++){
        a_list[i] = accelerationInteract(planets,i);
        a_sol = acceleration(planets[i]);
        a_list[i].x += a_sol;
        a_list[i].y += a_sol;
        a_list[i].z += a_sol;

    }

    for (int i = 0 ; i < N_PLANETS ; i++){
        planet = &(planets[i]);

        // Calcule de la position en t

        planet->pos.x += planet->v.x * pas;
        planet->pos.y += planet->v.y * pas;
        planet->pos.z += planet->v.z * pas;

        // Calcule de l'accélération à t-1

        planet->a.x = a_list[i].x * planet->pos.x;
        planet->a.y = a_list[i].y * planet->pos.y;
        planet->a.z = a_list[i].z * planet->pos.z;

        // Calcule de la vitesse en t

        planet->v.x += planet->a.x * pas;
        planet->v.y += planet->a.y * pas;
        planet->v.z += planet->a.z * pas;
    }
}

/**
 * @brief Fonction RK
 * @param
 */
void fRK(){
    //TODO : RK interact directe
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
    planets[0] = initPlanet(3.302e23, -5.939249500394277e+10, -1.424142594711035e+10, 4.338754675023021e+09, 6.939545151869151e+02, -4.540444941343805e+04, -3.773696839065323e+03); 
    // Venus
    planets[1] = initPlanet(48.685e23, -1.070725166099876e+11, 1.139346175891170e+10, 6.345074050304741e+09, -4.155817640337680e+03, -3.494715982962389e+04, -2.399645931443928e+02); 
    // Terre
    planets[2] = initPlanet(5.97219e24, -2.982262782007100e+10, -1.497712589089212e+11, 2.524112971142679e+07, 2.875115297834278e+04, -5.912322191541242e+03, -9.010199215850889e-01);
    // Mars
    planets[3] = initPlanet(6.4171e23, 1.948457196881133e+11, 8.312623598954162e+10, -3.009652509615280e+09, -8.633422290015989e+03, 2.433146747698761e+04, 7.215861409413744e+02); 
    // Jupiter
    planets[4] = initPlanet(18.9819e26, -4.185842907497987e+11, 6.672393758476599e+11, 6.600004253878236e+09, -1.122350194732934e+04, -6.326098818069995e+03, 2.774691500916044e02); 
    // Saturne
    planets[5] = initPlanet(5.6834e26, 1.404645175816801e+12, 1.705625338362996e+11, -5.889295729628324e+10, -1.695896396373922e+03, -9.566860941524073e+03, -9.823364202462770e01); 
    // Uranus
    planets[6] = initPlanet(86.813e24, 1.395028718386390e+12, 2.554458131338562e+12, -8.585835704688668e+09, -6.026990776370782e+03, 2.946566820385188e+03, 8.876416418802502e01); 
    // Neptune
    planets[7] = initPlanet(102.409e24, 4.465925471043573e+12, 1.523616015850787e+11, -1.060594799610975e+11, -2.218508272557781e+02, 5.464150567074379e+03, -1.069683757738009e+02); 
    // Lune
    planets[8] = initPlanet(7.349e22, -2.945076249617782e+10, -1.497169462776675e+11, 4.465133798880875e+07, 2.852887058558411e+04, -4.897018941321519e+03, 6.958124137372379e+01); 
    
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

    // Jupiter
    strcpy(files[4].path,"./temp_json_files/");
    strcat(files[4].path, prefixe);
    strcat(files[4].path,"_jupiter.json");
    files[4].file = fopen(files[4].path,"w+");
    fprintf(files[4].file,"    \"%s%s\":","jupiter_",prefixe);

    // Saturne
    strcpy(files[5].path,"./temp_json_files/");
    strcat(files[5].path, prefixe);
    strcat(files[5].path,"_saturne.json");
    files[5].file = fopen(files[5].path,"w+");
    fprintf(files[5].file,"    \"%s%s\":","saturne_",prefixe);

    // Uranus
    strcpy(files[6].path,"./temp_json_files/");
    strcat(files[6].path, prefixe);
    strcat(files[6].path,"_uranus.json");
    files[6].file = fopen(files[6].path,"w+");
    fprintf(files[6].file,"    \"%s%s\":","uranus_",prefixe);

    // Neptune
    strcpy(files[7].path,"./temp_json_files/");
    strcat(files[7].path, prefixe);
    strcat(files[7].path,"_neptune.json");
    files[7].file = fopen(files[7].path,"w+");
    fprintf(files[7].file,"    \"%s%s\":","neptune_",prefixe);

    // Lune
    strcpy(files[8].path,"./temp_json_files/");
    strcat(files[8].path, prefixe);
    strcat(files[8].path,"_lune.json");
    files[8].file = fopen(files[8].path,"w+");
    fprintf(files[8].file,"    \"%s%s\":","lune_",prefixe);

    return files;

}

/**
 * @brief Envoie les infos de la planète dans un fichier json
 * @param file Le fichier json
 * @param planet Planète à afficher
 * @param t L'instant t à laquelle les infos sont calculées
 */
void saveFile(FILE * file, Planet planet, int t){
    fprintf(file, ",\n      [[%e,%e,%e], [%lf,%lf,%lf], %d]", planet.pos.x , planet.pos.y , planet.pos.z , planet.v.x , planet.v.y , planet.v.z , t);
}

/**
 * @brief Sauvegarde le contenu des fichiers temporaires dans le fichier methodes et les supprime
 * @param methodes Le fichier "methodes"
 * @param listeFiles La liste de fichier temporaires
 */
void saveToMain(FILE * methodes, TempFILE * listeFiles){
    exportFile(methodes,listeFiles[0].file);
    fclose(listeFiles[0].file);
    remove(listeFiles[0].path);

    for (int i = 1 ; i < N_PLANETS ; i++){
        fprintf(methodes,",\n");
        exportFile(methodes,listeFiles[i].file);
        fclose(listeFiles[i].file);
        remove(listeFiles[i].path);
    }
}

/**
 * @brief Sauvegarde les informations dans un fichier temporaire afin de libérer de la ram
 * @param planet_list Liste des planètes
 * @param TempFILE Liste des fichiers temporaires
 * @param jours Nombre de jours à calculer
 * @param f Fonction à utiliser
 */
void createTempFiles(Planet * planet_list, TempFILE * files_list, float jours, void (*f)(Planet *, int)){
    Planet temp_planet;

    // boucle ajout des infos dans le bon format pour t = 0
    for (int i = 0; i < N_PLANETS; i++){
        temp_planet = planet_list[i];
        fprintf(files_list[i].file,"[[[%e,%e,%e], [%e,%e,%e], %d]", temp_planet.pos.x , temp_planet.pos.y , temp_planet.pos.z , temp_planet.v.x , temp_planet.v.y , temp_planet.v.z , 0);
    }

    // boucle ajout des infos dans le bon format pout t=1 à t=tmax
    for (int i = 1; i < (S_TO_D/PAS_SAUVEGARDE) * jours; i++){
        for (int j = 0; j < PAS_SAUVEGARDE/PAS_REEL; j++) f(planet_list, PAS_REEL);
        for (int j = 0; j < N_PLANETS; j++){
            saveFile(files_list[j].file, planet_list[j], i);
        }
    }
    for (int i = 0; i < N_PLANETS; i++){
        fprintf(files_list[i].file,"]");
        
    }
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
    printf("[[%e,%e,%e], [%e,%e,%e], %d]\n", planet.pos.x , planet.pos.y , planet.pos.z , planet.v.x , planet.v.y , planet.v.z , t);
    // [[x,y,z], [vx,vy,vz], t]
}