#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "inputs.h"
#include "def.h"

double alpha, beta, profileGamma, aCorr, grav, invZ, albedo, emissiv, karman; 
double measureZ, windU, surfaceT, airT, specificQ, precip, longwave, solar, density;
double surfaceTPrevious, surfaceQPrevious;
double wg, coefEN, coefHN, roughZ;
double dt, volZ, refresh, deltaQCoef;
double period;
double humidityPrint;

double precipBegin;
double precipEnd;


CoareData takeInputs()
{
    CoareData out;
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
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%s", dummy);
    fscanf(inputFile, "%lf", &humidityPrint);

    out.alpha = alpha;
    out.gamma = profileGamma;
    out.a = aCorr;
    out.g = grav;
    out.karman = karman;
    out.measureZ = measureZ;
    out.u = windU;
    out.surfaceT = surfaceT;
    out.airT = airT;
    out.q = specificQ;
    out.rho = density;
    out.wgGuess = wg;
    out.cEN = coefEN;
    out.cHN = coefHN;
    out.z0 = roughZ;

    out.dt = dt;
    out.period = period;
    out.dqCoef = deltaQCoef;

    out.humidCutoff = humidityPrint;

    return out;
}