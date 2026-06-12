#include "planetes.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    printf("[DEBUT DU PROGRAMME]\n\n");

    Planet * planets = NULL;
    TempFILE * listTempFiles = malloc(N_PLANETS * sizeof(TempFILE));
    FILE * methodes = fopen("../web/methodes.json","w");
    fprintf(methodes, "{\n");
    planets = reset(planets);
    

    // Euler - utilisé uniquement pour le visuel - il faudrait entièrement refaire la lune pour juste cette fonction
    printf("Euler :\n\n");

    print_debug(planets[0],0);
    for (int i = 1 ; i < 11 ; i++){
        euler(&planets[0],PAS_REEL);
        print_debug(planets[0],i);
    }

    // Euler Interact
    
    printf("\n[DEBUT] - Euler\n");

    planets = reset(planets);
    listTempFiles = initFiles(listTempFiles,"euler");
    
    createTempFiles(planets,listTempFiles,PERIODE_ENREGISTREMENT,eulerInteract);

    saveToMain(methodes, listTempFiles);

    fprintf(methodes,",\n"); // Colle les listes

    printf("[ FIN ] - Euler\n");

    // Euler Asymétrique

    printf("\n[DEBUT] - Euler Asymétrique\n");

    planets = reset(planets);
    listTempFiles = initFiles(listTempFiles,"eulerAsym");
    
    createTempFiles(planets,listTempFiles,PERIODE_ENREGISTREMENT,eulerAsymInteract);

    saveToMain(methodes, listTempFiles);

    fprintf(methodes,",\n"); // Colle les listes

    printf("[ FIN ] - Euler Asymétrique\n");

    // R-K4
    
    printf("\n[DEBUT] - R-K 4\n");

    planets = reset(planets);
    listTempFiles = initFiles(listTempFiles,"RK4");
    
    createTempFiles(planets,listTempFiles,PERIODE_ENREGISTREMENT,rk4);

    saveToMain(methodes, listTempFiles);

    printf("[ FIN ] - R-K 4\n");







    // End prog
    free(planets);
    free(listTempFiles);
    fprintf(methodes,"\n}");
    fclose(methodes);



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