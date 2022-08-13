#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "spkmeans.h"
#include "utils.c"

int main(int argc, char *argv[])
{
    int k;
    char *goal;
    char *fileName;
    if (argc == 4)
    {
        k = atoi(argv[1]);
        goal = argv[2];
        fileName = argv[3];
        execByGoal(k, goal, fileName);
    }
    return 0;
}

void execByGoal(int k, char *goal, char *filename)
{
    
    /* executes by given goal,and stops at goal */

    /* base variables */
    double **pointArrPtr;
    char *inFileNamePtr;
    int numOfPointsArg, numOfCordsArg;

    double **weightedAdjMat;

    assignPoints(&pointArrPtr, &inFileNamePtr, &numOfPointsArg, &numOfCordsArg);

    if (!strcmp(goal, "wam")) /* if goal is adjacency */
    {
    }
    else
    {
    }
}
