#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "coare.h"
#include "inputs.h"
#include "def.h"
#include "main.h"

int main()
{
    struct stat s = {0};
    char destination[80] = "out/test";
    char destCheck[80];
    char iStr[80];

    mkdir("out");

    int i = 0;

    while(i < 100)
    {
        sprintf(iStr, "%d", i);
        strcpy(destCheck, destination);
        strcat(destCheck, iStr);

        if (stat(destCheck, &s) == -1) {
            strcpy(destination, destCheck);
            mkdir(destination);
            break;
        }

        i++;
    }

    CoareData test = takeInputs();
    strcpy(test.dest, destination);
    strcpy(test.modVariable, "dqCoef");
    double initialDQ = test.dqCoef;
    test.u = 0;
    for(int w = 0; w < 7; w++)
    {
        test.u += 2;
        test.dqCoef = initialDQ;
        for(int i = 0; i < 100; i++)
        {
            test.filecnt = (w * 1000) + i;
            test.dqCoef += 0.0001;
            timeLoop(test);
        }
        
    }
}