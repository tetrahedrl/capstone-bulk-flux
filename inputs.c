#include "inputs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int time;
double alpha, beta, profileGamma, aCorr, grav, invZ, albedo, emissiv, karman; 
double measureZ, windU, surfaceT, airT, specificQ, precip, longwave, solar, pressure;
double surfaceTPrevious, surfaceQPrevious;
double wg, coefEN, coefHN, roughZ;


void takeInputs()
{

    char dummy[80]; 
    FILE* inputFile;
    inputFile = fopen ("inputs.txt", "r");
    
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &alpha);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &beta);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &profileGamma);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &aCorr);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &grav);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &invZ);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &albedo);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &emissiv);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &karman);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%d", &time);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &measureZ);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &windU);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &surfaceT);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &airT);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &specificQ);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &precip);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &longwave);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &solar);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &pressure);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &wg);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &coefEN);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &coefHN);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &roughZ);
    
    
    /*

    printf("Alpha\n%lf\n", alpha);
    printf("Convection Beta\n%lf\n", beta);
    printf("Profile profileGamma\n%lf\n", profileGamma);
    printf("Correction a\n%lf\n", aCorr);
    printf("g acceleration\n%lf\n", grav);
    printf("Inversion Height\n%lf\n", invZ);
    printf("Albedo\n%lf\n", albedo);
    printf("Emissivity\n%lf\n", emissiv);
    printf("Von Karman Constant\n%lf\n\n", karman);

    printf("Time\n%d\n", time);
    printf("Measurement height\n%lf\n", measureZ);
    printf("Wind Velocity U\n%lf\n", windU);
    printf("Surface T\n%lf\n", surfaceT);
    printf("Air T\n%lf\n", airT);
    printf("Specific Humidity\n%lf\n", specificQ);
    printf("Precipitation Rate\n%lf\n", precip);
    printf("Longwave Flux\n%lf\n", longwave);
    printf("Solar Flux\n%lf\n", solar);

    */


/*    while (fscanf(inputFile, "%lf \n", &inTest) == 1)
    {
        printf("%lf\n", inTest);
    }
*/

}