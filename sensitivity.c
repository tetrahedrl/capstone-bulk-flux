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
    strcpy(test.dest, "test1");
    strcpy(test.modVariable, "u");
    test.u = 4;
    for(int i = 0; i < 10; i++)
    {
        test.filecnt = i;
        test.u = test.u + 0.1;
        timeLoop(test);
    }
}