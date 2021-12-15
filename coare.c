#include "inputs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern int time;
extern double alpha, beta, profileGamma, aCorr, grav, invZ, albedo, emissiv, karman; 
extern double measureZ, windU, surfaceT, airT, specificQ, precip, longwave, solar;
extern double surfaceTPrevious, surfaceQPrevious;

int main()
{

    takeInputs();
    printf("%lf", profileGamma);
    return 0;

}
