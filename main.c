#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    CoareData coareIn;
    coareIn.alpha = looperInputs.alpha;
    coareIn.gamma = looperInputs.gamma;
    coareIn.a = looperInputs.a;
    coareIn.g = looperInputs.g;
    coareIn.karman = looperInputs.karman;
    coareIn.measureZ = looperInputs.measureZ;
    coareIn.u = looperInputs.u;
    coareIn.surfaceT = looperInputs.surfaceT;
    coareIn.airT = looperInputs.airT;
    coareIn.q = looperInputs.q;
    coareIn.rho = looperInputs.rho;
    coareIn.wgGuess = looperInputs.wgGuess;
    coareIn.cEN = looperInputs.cEN;
    coareIn.cHN = looperInputs.cHN;
    coareIn.z0 = looperInputs.z0;

    char filecntString[80];
    sprintf(filecntString, "%d", looperInputs.filecnt);

    char filename[80];
    strcpy(filename, looperInputs.dest);
    strcat(filename, "/");
    
    char coareFilename[80];
    strcpy(coareFilename, filename);

    strcat(filename, looperInputs.modVariable);
    strcat(filename, ".txt");

    strcat(coareFilename, looperInputs.modVariable);
    strcat(coareFilename, "/");

    char convFilename[80];
    strcpy(convFilename, coareFilename);
    strcat(convFilename, "converge/");

    strcat(coareFilename, filecntString);
    strcat(coareFilename, ".txt");
    
    strcat(convFilename, filecntString);
    strcat(convFilename, "converge.txt");




    strcpy(coareIn.coareFilename, coareFilename);
    strcpy(coareIn.convFilename, convFilename);
    FILE *output = fopen(filename, "a");


    
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





