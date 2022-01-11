#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double humidity(double t, double p)
{
    double qs = 0.98 * ((1.0007 + 0.00000356 * p) * 6.1121 * exp(17.502 * t / (240.97 + t)));
    return 0.62197 * (qs / (p - 0.378 * qs));
}

void main()
{
    double test = humidity(27.7, 1000);
    printf("%lf", test);
    
    
    
    
    /*double profileGamma = 12.87;
    double z = 11660.430461003674;
    double test = 1 - profileGamma * z;
    double y = pow(1 - profileGamma * z, 1 / 3);
    double psiC = 1.5 * log((y * y + y + 1) / 3) - sqrt(3) * atan((2 * y + 1) / sqrt(3)) + M_PI / sqrt(3);
    double psiKU = 2 * log((1 + sqrt(y)) / 2) + log((y + 1)/2);
    double psiKH = 2 * log((1 + y) / 2);
    double psiU = (1 / (1 + pow(z, 2))) * psiKU + (pow(z, 2) / (1 + pow(z, 2))) * psiC;
    double psiH = (1 / (1 + pow(z, 2))) * psiKH + (pow(z, 2) / (1 + pow(z, 2))) * psiC;*/
}