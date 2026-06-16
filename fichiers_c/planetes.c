/**
 * @file planetes.c
 * @brief Un ensemble de fonctions pour estimer les trajectiores d'objets dans l'espace
 * @date 2026-06-16
 */

// LIBRAIRIES

#include "planetes.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
###############################################################################################################
################################################# OUTILS VECT #################################################
###############################################################################################################
*/



/**
 * @brief Renvoie la norme
 * @param vecteur Vecteur sur R³ 
 */
double norme(Vect vecteur){
    return sqrt(vecteur.x*vecteur.x + vecteur.y*vecteur.y + vecteur.z*vecteur.z);
}

/**
 * @brief Renvoie la somme de 2 vecteurs
 * @param u Vecteur u
 * @param v Vecteur v
 * @return Le vecteur somme des 2 autres vecteurs
 */
Vect sumVect(Vect u, Vect v){
    Vect sum;
    sum.x = u.x + v.x;
    sum.y = u.y + v.y;
    sum.z = u.z + v.z;
    return sum;
}

/**
 * @brief Somme de vecteurs (au moins 1 vect)
 * @param list_vect Liste des vecteurs
 * @param n Nombre de vecteurs
 * @return Somme des vecteurs
 */
Vect sumListVect(Vect * list_vect, int n){
    Vect sum = list_vect[0];
    for (int i = 1; i < n ; i++){
        sum.x += list_vect[i].x;
        sum.y += list_vect[i].y;
        sum.z += list_vect[i].z;
    }
    return sum;
}

/**
 * @brief Soustrait un vecteur à un autre
 * @param a Premier vecteur, valeur ajoutée
 * @param b Deuxième vecteur, valeur soustraite
 * @return Le vecteur résultant
 */
Vect subtractVect(Vect a, Vect b){
    Vect ab;
    ab.x = a.x - b.x;
    ab.y = a.y - b.y;
    ab.z = a.z - b.z;
    return ab;
}

/**
 * @brief Renvoie un vectu=eur multiplié par un scalaire
 * @param vecteur Le vecteur à multipler
 * @param scalaire Le scalaire multiplicateur
 */
Vect multVectScal(Vect vecteur, double scalaire){
    vecteur.x *= scalaire;
    vecteur.y *= scalaire;
    vecteur.z *= scalaire;
    return vecteur;
}


/*
###############################################################################################################
################################################ ACCELERATIONS ################################################
###############################################################################################################
*/


/**
 * @brief Calcule la variable à multiplier par l'accélération en x, y et z
 * @param planet La planète dont on cherche l'accélération
 */
double acceleration(Planet planet){
    double r = norme(planet.pos);
    return (-G * mS / (r*r*r));
}

/**
 * @brief Calcule les forces appliquer sur l'objet par les autres objets
 * @param planets Liste des planètes
 * @param id_target Id de la planète que l'on étudie
 */
Vect accelerationInteract(Planet *planets, int id_target){
    Vect a;
    Vect r;
    a.x = 0;
    a.y = 0;
    a.z = 0;
    double norme_r;
    double force;

    for (int i = 0 ; i < N_PLANETS ; i++){
        if (i != id_target){
            r = subtractVect(planets[id_target].pos,planets[i].pos);
            norme_r = norme(r);
            force = (-G * planets[i].m / (norme_r * norme_r * norme_r));

            a = sumVect(a, multVectScal(r,force));
        }
    }

    //Soleil

    r = planets[id_target].pos;
    norme_r = norme(r);
    force = (-G * mS / (norme_r * norme_r * norme_r));
    a = sumVect(a, multVectScal(r,force));

    return a;
}


/*
###############################################################################################################
################################################### ENERGIES ##################################################
###############################################################################################################
*/


/**
 * @brief Calcul de l'Energie potentielle d'une planète
 * @param planetes Liste de planètes
 * @param id_target L'id de la planète cible
 */
double Ep(Planet * planets, int id_target){
    
    Vect r;
    double norme_r;
    double E = 0;
    Planet target = planets[id_target];

    for (int i = 0 ; i < N_PLANETS ; i++){
        if (i != id_target){
            r = subtractVect(target.pos,planets[i].pos);
            norme_r = norme(r);
            E = (-G * planets[i].m  * target.m / (norme_r * norme_r * norme_r));
        }
    }

    //Soleil

    r = target.pos;
    norme_r = norme(r);
    E = (-G * mS * target.m / (norme_r * norme_r * norme_r));
    return E;
}

/**
 * @brief Calcule l'énéregie cinétique d'une planète
 * @param target Planète cible
 */
