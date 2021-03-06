#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "def.h"

double fluxLFinal;
double humidityFinal;

double alpha, profileGamma, aCorr, grav, karman, measureZ, windU, airT, surfaceT, specificQ, density, wg, coefEN, coefHN, roughZ, starQ, starT, starU, longwave;
double lastHL, lastHS, lastStarU;

/*double sstCorrection()
{
    double salinitySB = 0.026;
    double expansionAlpha = 0.0003;
    double cpWater = 4184.;
    double densityWater = 1023.;
    double viscWater = 0.000000937;
    double conductivityK = 0.615;
    
    double cooling = longwave - lastHL - lastHS; 
    double virtualCooling = lastHL * (salinitySB * cpWater / (expansionAlpha * enthalpyV(airT))) + cooling;

    double saundersLambda = 16. * virtualCooling * grav * expansionAlpha * densityWater * cpWater * pow(viscWater, 3);
    saundersLambda = saundersLambda / (pow(lastStarU, 4) * (density / densityWater) * (density / densityWater) * conductivityK * conductivityK);
    saundersLambda = 6. / pow((1 + pow(saundersLambda, (3./4.))), (1./3.));

    double subThickness = saundersLambda * viscWater / (sqrt(density / densityWater) * lastStarU);

    double coolCorrection = cooling * subThickness / conductivityK;

}*/

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

double calcZeta(double measureZ, double karman, double grav, double t, double starT, double starQ, double starU)
{
    //zeta = measureZ * ((karman * grav / (airT+273.)) * (starT + 0.61 * (airT+273.) * starQ) / (starU * starU));
    return measureZ * ((karman * grav / (t)) * (starT + 0.61 * (t) * starQ) / (starU * starU)); //C or K?
}

double calcRoughZ(double alpha, double starU, double grav)
{
    return alpha * ((starU * starU) / grav); //+ 0.11 * (viscAir / starU);
}

// Uses reynolds number to calculate equivalents for temp (0) and humidity (1)
double reynoldsConvert(double r, int type) 
{
    if (!(type == 0 || type == 1)) return -1;
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
        if (r > range[i] && r <= range[i + 1])
        {
            return a[type][i] * pow(r, b[type][i]);
        }
    }
}

double sqrtNeutrals(double reynolds, double correction, double viscAir, double starU, double karman, double z)
{
    double roughZEQ = (viscAir / starU) * reynolds;
    return correction * (karman / log(z / roughZEQ)); //log or log10
}

double sqrtComponents(double sqrtNeutral, double psi, double correction, double karman)
{
    return sqrtNeutral / (1 - (sqrtNeutral / (karman * correction)) * psi);
}

// get psi function for wind U (0) or temp / humidity H (1)
double getPsi(double z, double profileGamma, int type) 
{
    if (!(type == 0 || type == 1)) return -1;
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

    if (type == 0) return (1 / (1 + (z * z))) * psiKU + ((z * z) / (1 + (z * z))) * psiC; // U
    else return (1 / (1 + (z * z))) * psiKH + ((z * z) / (1 + (z * z))) * psiC; // H
}

/*
Run COARE algorithm, returning latent heat flux and writing results to [coareFilename].txt
*/
void runCoare(CoareData inputs, double *humidityFinal, double *fluxLFinal)
{
    FILE *coare = fopen(inputs.coareFilename, "a");
    FILE *conv = fopen(inputs.convFilename, "a");

    alpha = inputs.alpha;
    profileGamma = inputs.gamma;
    aCorr = inputs.a;
    grav = inputs.g;
    karman = inputs.karman;
    measureZ = inputs.measureZ;
    windU = inputs.u;
    surfaceT = inputs.surfaceT;
    airT = inputs.airT;
    specificQ = inputs.q;
    density = inputs.rho;
    wg = inputs.wgGuess;
    coefEN = inputs.cEN;
    coefHN = inputs.cHN;
    roughZ = inputs.z0;
    starQ = inputs.starQ;
    starT = inputs.starT;
    starU = inputs.starU;
    longwave = inputs.longwave;
    lastHL = inputs.lastHL;
    lastHS = inputs.lastHS;
    lastStarU = inputs.lastStarU;

    double zeta = 0;
    double rR, rQ, rT;
    double fluxS, stability, gustiness, tau;
    double sqrtCoefDN, sqrtCoefTN, sqrtCoefQN, sqrtCoefD, sqrtCoefT, sqrtCoefQ, psiU, psiH;
    double starUt = 0;

    double viscAir = kinVisc(airT);
    double enthalpyL = enthalpyV(airT);
    double windS = sqrt((windU * windU) + (wg * wg));

    double potT = airT + 0.0098 * measureZ;
    double satSpecificQ = satMix(airT);

    int i = 0;
    double prevL = -1;
    double fluxL = 0;


    while(fabs(prevL - fluxL) > .0000001 && i < 20)
    {
        prevL = fluxL;
        i++;

        zeta = calcZeta(measureZ, karman, grav, airT, starT, starQ, starU);
        roughZ = calcRoughZ(alpha, starU, grav);

        rR = starU * roughZ / viscAir;
        rT = reynoldsConvert(rR, 0);
        rQ = reynoldsConvert(rR, 1);

        psiU = getPsi(zeta, profileGamma, 0);
        psiH = getPsi(zeta, profileGamma, 1);

        sqrtCoefDN = sqrtNeutrals(rR, 1., viscAir, starU, karman, measureZ);
        sqrtCoefTN = sqrtNeutrals(rT, aCorr, viscAir, starU, karman, measureZ);
        sqrtCoefQN = sqrtNeutrals(rQ, aCorr, viscAir, starU, karman, measureZ);

        sqrtCoefD = sqrtComponents(sqrtCoefDN, psiU, 1., karman);
        sqrtCoefT = sqrtComponents(sqrtCoefTN, psiH, aCorr, karman);
        sqrtCoefQ = sqrtComponents(sqrtCoefQN, psiH, aCorr, karman);

        starUt = sqrt(sqrtCoefD * sqrtCoefD * windS * windS);
        starT = -1 * sqrtCoefT * (surfaceT - potT);
        starQ = -1 * sqrtCoefQ * (satSpecificQ - specificQ);

        fluxS = -1 * density * 1004.67 * starU * starT;

        stability = 1 / ((1 - sqrtCoefDN * psiU / karman) * (1 - sqrtCoefQN * psiH / (aCorr * karman)));
        gustiness = sqrt(1 + pow(wg / windU, 2));
        fluxL = density * enthalpyL * coefEN * windU * (satSpecificQ - specificQ) * gustiness * stability;

        tau = starU * starU * density;
        starU = starUt;
  
        fprintf(conv, "%lf,", fluxL);

    }

    fprintf(coare, "%d,%lf,%lf,%lf,%lf,%lf,%lf\n", i, specificQ, fluxL, fluxS, tau, stability, gustiness);
    fclose(coare);
    fprintf(conv, "\n");
    fclose(conv);

    *fluxLFinal = fluxL;
    *humidityFinal = specificQ;
    
}