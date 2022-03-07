#include <stdio.h>
#include <stdlib.h>
#include "def.h"
#include "coare.h"
#include "inputs.h"

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

void updateSpecificQ(double latent)
{
    deltaSpecificQ = latent / (enthalpyV(airT) * volZ * density);
    specificQ += deltaSpecificQ * dt * deltaQCoef;
    //specificQ = specificQ * refresh + initSpecificQ * (1 - refresh);
}

int main() // run function returns a struct, stored into struct results and then printed
{
    FILE *output = fopen("out.txt", "w");
    takeInputs();
    initSpecificQ = specificQ;
    struct coare results = run();
    printf("\n\n Latent Flux %lf, Loops %d", results.latent, results.loops);

    //deltaSpecificQ = results.latent / (enthalpyV(airT) * volZ * density);        
    printf("\n\n Delta specificQ %lf", deltaSpecificQ);

    updateSpecificQ(results.latent);

    for(int time = 0; time < (int) period / dt; time++)
    {
        results = run();
        //deltaSpecificQ = results.latent / (enthalpyV(airT) * volZ * density);        
        updateSpecificQ(results.latent);
        fprintf(output, "%lf\n", results.sensible);
        
        //fprintf(output, "\n%lf\n", (satSpecificQ - specificQ) / satSpecificQ);
    }

    printf("\n\n Latent Flux %lf, Loops %d", results.latent, results.loops);
    printf("\n\n Delta specificQ %lf", deltaSpecificQ);
    printf("\n\n");

    fclose(output);
}