double Ec(Planet target){
    double v = norme(target.v);
    return target.m * v * v;
}

/**
 * @brief Calcule l'énergie du système et si la marge d'erreur est dépassée, écrit un message dans un fichier
 * @param planets Liste de planètes
 * @param Esys Pointeur sur l'énergie cynétique précédente
 * @param jour Nombre de jour qui s'est écoulé
 * @param fileEnergie Fichier d'enregistrement
 */
void ESysteme(Planet * planets, double * Esys, int jour, FILE * fileEnergie){
    double Etot = 0;

    for (int i = 0; i < N_PLANETS ; i++){
        Etot += Ep(planets, i) + Ec(planets[i]);
    }

    if(*Esys == -3.1415926535) *Esys = Etot;

    if (Etot > *Esys * (1+MARGE_ENERGIE*0.01) || Etot < *Esys * (1-MARGE_ENERGIE*0.01)){
        fprintf(fileEnergie,"Variation d'énergie de plus de %lf%% détectée au jour %d, valeur exacte : %lf%%\n\n", MARGE_ENERGIE, jour, ((Etot - *Esys) / *Esys) * 100);
    } 

    *Esys = Etot;
}

/*
###############################################################################################################
################################################## METHODES ###################################################
###############################################################################################################
*/

// Méthodes Euler

/**
 * @brief Calcule les informations d'une planète après pas secondes
 * @param planet La planète dont on cherche les infos.
 * @param pas Pas en secondes
 */
void euler(Planet * planet, int pas){

    // Calcul de l'accélération à t-1
    double a = acceleration(*planet);

    planet->a = multVectScal(planet->pos,a);

    // position en t

    planet->pos = sumVect(multVectScal(planet->v,pas),planet->pos);

    // vitesse en t

    planet->v = sumVect(multVectScal(planet->a, pas),planet->v);

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

    // Boucle accélérations
    for (int i = 0; i < N_PLANETS ; i++){
        a_list[i] = accelerationInteract(planets,i);
    }

    // Boucle ajout informations
    for (int i = 0 ; i < N_PLANETS ; i++){
        planet = &(planets[i]);

        // accélération à t-1
    
        planet->a = a_list[i];

        // position en t

        planet->pos = sumVect(multVectScal(planet->v,pas),planet->pos);

        //  vitesse en t

        planet->v = sumVect(multVectScal(planet->a, pas),planet->v);
    }
}

/**
 * @brief Calcule les informations avec la méthode d'Euler et des intéraction avec les autres planètes
 * @param planets Liste des planètes
 * @param pas Pas en secondes
 */
void eulerAssymInteract(Planet * planets, int pas){
    Vect  a_list[N_PLANETS];
    Planet * planet;
    double a_sol;


    // Boucle ajout informations positions
    for (int i = 0 ; i < N_PLANETS ; i++){
        planet = &(planets[i]);

        //  position en t

        planet->pos = sumVect(multVectScal(planet->v,pas),planet->pos);
    }

    // Boucle accélérations
    for (int i = 0; i < N_PLANETS ; i++){
        a_list[i] = accelerationInteract(planets,i);
    }

    // Boucle ajout informations restantes
    for (int i = 0 ; i < N_PLANETS ; i++){  
        planet = &(planets[i]);
        // accélération à t-1

        planet->a = a_list[i];

        // vitesse en t

        planet->v = sumVect(multVectScal(planet->a, pas),planet->v);
    }
}

// Méthode RK

/**
 * @brief La fonction f de la méthode RK (f(pos,v) ~= a)
 * @param planets Liste des planètes
 * @return Liste des k
 */
KList fRK(Planet * planets){
    KList k;
    for (int i = 0 ; i < N_PLANETS ; i++){
        k.vect[i] = accelerationInteract(planets, i);
    }
    return k;
}

/**
 * @brief Calcul de K1 de la méthode RK
 * @param planets Liste des planètes
 * @return Liste des k1
 */
KList calcK1(Planet * planets){
    return fRK(planets);
}

/**
 * @brief Calcul de K2 de la méthode RK
 * @param planets Liste des planètes
 * @param k1 K précédent
 * @param h intervalle
 * @return Liste des k2
 */
KList calcK2(Planet * planets, KList k1, double h){
    Planet planetus[N_PLANETS];
    for (int i = 0 ; i < N_PLANETS; i++) {
        planetus[i] = planets[i];
        // pos + v x h/2
        planetus[i].pos = sumVect(planets[i].pos, multVectScal(planets[i].v, h/2.0));
        // v + k1 x h/2
        planetus[i].v = sumVect(planets[i].v, multVectScal(k1.vect[i], h/2.0));
    }
    return fRK(planetus);
}

