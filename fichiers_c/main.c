#include "planetes.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    Planet mercury = initPlanet(3.285e23,4.700000e+10,0,0,0,5.897569e+04,0);

    printf("Euler :\n\n");

    print_debug(mercury,0);
    for (int i = 0 ; i < 3 ; i++){
        euler(&mercury);
        print_debug(mercury,i);
    }

    printf("K-R :\n\n");
    mercury = initPlanet(3.285e23,4.700000e+10,0,0,0,5.897569e+04,0);
    print_debug(mercury,0);
    for (int i = 0 ; i < 3 ; i++){
        euler(&mercury);
        print_debug(mercury,i);
    }
    return 0;
}