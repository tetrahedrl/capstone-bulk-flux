#include <stdio.h>
#include <stdlib.h>
#include "def.h"
#include "coare.h"
#include "inputs.h"

double deltaSpecificQ;
double initSpecificQ;

extern double refresh;
extern int steps;
extern double dt;
extern double deltaQCoef;

extern double airT;
extern double windV;
extern double density;
extern double specificQ;
extern double satSpecificQ;
extern double reynoldsR;

void updateSpecificQ(double latent)
{
    deltaSpecificQ = latent / (enthalpyV(airT) * windV * density);
    specificQ += deltaSpecificQ * dt * deltaQCoef;
    //specificQ = specificQ * refresh + initSpecificQ * (1 - refresh);
}

int main() // run function returns a struct, stored into struct results and then printed
{
    
    takeInputs();
    initSpecificQ = specificQ;
    struct coare results = run();
    printf("\n\n Latent Flux %lf, Loops %d", results.latent, results.loops);

    //deltaSpecificQ = results.latent / (enthalpyV(airT) * windV * density);        
    printf("\n\n Delta specificQ %lf", deltaSpecificQ);

    updateSpecificQ(results.latent);

    for(int i = 0; i < steps; i++)
    {
        results = run();
        //deltaSpecificQ = results.latent / (enthalpyV(airT) * windV * density);        
        updateSpecificQ(results.latent);
        printf("\n%lf", specificQ);
    }

    printf("\n\n Latent Flux %lf, Loops %d", results.latent, results.loops);
    printf("\n\n Delta specificQ %lf", deltaSpecificQ);
    printf("\n\n");

    
}