/**
 * @brief Calcul de K3 de la méthode RK
 * @param planets Liste des planètes
 * @param k1 K précédent le précéndent (précent² ?)
 * @param k2 K précédent
 * @param h intervalle
 * @return Liste des k3
 */
KList calcK3(Planet * planets, KList k1, KList k2, double h){
    Planet planetos[N_PLANETS];
    for (int i = 0 ; i < N_PLANETS; i++) {
        planetos[i] = planets[i];
        // pos + h/2 x v + k1 x h²/4
        planetos[i].pos = sumVect(planets[i].pos, sumVect(multVectScal(planets[i].v, h/2.0), multVectScal(k1.vect[i], h*h/4.0)));
        // v + h/1 x k2
        planetos[i].v = sumVect(planets[i].v, multVectScal(k2.vect[i], h/2.0));
    }
    return fRK(planetos);
}

/**
 * @brief Calcul de K4 de la méthode RK
 * @param planets Liste des planètes
 * @param k2 K précédent le précéndent (précent² ?)
 * @param k3 K précédent
 * @param h intervalle
 * @return Liste des k4
 */
KList calcK4(Planet * planets, KList k2, KList k3, double h){
    Planet planetis[N_PLANETS];
    for (int i = 0; i < N_PLANETS; i++) {
        planetis[i] = planets[i];
        // pos + h x v + h²/2 x k2
        planetis[i].pos = sumVect(planets[i].pos, sumVect(multVectScal(planets[i].v, h), multVectScal(k2.vect[i], h*h/2.0)));
        // v + h x k3
        planetis[i].v = sumVect(planets[i].v, multVectScal(k3.vect[i], h));
    }
    return fRK(planetis);
}

/**
 * @brief Fonction RK de niveau(?) 4
 * @param planets Liste des planetes
 * @param pas Le pas (en sec)
 */
void rk4(Planet * planets, int pas){
    double h = (double)pas;
    
    // K1 à k4 pour pouvoir respirer

    KList k1 = calcK1(planets);
    KList k2 = calcK2(planets, k1, h);
    KList k3 = calcK3(planets, k1, k2, h);
    KList k4 = calcK4(planets, k2, k3, h);

    // pas très beau mais application de la formule finale
    for (int i = 0; i < N_PLANETS; i++) {

        // k1 à k3 pour la position des objets
        Vect kPos[3] = {k1.vect[i], k2.vect[i], k3.vect[i]};
        // k1, 2k2, 2k3 et k4 pour la vitesse des objets
        Vect kV[6] = {k1.vect[i], k2.vect[i], k2.vect[i], k3.vect[i], k3.vect[i], k4.vect[i]};

        // pos + h x v + (h²/6) x (k1 + k2 + k3)
        planets[i].pos = sumVect(planets[i].pos, sumVect(multVectScal(planets[i].v, h), multVectScal(sumListVect(kPos, 3), h*h / 6.0)));
        // v + (h/6) x (k1 + 2 x k2 + 2 x k3 + k4)
        planets[i].v   = sumVect(planets[i].v, multVectScal(sumListVect(kV, 6), h / 6.0));
    }
}


