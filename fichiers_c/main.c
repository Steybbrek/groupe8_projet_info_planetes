#include "planetes.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    printf("[DEBUT DU PROGRAMME]\n\n");

    Planet * planets = NULL;
    Planet temp_planet;
    planets = reset(planets);
    

    // Euler
    printf("Euler :\n\n");

    print_debug(planets[0],0);
    for (int i = 1 ; i < 11 ; i++){
        euler(&planets[0]);
        print_debug(planets[0],i);
    }

    // Euler Interact
    FILE ** listeEulerInteract = malloc(N_PLANETS * sizeof(FILE *));
    FILE * methodes = fopen("../fichiers_json/methodes.json","w");
    fprintf(methodes, "{\n");
    
    planets = reset(planets);
    listeEulerInteract = initFiles(listeEulerInteract,"eulerInteract");
    
    for (int i = 0; i < N_PLANETS; i++){
        temp_planet = planets[i];
        fprintf(listeEulerInteract[i],"[[[%lf,%lf,%lf], [%lf,%lf,%lf], %d]", temp_planet.pos.x , temp_planet.pos.y , temp_planet.pos.z , temp_planet.v.x , temp_planet.v.y , temp_planet.v.z , 0);
    }
    for (int i = 1; i < 21; i++){
        eulerInteract(planets);
        for (int j = 0; j < N_PLANETS; j++){
            saveFile(listeEulerInteract[j], planets[j], i);
        }
    }
    for (int i = 0; i < N_PLANETS; i++){
        fprintf(listeEulerInteract[i],"]");
    }


/*     
    printf("K-R :\n\n");
    mercury = initPlanet(3.285e23,4.700000e+10,0,0,0,5.897569e+04,0);
    print_debug(mercury,0);
    for (int i = 0 ; i < 3 ; i++){
        euler(&mercury);
        print_debug(mercury,i);
    }
*/
    printf("\n[FIN DU PROGRAMME]\n\n");
    return 0; 
}