#include "inputs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern int time;
extern double alpha, beta, profileGamma, aCorr, grav, invZ, albedo, emissiv, karman; 
extern double measureZ, windU, surfaceT, airT, specificQ, precip, longwave, solar, pressure;
extern double surfaceTPrevious, surfaceQPrevious;
extern double wg, coefEN, coefHN, roughZ;

double sqrtCoefDN, sqrtCoefTN, sqrtCoefQN, sqrtCoefD, sqrtCoefT, sqrtCoefQ;
double viscAir, enthalpyL, windS, surfaceQ, potT, potDiffT, interfSpecificQ, zeta, profileY;
double starT, starQ, starU, starUt;
double reynoldsR, reynoldsT, reynoldsQ;
double psiU, psiH;
double fluxS, fluxL, fluxT;
double prevS = 1;

double stability;
double gustiness;
double tau;

int i;

double humidity(double t, double p)
{
    p = 1000;
    double qs = 0.98 * ((1.0007 + 0.00000356 * p) * 6.1121 * exp(17.502 * t / (240.97 + t)));
    return 0.62197 * (qs / (p - 0.378 * qs));
}

double scalingParam(double coefTransfer, double interfaceValue, double tempDependant)
{
    return -1 * sqrt(coefTransfer) * (interfaceValue - tempDependant);
}

void calcZeta()
{
    zeta = measureZ * ((karman * grav / (airT+273.)) * (starT + 0.61 * (airT+273.) * starQ) / (starU * starU));
}

void calcRoughZ()
{
    roughZ = alpha * ((starU * starU) / grav) + 0.11 * (viscAir / starU);
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
    return correction * (karman / log10(measureZ / roughZEQ));
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
    psiU = (1 / (1 + (z * z))) * psiKU + ((z * z) / (1 + (z * z))) * psiC;
    psiH = (1 / (1 + (z * z))) * psiKH + ((z * z) / (1 + (z * z))) * psiC;
}

int main()
{

    takeInputs();
    //printf("Air T\n%lf", airT);

    specificQ = specificQ / 1000; // 

    viscAir = 0.00001326 * (1 + 0.006542 * airT + 0.000008301 * (airT *airT) - 0.00000000484 * pow(airT, 3));
    //printf("\nAir Viscosity\n%lf", viscAir);

    enthalpyL = 100000 * (25 - 0.02274 * airT);
    //printf("\nLatent Heat of Vaporization\n%lf", enthalpyL);

    windS = sqrt((windU * windU) + (wg * wg));
    //printf("\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf", wg, coefEN, coefHN, coefDN, coefTN, coefQN);

    potT = airT + 0.0098 * measureZ;
    potDiffT = fabs(airT - (surfaceT + 0.0098 * measureZ));
    interfSpecificQ = humidity(airT, pressure);

    //printf("\nspecificQ\n%lf\ninterfSpecificQ\n%lf", specificQ, interfSpecificQ);
    //printf("\nairT\n%lf\npotT\n%lf\npotDiffT\n%lf", airT, potT, potDiffT);

    starU = 0.04 * windS;
    starT = -1 * 0.04 * potDiffT;
    starQ = -1 * fabs(specificQ - interfSpecificQ);

    //for(int i = 0; i < 20; i++)
    while(abs(prevS - fluxS) > .001 && i < 20)
    {
        prevS = fluxS;
        i++;
        //printf("\n\nairT\n%lf\nstarT\n%lf\nstarQ\n%lf\nstarU\n%lf\n", airT, starT, starQ, starU);
        calcZeta();
        //printf("\n\nzeta\n%lf\n", zeta);
        calcRoughZ();
        //printf("\n\nroughZ\n%lf\n", roughZ);
        reynoldsR = starU * roughZ / viscAir;
        reynoldsConvert();
        //printf("\noriginal DN\n%lf", karman / log(measureZ / roughZ));
        getPsi(zeta);
        sqrtCoefDN = sqrtNeutrals(reynoldsR, 1);
        sqrtCoefTN = sqrtNeutrals(reynoldsT, aCorr);
        sqrtCoefQN = sqrtNeutrals(reynoldsQ, aCorr);
        //printf("\nDN\n%lf", sqrtCoefDN);
        //printf("\nTN\n%lf", sqrtCoefTN);
        //printf("\nQN\n%lf", sqrtCoefQN);
        sqrtCoefD = sqrtComponents(sqrtCoefDN, psiU, 1);
        sqrtCoefT = sqrtComponents(sqrtCoefTN, psiH, aCorr);
        sqrtCoefQ = sqrtComponents(sqrtCoefQN, psiH, aCorr);
        starUt = sqrt(sqrtCoefD * sqrtCoefD * windS * windS);
        starT = -1 * sqrtCoefT * (surfaceT - potT);
        starQ = -1 * sqrtCoefQ * (interfSpecificQ - specificQ);
        fluxS = -1 * pressure * 1004.67 * starU * starT;
        fluxL = -1 * pressure * enthalpyL * starU * starQ;
        printf("\n\nLatent Heat Flux Old %lf", fluxL);
        stability = 1 / ((1 - sqrtCoefDN * psiU / karman) * (1 - sqrtCoefQN * psiH / (aCorr * karman)));
        gustiness = sqrt(1 + pow(wg / windU, 2));
        fluxL = pressure * enthalpyL * coefEN * windU * (interfSpecificQ - specificQ) * gustiness * stability;
        fluxT = -1 * pressure * starU * starU;
        tau = starU * starU * pressure;

        printf("\n\nLoop counter %d", i);
        printf("\n\nSensible Heat Flux %lf", fluxS);
        printf("\n\nLatent Heat Flux %lf", fluxL);

        starU = starUt;
        //printf("\npsiC(zeta)\n%lf", psiC(zeta));
    }

    //calcZeta();

    //printf("\nZeta\n%lf", zeta);

    //starT = scalingParam(coefTN, surfaceT, potT);
    
 /*   reynoldsR = 30;
    reynoldsConvert();
    printf("\nRR\n%lf", reynoldsR);
    printf("\nRT\n%lf", reynoldsT);
    printf("\nRQ\n%lf", reynoldsQ);
*/
}
