#include "def.h"
#ifndef COARE_H
#define COARE_H


// Run COARE algorithm, returning latent heat flux and writing results to [coareFilename].txt
void runCoare(CoareData inputs, double *humidityFinal, double *fluxLFinal);

// return enthalpy of vaporization for temp t
double enthalpyV(double t);

// return saturation mixing ratio for temp t
double satMix(double t);

// return kinematic viscosity of air for temp t
double kinVisc(double t);

double scalingParam(double coefTransfer, double total, double subtract);

double calcZeta(double measureZ, double karman, double grav, double t, double starT, double starQ, double starU);

double calcRoughZ(double alpha, double starU, double grav);

// Uses reynolds number to calculate equivalents for temp (0) and humidity (1)
double reynoldsConvert(double r, int type); 

double sqrtNeutrals(double reynolds, double correction, double viscAir, double starU, double karman, double z);

double sqrtComponents(double sqrtNeutral, double psi, double correction, double karman);

// get psi function for wind U (0) or temp / humidity H (1)
double getPsi(double z, double gamma, int type);

#endif