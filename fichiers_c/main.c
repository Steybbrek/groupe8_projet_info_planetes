#include "planetes.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    printf("[DEBUT DU PROGRAMME]\n\n");

    Planet * planets = NULL;
    Planet temp_planet;
    planets = reset(planets);
    

    // Euler - utilisé uniquement pour le visuel - il faudrait entièrement refaire la lune pour juste cette fonction
    printf("Euler :\n\n");

    print_debug(planets[0],0);
    for (int i = 1 ; i < 11 ; i++){
        euler(&planets[0]);
        print_debug(planets[0],i);
    }

    // Euler Interact
    TempFILE * listeEulerInteract = malloc(N_PLANETS * sizeof(TempFILE));
    FILE * methodes = fopen("../fichiers_json/methodes.json","w");
    fprintf(methodes, "{\n");
    
    planets = reset(planets);
    listeEulerInteract = initFiles(listeEulerInteract,"eulerInteract");
    
    eulerInteractTempFile(planets,listeEulerInteract,365.25);

    saveToMain(methodes, listeEulerInteract);

    free(listeEulerInteract);

    fprintf(methodes,"\n}");


/*     
    printf("R-K :\n\n");
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