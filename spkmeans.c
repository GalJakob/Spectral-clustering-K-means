#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "spkmeans.h"
#include "utils.c"
#define EPSILON 0.00001
/* EPSILON 0.00001 */
#define MAX_ROTATIONS 100

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
    int numOfPointsArg, numOfCordsArg;
    double **weightedAdjMat;
    double **diagonalDegreeMat;
    double **LnormMat;
    double **eigenVectorsMat;
    assignPoints(&pointArrPtr, &filename, &numOfPointsArg, &numOfCordsArg);

    if (!strcmp(goal, "jacobi"))
    {
        printf("in expec %f\n", pointArrPtr[0][0]);
        performJacobiAlg(pointArrPtr, numOfPointsArg);
        exit(1);
    }

    createWeightedAdjMat(&weightedAdjMat, &pointArrPtr, &numOfPointsArg, &numOfCordsArg);
    createDiagonalDegreeMat(&diagonalDegreeMat, &weightedAdjMat, numOfPointsArg);
    createTheNormalizedGraphLaplacian(&LnormMat, &weightedAdjMat, &diagonalDegreeMat, numOfPointsArg);
    performJacobiAlg(LnormMat, numOfPointsArg);
    k = 3;
    if (k == 3)
    {
        /* code */
    }

    if (!strcmp(goal, "wam")) /* if goal is adjacency */
    {
    }
    else
    {
    }
}

void createWeightedAdjMat(double ***weightedAdjMat, double ***pointArrPtr, int *numOfPoints, int *numOfCords)
{
    int i, rowIdx, colIdx;
    double nodeVal;
    *weightedAdjMat = calloc(*numOfPoints, sizeof(double *));
    customAssert(*weightedAdjMat != NULL);
    for (i = 0; i < *numOfPoints; i++)
    {
        (*weightedAdjMat)[i] = calloc(*numOfPoints, sizeof(double));
        customAssert((*weightedAdjMat)[i] != NULL);
    }

    for (rowIdx = 0; rowIdx < *numOfPoints; rowIdx++)
    {
        for (colIdx = rowIdx; colIdx < *numOfPoints; colIdx++)
        {
            if (rowIdx == colIdx)
                continue;
            nodeVal = exp(-getEuclideanNorm((*pointArrPtr)[rowIdx], (*pointArrPtr)[colIdx], *numOfCords) / 2);

            (*weightedAdjMat)[rowIdx][colIdx] = nodeVal;
            (*weightedAdjMat)[colIdx][rowIdx] = nodeVal;
        }
    }
}

void createDiagonalDegreeMat(double ***ddg, double ***weightedAdjMat, int n)
{

    /**/
    int i;
    *ddg = (double **)calloc(n, sizeof(double *));
    customAssert(*ddg != NULL);
    for (i = 0; i < n; i++)
    {
        (*ddg)[i] = (double *)calloc(n, sizeof(double));
        customAssert((*ddg)[i] != NULL);
        (*ddg)[i][i] = SumMatRow((*weightedAdjMat)[i], n);
    }
}

void performJacobiAlg(double **LnormMat, int numOfPoints)
{

    /* performs the Jacobi algorithm and gets the eigenvales and eigenvectors of Lnrom */

    int rotIdx, k;
    double **P, **PTranspose, **A, **ATag, **productOfPs;
    EIGEN *sortedEIGENS;
    productOfPs = NULL;
    rotIdx = 1;
    A = LnormMat;

    while (rotIdx <= 100)
    {
        P = buildRotMatP(A, numOfPoints);
        PTranspose = transpose(P, numOfPoints, numOfPoints);
        ATag = multiplyMats(multiplyMats(PTranspose, A, numOfPoints), P, numOfPoints);
        if (rotIdx == 1)
        {
            productOfPs = P;
        }
        else
            productOfPs = multiplyMats(productOfPs, P, numOfPoints);
        if (getSumOfSquaresOffDiag(A, numOfPoints) - getSumOfSquaresOffDiag(ATag, numOfPoints) <= EPSILON)
        {
            break;
        }
        A = ATag;
        rotIdx++;
    }
    sortedEIGENS = buildEIGENArr(productOfPs, A, numOfPoints);
    printf("%f\n", sortedEIGENS[0].eigenVal);
    quickSortByEigenVal(sortedEIGENS,0, numOfPoints);
    exit(1);

    /*TODO: if (k==0)
    {
        
    } */
    

    getSortedEigens(A, numOfPoints);

    printf("%f\n", LnormMat[2][2]);
    printf("%f\n", LnormMat[2][0]);
    printf("%f\n", LnormMat[1][0]);
    exit(1);
}

void createRenormalizedMat(double ***mat, double ***jacobi, int k, int n)
{
    int i, j;
    /*create zero mat*/
    *jacobi = transpose(jacobi, n, k);
    *mat = (double **)calloc(k, sizeof(double *));
    assert(*mat != NULL);
    for (i = 0; i < k; i++)
    {
        (*mat)[i] = (double *)calloc(n, sizeof(double));
        assert((*mat)[i] != NULL);
        for (j = 0; j < n; j++)
        {
            (*mat)[i][j] = (*jacobi)[i][j] / normalizedSumRow(n, (*jacobi)[i]);
        }
    }
}

void createTheNormalizedGraphLaplacian(double ***lnorm, double ***wam, double ***ddg, int n)
{
    int i, j, k;
    /*create zero mat*/
    *lnorm = (double **)calloc(n, sizeof(double *));
    assert(*lnorm != NULL);
    for (i = 0; i < n; i++)
    {
        (*lnorm)[i] = (double *)calloc(n, sizeof(double));
        assert((*lnorm)[i] != NULL);
    }
    *ddg = hofchit(*ddg, n);
    *lnorm = multiplyMats(*ddg, *wam, n);
    *lnorm = multiplyMats(*lnorm, *ddg, n);

    for (j = 0; j < n; j++)
    {
        for (k = 0; k < n; k++)
        {
            if (j == k)
            {
                (*lnorm)[j][k] = 1 - (*lnorm)[j][k];
            }
            else
            {
                (*lnorm)[j][k] = 0 - (*lnorm)[j][k];
            }
        }
    }
}