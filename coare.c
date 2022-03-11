#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "def.h"

extern double alpha, beta, profileGamma, aCorr, grav, invZ, albedo, emissiv, karman; 
extern double measureZ, windU, surfaceT, airT, specificQ, precip, longwave, solar, density;
extern double surfaceTPrevious, surfaceQPrevious;
extern double wg, coefEN, coefHN, roughZ;

double sqrtCoefDN, sqrtCoefTN, sqrtCoefQN, sqrtCoefD, sqrtCoefT, sqrtCoefQ;
double viscAir, enthalpyL, windS, surfaceQ, potT, potDiffT, satSpecificQ, zeta, profileY;
double starT, starQ, starU, starUt;
double reynoldsR, reynoldsT, reynoldsQ;
double psiU, psiH;
double fluxS, fluxL, fluxT;
double prevL = 1;

double stability;
double gustiness;
double tau;

int i;

// return enthalpy of vaporization for temp t

double enthalpyV(double t)
{
    return 100000 * (25 - 0.02274 * t);
}

// return saturation mixing ratio for temp t
double satMix(double t) 
{
    double lv = enthalpyV(t);
    t += 273.15; // c to k
    double svp = 611.12 * exp((lv / 461.2) * ((1 / 273.15) - (1 / t))); // saturation vapor pressure
    return svp * 0.622 / (100000 - svp); // saturation mixing ratio
}

// return kinematic viscosity of air for temp t
double kinVisc(double t) 
{
    return 0.00001326 * (1 + 0.006542 * t + 0.000008301 * (t * t) - 0.00000000484 * (t * t * t));
}

double scalingParam(double coefTransfer, double total, double subtract)
{
    return -1 * sqrt(coefTransfer) * (total - subtract);
}

void calcZeta()
{
    //zeta = measureZ * ((karman * grav / (airT+273.)) * (starT + 0.61 * (airT+273.) * starQ) / (starU * starU));
    zeta = measureZ * ((karman * grav / (airT)) * (starT + 0.61 * (airT) * starQ) / (starU * starU)); //C or K?
}

void calcRoughZ()
{
    roughZ = alpha * ((starU * starU) / grav); //+ 0.11 * (viscAir / starU);
}

void reynoldsConvert()
{
    double range[7] = {0, 0.11, 0.85, 3.0, 10.0, 30.0, 100.0};
    double a[2][6] = {
        {0.177, 1.376, 1.026, 1.625, 4.661, 34.904}, 
        {0.292, 1.808, 1.393, 1.956, 4.994, 30.790}
    };
    double b[2][6] = {
        {0, 0.929, -0.599, -1.018, -1.475, -2.067},
        {0, 0.826, -0.528, -0.870, -1.297, -1.845}
    };
    for(int i = 0; i < 6; i++)
    {
        if (reynoldsR > range[i] && reynoldsR <= range[i + 1])
        {
            reynoldsT = a[0][i] * pow(reynoldsR, b[0][i]);
            reynoldsQ = a[1][i] * pow(reynoldsR, b[1][i]);
        }
    }
}

double sqrtNeutrals(double reynolds, double correction)
{
    double roughZEQ = (viscAir / starU) * reynolds;
    return correction * (karman / log(measureZ / roughZEQ)); //log or log10
}

double sqrtComponents(double sqrtNeutral, double psi, double correction)
{
    return sqrtNeutral / (1 - (sqrtNeutral / (karman * correction)) * psi);
}

void getPsi(double z)
{
    double y = cbrt(1. - profileGamma * z);
    double psiC = 1.5 * log((y * y + y + 1) / 3) - sqrt(3) * atan((2 * y + 1) / sqrt(3)) + M_PI / sqrt(3);
    double psiKU = 2 * log((1 + sqrt(y)) / 2) + log((y + 1)/2);
    double psiKH = 2 * log((1 + y) / 2);

    /*

    double au = 16.;
    double yLKB = sqrt(1 - au * z);
    double xLKB = sqrt(yLKB);
    
    double psiKHLKB = 2 * log((1 + yLKB)/2);
    double psiKULKB = 2 * log((1 + xLKB)/2) + log((xLKB * xLKB + 1) / 2) - 2 * atan(xLKB) + M_PI / 2;

    */

    psiU = (1 / (1 + (z * z))) * psiKU + ((z * z) / (1 + (z * z))) * psiC;
    psiH = (1 / (1 + (z * z))) * psiKH + ((z * z) / (1 + (z * z))) * psiC;
}


struct coare run() // here's where we put the current main() function from coare.c
{


    FILE *conv = fopen("converge.txt", "a");

    viscAir = kinVisc(airT);
    enthalpyL = enthalpyV(airT);
    windS = sqrt((windU * windU) + (wg * wg));

    potT = airT + 0.0098 * measureZ;
    potDiffT = fabs(airT - (surfaceT + 0.0098 * measureZ));
    satSpecificQ = satMix(airT);

    starU = 0.04 * windS;
    starT = -1 * 0.04 * potDiffT;
    starQ = -1 * fabs(specificQ - satSpecificQ);

    i = 0;
    prevL = -200000;



    while(fabs(prevL - fluxL) > .0000001 && i < 20)
    {
        prevL = fluxL;
        i++;

        calcZeta();
        calcRoughZ();
        reynoldsR = starU * roughZ / viscAir;
        reynoldsConvert();
        getPsi(zeta);
        sqrtCoefDN = sqrtNeutrals(reynoldsR, 1.);
        sqrtCoefTN = sqrtNeutrals(reynoldsT, aCorr);
        sqrtCoefQN = sqrtNeutrals(reynoldsQ, aCorr);
        sqrtCoefD = sqrtComponents(sqrtCoefDN, psiU, 1);
        sqrtCoefT = sqrtComponents(sqrtCoefTN, psiH, aCorr);
        sqrtCoefQ = sqrtComponents(sqrtCoefQN, psiH, aCorr);
        starUt = sqrt(sqrtCoefD * sqrtCoefD * windS * windS);
        starT = -1 * sqrtCoefT * (surfaceT - potT);
        starQ = -1 * sqrtCoefQ * (satSpecificQ - specificQ);
        fluxS = -1 * density * 1004.67 * starU * starT;
        stability = 1 / ((1 - sqrtCoefDN * psiU / karman) * (1 - sqrtCoefQN * psiH / (aCorr * karman)));
        gustiness = sqrt(1 + pow(wg / windU, 2));
        fluxL = density * enthalpyL * coefEN * windU * (satSpecificQ - specificQ) * gustiness * stability;
        printf("\n\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf", fluxL, density, enthalpyL, coefEN, windU, satSpecificQ, specificQ, gustiness, stability);
        printf("\n\n%lf", specificQ / satSpecificQ);
        printf("\n\n%lf and %lf", specificQ, satSpecificQ);
        tau = starU * starU * density;
        starU = starUt;

        //fprintf(conv, "%lf,%lf,%lf,%lf,%lf;", zeta, starU, starT, starQ, fluxL);
        fprintf(conv, "%lf,", reynoldsR);
    }

    fprintf(conv, "\n");
    fclose(conv);

    struct coare final;
    final.loops = i;
    final.sensible = fluxS;
    final.latent = fluxL;
    final.tau = tau;
    return final;
}