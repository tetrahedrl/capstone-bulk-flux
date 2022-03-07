#include "inputs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double alpha, beta, profileGamma, aCorr, grav, invZ, albedo, emissiv, karman; 
double measureZ, windU, surfaceT, airT, specificQ, precip, longwave, solar, density;
double surfaceTPrevious, surfaceQPrevious;
double wg, coefEN, coefHN, roughZ;
double dt, volZ, refresh, deltaQCoef;
double period;

double precipBegin;
double precipEnd;


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
    fscanf(inputFile, "%lf", &measureZ);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &windU);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &surfaceT);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &airT);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &specificQ);
    //fscanf(inputFile, "%s", dummy);
    //fscanf(inputFile, "%lf", &precip);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &longwave);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &solar);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &density);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &wg);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &coefEN);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &coefHN);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &roughZ);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &dt);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &period);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &volZ);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &refresh);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &deltaQCoef);

    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &precipBegin);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &precipEnd);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &precip);


}