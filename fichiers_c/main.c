#include "planetes.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    printf("[DEBUT DU PROGRAMME]\n\n");

    Planet * planets = malloc(sizeof(Planet)*N_PLANETS);
    // Mercure
    planets[0] = initPlanet(3.302e23, -5.939249500394277E+10, -1.424142594711035E+10, 4.338754675023021E+09, 6.939545151869151E+02, -4.540444941343805E+04, -3.773696839065323E+03); 
    // Venus
    planets[1] = initPlanet(48.685e23, -1.070725166099876E+11, 1.139346175891170E+10, 6.345074050304741E+09, -4.155817640337680E+03, -3.494715982962389E+04, -2.399645931443928E+02); 
    // Terre
    planets[2] = initPlanet(285e23, 4.700000e+10, 0, 0, 0, 5.897569e+04, 2);
    // Mars
    planets[3] = initPlanet(285e23, 4.700000e+10, 0, 0, 0, 5.897569e+04, 3); 
    // Jupiter
    // Saturne
    // Uranus
    // Neptune
    // Pluton
    
    printf("Euler :\n\n");

    print_debug(planets[0],0);
    for (int i = 1 ; i < 11 ; i++){
        euler(&planets[0]);
        print_debug(planets[0],i);
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