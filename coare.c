#include "inputs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern int time;
extern double alpha, beta, profileGamma, aCorr, grav, invZ, albedo, emissiv, karman; 
extern double measureZ, windU, surfaceT, airT, specificQ, precip, longwave, solar;
extern double surfaceTPrevious, surfaceQPrevious;
extern double wg, coefEN, coefHN, coefDN, coefTN, coefQN;

double viscAir, enthalpyL;

int main()
{

    takeInputs();
    printf("Air T\n%lf", airT);

    viscAir = 0.00001326 * (1 + 0.006542 * airT + 0.000008301 * pow(airT, 2) - 0.00000000484 * pow(airT, 3));
    printf("\nAir Viscosity\n%lf", viscAir);

    enthalpyL = 100000 * (25 - 0.02274 * airT);
    printf("\nLatent Heat of Vaporization\n%lf", enthalpyL);

    //printf("\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf", wg, coefEN, coefHN, coefDN, coefTN, coefQN);

}
