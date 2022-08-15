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
    double **diagonalDegreeMat;

    assignPoints(&pointArrPtr, &filename, &numOfPointsArg, &numOfCordsArg);
    createWeightedAdjMat(&weightedAdjMat, &pointArrPtr, &numOfPointsArg, &numOfCordsArg);
    createDiagonalDegreeMat(&diagonalDegreeMat, &weightedAdjMat, numOfCordsArg);
    if (!strcmp(goal, "wam")) /* if goal is adjacency */
    {
    }
    else
    {
    }
}

void createWeightedAdjMat(double ***weightedAdjMat, double ***pointArrPtr, int *numOfPoints, int *numOfCords)
{
    int nodeVal;
    *weightedAdjMat = calloc(sizeof(double *), *numOfPoints);
    customAssert(*weightedAdjMat != NULL);
    for (int i = 0; i < *numOfPoints; i++)
    {
        (*weightedAdjMat)[i] = calloc(sizeof(double), *numOfPoints);
        customAssert((*weightedAdjMat)[i] != NULL);
    }

    for (int rowIdx = 0; rowIdx < *numOfPoints; rowIdx++)
    {
        for (int colIdx = rowIdx; colIdx < *numOfPoints; colIdx++)
        {
            if (rowIdx == colIdx)
                continue;
            nodeVal = exp(-getEuclideanNorm((*pointArrPtr)[rowIdx], (*pointArrPtr)[colIdx], *numOfCords) / 2);
            printf("%f",nodeVal);
            (*weightedAdjMat)[rowIdx][colIdx] = nodeVal;
            (*weightedAdjMat)[colIdx][rowIdx] =   nodeVal;
        }
    }
    printf("%f",(*weightedAdjMat)[1][2]);

    /* code */
}

void createDiagonalDegreeMat(double *** ddg, double *** mat, int n)
{
    int i;
    /*create zero mat*/
    *ddg = (double **) calloc(n, sizeof(double *));
    customAssert(*ddg != NULL);
    for (i = 0; i < n; i++){
        (*ddg)[i] = (double *) calloc(n, sizeof(double));
        customAssert((*ddg)[i] != NULL);
        (*ddg)[i][i] = SumMatRow((*mat)[i], n);
    }
}
