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

void timeLoop(CoareData in) 
{
    double fluxL = 0;
    double humidity = 0;
    double prevHumid = 0;
    double timePrint = 0;
    double prevT = 0;
    int printed = 0;

    CoareData coareIn = in;

    char filecntString[80];
    sprintf(filecntString, "%d", in.filecnt);

    char filename[80];
    strcpy(filename, in.dest);
    strcat(filename, "/");

    strcat(filename, in.modVariable);
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

    double windU = in.u;
    double wg = in.wgGuess;
    double windS = sqrt((windU * windU) + (wg * wg));
    double potDiffT = fabs(in.airT - (in.surfaceT + 0.0098 * in.measureZ));   
    double starU = 0.04 * windS;
    double starT = -1 * 0.04 * potDiffT;
    double satSpecificQ = satMix(in.airT);
    double starQ = -1 * fabs(in.q - satSpecificQ);

    coareIn.starU = starU;
    coareIn.starT = starT;
    coareIn.starQ = starQ;
    
    for(int time = 0; time < (int) (in.period / in.dt); time++)
    {
        runCoare(coareIn, &humidity, &fluxL);  
        double relative = humidity / satSpecificQ;
        
        if(relative > in.humidCutoff && !printed)
        {
            double prevRelative = prevHumid / satSpecificQ;
            timePrint = prevT + (time * in.dt - prevT) * (in.humidCutoff - prevRelative) / (relative - prevRelative);
            printed = 1;
        }      
        //fprintf(output, "%lf, %lf\n", time * in.dt, fluxL);      
        
        coareIn.q += deltaSpecificQ(fluxL, in.dqCoef, in.rho, in.dt, in.airT);

        prevHumid = humidity;
        prevT = time * in.dt;
        //"%20.12lf\n"
        //fprintf(output, "\n%lf\n", (satSpecificQ - specificQ) / satSpecificQ);
    }

    fprintf(output, "%lf,%lf,%lf\n", in.dqCoef, windU, timePrint / 3600);
    printf("\nRan for %d loops over %.0lf seconds with dt %.2lf\n\n", ((int) (in.period / in.dt)), in.period, in.dt);
    fclose(output);
}





