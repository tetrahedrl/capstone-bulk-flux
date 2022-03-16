#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "math.h"
#include "coare.h"
#include "inputs.h"
#include "def.h"

double deltaSpecificQ(double latent, double deltaQCoef, double density, double dt, double airT)
{
    double dq = deltaQCoef * latent / (enthalpyV(airT) * density);
    return dq * dt;
}

void timeLoop(CoareData looperInputs) 
{
    double fluxL = 0;

    CoareData coareIn = looperInputs;

    char filecntString[80];
    sprintf(filecntString, "%d", looperInputs.filecnt);

    char filename[80];
    strcpy(filename, looperInputs.dest);
    strcat(filename, "/");

    strcat(filename, looperInputs.modVariable);
    mkdir(filename);
    char coareFilename[80];
    strcpy(coareFilename, filename);
    strcat(filename, ".txt");

    strcat(coareFilename, "/");

    char convFilename[80];
    strcpy(convFilename, coareFilename);
    strcat(convFilename, "converge");
    mkdir(convFilename);
    strcat(convFilename, "/");
    strcat(coareFilename, filecntString);
    strcat(coareFilename, ".txt");
    
    strcat(convFilename, filecntString);
    strcat(convFilename, "converge.txt");




    strcpy(coareIn.coareFilename, coareFilename);
    strcpy(coareIn.convFilename, convFilename);
    FILE *output = fopen(filename, "a");

    double windU = looperInputs.u;
    double wg = looperInputs.wgGuess;
    double windS = sqrt((windU * windU) + (wg * wg));
    double potDiffT = fabs(looperInputs.airT - (looperInputs.surfaceT + 0.0098 * looperInputs.measureZ));   
    double starU = 0.04 * windS;
    double starT = -1 * 0.04 * potDiffT;
    double satSpecificQ = satMix(looperInputs.airT);
    double starQ = -1 * fabs(looperInputs.q - satSpecificQ);

    coareIn.starU = starU;
    coareIn.starT = starT;
    coareIn.starQ = starQ;
    
    for(int time = 0; time < (int) (looperInputs.period / looperInputs.dt); time++)
    {
        fluxL = runCoare(coareIn);        
        fprintf(output, "%lf,", fluxL);      
        coareIn.q += deltaSpecificQ(fluxL, looperInputs.dqCoef, looperInputs.rho, looperInputs.dt, looperInputs.airT);

        //"%20.12lf\n"
        //fprintf(output, "\n%lf\n", (satSpecificQ - specificQ) / satSpecificQ);
    }

    fprintf(output, "\n");
    printf("\nRan for %d loops over %.0lf seconds with dt %.2lf\n\n", ((int) (looperInputs.period / looperInputs.dt)), looperInputs.period, looperInputs.dt);
    fclose(output);
}