/*
###############################################################################################################
############################################### OUTILS PLANETES ###############################################
###############################################################################################################
*/


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
    if(planets == NULL){
        fprintf(stderr, "ERROR : \n    - TYPE : Realoc returned NULL pointer \n    - LOCATION : Function reset\n");
        exit(EXIT_FAILURE);
    }

    // Pas très beau mais écriture planète par planète des coordonées du vecteur position et des coordonées du vecteur vitesse

    // Mercure
    planets[0] = initPlanet(3.302e+23, -5.939249500394277e+10, -1.424142594711035e+10, 4.338754675023021e+09, 6.939545151869151e+02, -4.540444941343805e+04, -3.773696839065323e+03); 
    // Venus
    planets[1] = initPlanet(48.685e+23, -1.070725166099876e+11, 1.139346175891170e+10, 6.345074050304741e+09, -4.155817640337680e+03, -3.494715982962389e+04, -2.399645931443928e+02); 
    // Terre
    planets[2] = initPlanet(5.97219e+24, -2.982262782007100e+10, -1.497712589089212e+11, 2.524112971142679e+07, 2.875115297834278e+04, -5.912322191541242e+03, -9.010199215850889e-01);
    // Mars
    planets[3] = initPlanet(6.4171e+23, 1.948457196881133e+11, 8.312623598954162e+10, -3.009652509615280e+09, -8.633422290015989e+03, 2.433146747698761e+04, 7.215861409413744e+02); 
    // Jupiter
    planets[4] = initPlanet(18.9819e+26, -4.185842907497987e+11, 6.672393758476599e+11, 6.600004253878236e+09, -1.122350194732934e+04, -6.326098818069995e+03, 2.774691500916044e02); 
    // Saturne
    planets[5] = initPlanet(5.6834e+26, 1.404645175816801e+12, 1.705625338362996e+11, -5.889295729628324e+10, -1.695896396373922e+03, -9.566860941524073e+03, -9.823364202462770e01); 
    // Uranus
    planets[6] = initPlanet(86.813e+24, 1.395028718386390e+12, 2.554458131338562e+12, -8.585835704688668e+09, -6.026990776370782e+03, 2.946566820385188e+03, 8.876416418802502e01); 
    // Neptune
    planets[7] = initPlanet(102.409e+24, 4.465925471043573e+12, 1.523616015850787e+11, -1.060594799610975e+11, -2.218508272557781e+02, 5.464150567074379e+03, -1.069683757738009e+02); 
    // Lune
    planets[8] = initPlanet(7.349e+22, -2.945076249617782e+10, -1.497169462776675e+11, 4.465133798880875e+07, 2.852887058558411e+04, -4.897018941321519e+03, 6.958124137372379e+01); 
    // Pluton
    planets[9] = initPlanet(1.307e+22, 2.940887999796702e+12, -4.412230338721893e12, -3.785424305645955e+11, 4.692149944304331e+03, 1.819815776140067e+03, -1.543874510774366e+03);
    // Io
    planets[10] = initPlanet(8.9319e+22,-4.188515900625595e+11, 6.675634713353819e+11, 6.607892629425406e+09, -2.466690728686926e+04, -1.736153197499167e+04, -3.115384508508399e+02);
    // Europe
    planets[11] = initPlanet(4.79984e+22, -4.182733382877014e+11, 6.666518649312292e+11, 6.588482399394184e+09, 1.043704246243166e+03, 1.294711883090337e+02, 7.266539341548072e+02);
    // Ganymède
    planets[12] = initPlanet(1.4819e+23, -4.189886583473843e+11, 6.682306626902583e+11, 6.631999734327227e+09, -2.129782925211370e+04, -1.040860637679656e+04, -2.526699431222390e+01);
    // Callisto
    planets[13] = initPlanet(1.08e+23, -4.176605255931420e+11, 6.656047355796517e+11, 6.561266624569923e+09, -4.092475764624735e+03, -2.237450867770709e+03, 5.015659323112426e+02);

    return planets;
}


/*
###############################################################################################################
############################################### OUTILS FICHIERS ###############################################
###############################################################################################################
*/

/**
 * @brief Initialise un fichier json temporaire pour le stockage de valeurs
 * @param p_files Pointeur sur TempFILE
 * @param prefixe Le prefixe / la méthode dans le nom de fichier
 * @param name Le nom de la planète / de l'objet
 * @note Nombre d'essaies pour créer la fonction : 1
 */
void initiateFileData(TempFILE * p_file, char * prefixe, char * name){
    // fichier json => ./temp_json_files/temp-prefixe-name.json
    strcpy(p_file->path,"./temp_json_files/temp-");
    strcat(p_file->path, prefixe);
    strcat(p_file->path,"-");
    strcat(p_file->path, name);
    strcat(p_file->path, ".json");
    p_file->file = fopen(p_file->path,"w+");
    // "name-prefixe": <= début fichier json
    fprintf(p_file->file,"    \"%s-%s\":",name,prefixe);
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
    if (files == NULL){
        fprintf(stderr, "ERROR : \n    - TYPE : Realoc returned NULL pointer \n    - LOCATION : Function initFiles\n");
        exit(EXIT_FAILURE);
    }

    initiateFileData(&files[0], prefixe, "mercure");

    initiateFileData(&files[1], prefixe, "venus");
    
    initiateFileData(&files[2], prefixe, "terre");

    initiateFileData(&files[3], prefixe, "mars");

    initiateFileData(&files[4], prefixe, "jupiter");

    initiateFileData(&files[5], prefixe, "saturne");

    initiateFileData(&files[6], prefixe, "uranus");

    initiateFileData(&files[7], prefixe, "neptune");

    initiateFileData(&files[8], prefixe, "lune");

    initiateFileData(&files[9], prefixe, "pluton");

    initiateFileData(&files[10], prefixe, "io");

    initiateFileData(&files[11], prefixe, "europe");

    initiateFileData(&files[12], prefixe, "ganymede");

    initiateFileData(&files[13], prefixe, "callisto");

    return files;

}

