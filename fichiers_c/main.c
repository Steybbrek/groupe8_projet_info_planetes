/**
 * @file main.c
 * @date 2026-06-15
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
    if (methodes == NULL){
        fprintf(stderr, "ERROR : \n    - TYPE : fopen returned NULL pointer \n    - LOCATION : main, methodes\n");
        exit(EXIT_FAILURE);
    }

    FILE * energie = (ENERGIE) ? fopen("./energie.txt","w+") : NULL;
    if (methodes == NULL && ENERGIE){
        fprintf(stderr, "ERROR : \n    - TYPE : fopen returned NULL pointer \n    - LOCATION : main, energie\n");
        exit(EXIT_FAILURE);
    }

    fprintf(methodes, "{\n");
    planets = reset(planets);
    

    print_options();


    // Euler - utilisé uniquement pour être affiché dans le terminal - il faudrait entièrement refaire les satélites pour juste cette fonction
    if(EULER_T){
        printf("Euler :\n\n");
        print_debug(planets[0],0);
        for (int i = 1 ; i < 11 ; i++){
            euler(&planets[0],PAS_REEL);
            print_debug(planets[0],i);
        }
    }



    // Euler Interact
    if(EULER){
        printf("\n[DEBUT] - Euler\n");

        if(ENERGIE) fprintf(energie, "Méthode d'Euler :\n");

        planets = reset(planets);
        listTempFiles = initFiles(listTempFiles,"euler");
        createTempFiles(planets,listTempFiles,PERIODE_ENREGISTREMENT,eulerInteract, energie);
        saveToMain(methodes, listTempFiles);
        
        printf("[ FIN ] - Euler\n");
    }



    // Euler Assymétrique
    if(EULER_ASSYM){        
        printf("\n[DEBUT] - Euler Assymétrique\n");

        if(EULER) fprintf(methodes,",\n"); // Colle les listes

        if(ENERGIE) fprintf(energie, "\nMéthode d'Euler Assymétrique :\n");

        planets = reset(planets);
        listTempFiles = initFiles(listTempFiles,"eulerAssym");   
        createTempFiles(planets,listTempFiles,PERIODE_ENREGISTREMENT,eulerAssymInteract, energie);
        saveToMain(methodes, listTempFiles);

        
        printf("[ FIN ] - Euler Assymétrique\n");
    }




    // Runge-Kutta 4
    if(EULER_ASSYM){        
    
        printf("\n[DEBUT] - Runge-Kutta 4\n");

        if(EULER || EULER_ASSYM) fprintf(methodes,",\n"); // Colle les listes

        if(ENERGIE) fprintf(energie, "\nMéthode de Runge-Kutta 4 :\n");

        planets = reset(planets);    
        listTempFiles = initFiles(listTempFiles,"RK4");   
        createTempFiles(planets,listTempFiles,PERIODE_ENREGISTREMENT,rk4, energie);
        saveToMain(methodes, listTempFiles);
        
        printf("[ FIN ] - Runge-Kutta 4\n");
    }


    // End prog
    if(ENERGIE){
        printf("\n\n[DEBUT] - Rapport des problèmes d'énergie à %d%%\n",MARGE_ENERGIE);
        print_file(energie);
        printf("\n[ FIN ] - Rapport\n\n\n");

    }

    free(planets);
    free(listTempFiles);
    fprintf(methodes,"\n}");
    fclose(methodes);
    if(ENERGIE){
        fclose(energie);
        remove("./energie.txt");
    }
    printf("\n[FIN DU PROGRAMME]\n\n");

    return EXIT_SUCCESS; 
}