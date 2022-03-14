#include <stdio.h>
#include <stdlib.h>
#include "coare.h"
#include "inputs.h"
#include "def.h"

double deltaSpecificQ;
double initSpecificQ;

extern double refresh;
extern double period;
extern double dt;
extern double deltaQCoef;

extern double airT;
extern double volZ;
extern double density;
extern double specificQ;
extern double satSpecificQ;
extern double reynoldsR;
extern double i;

void updateSpecificQ(double latent)
{
    deltaSpecificQ = deltaQCoef * latent / (enthalpyV(airT) * volZ * density);
    specificQ += deltaSpecificQ * dt;
    //specificQ = specificQ * refresh + initSpecificQ * (1 - refresh);
}

void timeLoop() // run function returns a struct, stored into struct results and then printed
{
    FILE *output = fopen("out.txt", "w");

    // clear converge.txt
    FILE *conv = fopen("converge.txt", "w");
    fprintf(conv, "");
    fclose(conv);
    
    initSpecificQ = specificQ;
    struct coare results = run();
    
    for(int time = 0; time < (int) (period / dt); time++)
    {
        results = run();
        fprintf(output, "%d,%d,%3.8lf,%3.8lf,%3.8lf\n", (int) (time * dt), i, results.latent, deltaSpecificQ, specificQ);
        
        //deltaSpecificQ = results.latent / (enthalpyV(airT) * volZ * density);        
        updateSpecificQ(results.latent);
        
        //"%20.12lf\n"
        //fprintf(output, "\n%lf\n", (satSpecificQ - specificQ) / satSpecificQ);
    }

    printf("\nRan for %d loops over %.0lf seconds with dt %.2lf\n\n", ((int) (period / dt)), period, dt);

    fclose(output);
}