/**
 * @brief Envoie les infos de la planète dans un fichier json
 * @param file Le fichier json
 * @param planet Planète à afficher
 * @param t L'instant t à laquelle les infos sont calculées
 */
void saveFile(FILE * file, Planet planet, int t){
    fprintf(file, ",\n      [[%e,%e,%e], [%e,%e,%e], %d]", planet.pos.x , planet.pos.y , planet.pos.z , planet.v.x , planet.v.y , planet.v.z , t);
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
 * @param energie Fichier de sauvegarde pour l'énergie
 */
void createTempFiles(Planet * planet_list, TempFILE * files_list, float jours, void (*f)(Planet *, int), FILE * energie){
    Planet temp_planet;
    int progress = 0;
    double Esys = -3.1415926535;

    // boucle ajout des infos dans le bon format pour t = 0
    for (int i = 0; i < N_PLANETS; i++){
        temp_planet = planet_list[i];
        fprintf(files_list[i].file,"[[[%e,%e,%e], [%e,%e,%e], %d]", temp_planet.pos.x , temp_planet.pos.y , temp_planet.pos.z , temp_planet.v.x , temp_planet.v.y , temp_planet.v.z , 0);
    }
    if(LOADING_BAR)printf("[--------------------] - 0%%");

    // boucle ajout des infos dans le bon format pout t=1 à t=tmax
    for (int i = 1; i < (DAY_TO_SEC * jours) / PAS_SAUVEGARDE; i++){
        // boucle pas reel
        for (int j = 0; j < PAS_SAUVEGARDE / PAS_REEL; j++){
            f(planet_list, PAS_REEL);
            print_loading_bar((i - 1) * PAS_SAUVEGARDE / PAS_REEL + j, (DAY_TO_SEC * jours) / PAS_REEL, &progress);
        }
        for (int j = 0; j < N_PLANETS; j++){
            saveFile(files_list[j].file, planet_list[j], i);
        }
        if(ENERGIE) ESysteme(planet_list,&Esys,i,energie);
    }
    for (int i = 0; i < N_PLANETS; i++){
        fprintf(files_list[i].file,"]");
        
    }
    progress = 0;
    print_loading_bar(100,100,&progress);
    if(LOADING_BAR)printf("\n");
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


/*
###############################################################################################################
################################################ FONCTION DEBUG ###############################################
###############################################################################################################
*/

/**
 * @brief Barre de chargement
 * @param actuel Pas actuel
 * @param total Pas total à faire
 * @param progress Progres du chargement
 * @note Adaptation du code de Karl M.P.
 */
void print_loading_bar(double actuel, double total, int * progress){  
    if (LOADING_BAR){
        double div = (int)((actuel / total) * 10000);
        int rempli = (int)(div / 500);

        if (*progress != div){
            printf("\r[");

            for (int i = 0; i < rempli; i++){
                printf("#");
            }
            for (int i = 0; i < 20 - rempli; i++){
                printf("-");
            }

            printf("] - %6.2lf%%", div/100.00);
            *progress = div;
        }
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

/**
 * @brief Affiche les options sélectionnés pour l'execution
 */
void print_options(){
    printf("Options séléctionnées :\n");
    printf("   - Barre de chargement :              %d\n",LOADING_BAR);
    printf("   - Calculs d'énergie :                %d\n",ENERGIE);
    printf("   - Méthode d'Euler - Terminal :       %d\n",EULER_T);
    printf("   - Méthode d'Euler - Fichier :        %d\n",EULER);
    printf("   - Méthode d'Euler Assymétrique :     %d\n",EULER_ASSYM);
    printf("   - Méthode de Runge-Kutta 4 :         %d\n",RK4);

    printf("\nValeurs des constantes :\n");
    printf("   - Nombre de planètes :               %d\n",N_PLANETS);
    printf("   - Période d'enregistrement :         %.2lf jours\n",PERIODE_ENREGISTREMENT);
    printf("   - Pas de sauvegarde :                %d sec\n",PAS_SAUVEGARDE);
    printf("   - Pas reel :                         %d sec\n",PAS_REEL);
    printf("   - Marge d'erreur énergie du sytème : %lf%%\n",MARGE_ENERGIE);
    printf("\n");
}

/**
 * @brief Affiche le contenu du fichier
 * @param file Le fichier à lire
 */
void print_file(FILE * file){
    fseek(file,0,SEEK_SET);
    char c = fgetc(file);
    while(c!=EOF){
        printf("%c",c);
        c = fgetc(file);
    }
}