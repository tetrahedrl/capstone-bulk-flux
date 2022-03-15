#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coare.h"
#include "inputs.h"
#include "def.h"
#include "main.h"

int main()
{
    CoareData test = takeInputs();
    strcpy(test.dest, "test2");
    strcpy(test.modVariable, "dqCoef");
    for(int i = 0; i < 10; i++)
    {
        test.filecnt = i;
        test.dqCoef += 0.01;
        timeLoop(test);
    }
}