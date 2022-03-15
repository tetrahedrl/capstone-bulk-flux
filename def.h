#ifndef DEF_H
#define DEF_H

typedef struct
{

    double alpha;
    double gamma;
    double a;
    double g;
    double karman;
    double measureZ;
    double u;
    double surfaceT;
    double airT;
    double q;
    double rho;
    double wgGuess;
    double cEN;
    double cHN;
    double z0;

    double starT;
    double starQ;
    double starU;

    char dest[80];
    char modVariable[80];
    int filecnt;
    
    double dt;
    double period;
    double dqCoef;

    char coareFilename[80];
    char convFilename[80];

} CoareData;

#endif