#include <stdio.h>
#include <stdlib.h>
#include "def.h"
#include "coare.h"
#include "inputs.h"

double deltaSpecificQ;

extern double airT;
extern double windV;
extern double density;
extern double specificQ;
extern double satSpecificQ;

int main() // run function returns a struct, stored into struct results and then printed
{
    
    takeInputs();
    struct coare results = run();
    printf("\n\n Latent Flux %lf, Loops %d", results.latent, results.loops);

    deltaSpecificQ = results.latent / (enthalpyV(airT) * windV * density);        
    printf("\n\n Delta specificQ %lf", deltaSpecificQ);

    specificQ += deltaSpecificQ;

    for(int i = 0; i < 100; i++)
    {
        results = run();
        deltaSpecificQ = results.latent / (enthalpyV(airT) * windV * density);        
        specificQ += deltaSpecificQ;
        printf("\n%lf", specificQ);
    }

    printf("\n\n Latent Flux %lf, Loops %d", results.latent, results.loops);
    printf("\n\n Delta specificQ %lf", deltaSpecificQ);

    
}
