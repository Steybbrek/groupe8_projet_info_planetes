/**
 * @file main.c
 * @authors SALMON RADENAC Théo, LAMOUR Briec
 * @date 2026-06-14
 */


// LIBRAIRIES

#include "planetes.h"
#include <stdlib.h>
#include <stdio.h>




// MAIN

int main(){
    
    
    printf("[DEBUT DU PROGRAMME]\n\n");

    Planet * planets = NULL;
    TempFILE * listTempFiles = malloc(N_PLANETS * sizeof(TempFILE));
    FILE * methodes = fopen("../web/methodes.json","w");
    fprintf(methodes, "{\n");
    planets = reset(planets);
    



    // Euler - utilisé uniquement pour être affiché dans le terminal - il faudrait entièrement refaire les satélites pour juste cette fonction

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

    printf("\n[FIN DU PROGRAMME]\n\n");

    return EXIT_SUCCESS; 
}