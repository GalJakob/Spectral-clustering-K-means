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
#define MAX_ROTATIONS 100

int main(int argc, char *argv[])
{
    /* this function is called if the spkmeans.c is called via cmd  */
    int k;
    char *goal;
    char *fileName;
    k = 0;
    if (argc == 3)
    {
        goal = argv[1];
        fileName = argv[2];
        if (!strcmp(goal, "jacobi"))
            k = getNumOfPoints(fileName);
        execByGoal(&k, goal, fileName);
    }
    else
    {
        printf("Invalid Input!");
        exit(0);
    }
    return 0;
}

double **execByGoal(int *k, char *goal, char *filename)
{
    /* executes by given goal, and stops at goal */

    int numOfPointsArg, numOfCordsArg;
    double **pointArrPtr;
    double **weightedAdjMat;
    double **diagonalDegreeMat;
    double **LnormMat;
    double **eigenVectorsMat;
    double **finalMat;
    EIGEN *sortedEigensPtr;
    eigenVectorsMat = NULL;
    sortedEigensPtr = NULL;
    finalMat = NULL;

    assignPoints(&pointArrPtr, &filename, &numOfPointsArg, &numOfCordsArg);
    if (!strcmp(goal, "jacobi"))
    {
        performJacobiAlg(pointArrPtr, numOfPointsArg, &(*k), &eigenVectorsMat, &sortedEigensPtr);
        printJacobiResults(numOfPointsArg, *k, eigenVectorsMat, sortedEigensPtr);

        customFreeForMat(eigenVectorsMat, numOfPointsArg);
        free(sortedEigensPtr); 
    }
    else /* all other goals are part of spk */
    {
        createWeightedAdjMat(&weightedAdjMat, &pointArrPtr, &numOfPointsArg, &numOfCordsArg);
        if (!strcmp(goal, "wam"))
        {

            printMat(weightedAdjMat, numOfPointsArg, numOfPointsArg);
            customFreeForMat(pointArrPtr, numOfPointsArg);
            customFreeForMat(weightedAdjMat, numOfPointsArg);
        }

        else
        {
            createDiagonalDegreeMat(&diagonalDegreeMat, &weightedAdjMat, numOfPointsArg);
            if (!strcmp(goal, "ddg"))
            {
                printMat(diagonalDegreeMat, numOfPointsArg, numOfPointsArg);
                customFreeForMat(pointArrPtr, numOfPointsArg);
                customFreeForMat(weightedAdjMat, numOfPointsArg);
                customFreeForMat(diagonalDegreeMat, numOfPointsArg);
            }

            else
            {
                createTheNormalizedGraphLaplacian(&LnormMat, &weightedAdjMat, &diagonalDegreeMat, numOfPointsArg);
                if (!strcmp(goal, "lnorm"))
                {
                    printMat(LnormMat, numOfPointsArg, numOfPointsArg);
                    customFreeForMat(pointArrPtr, numOfPointsArg);
                    customFreeForMat(weightedAdjMat, numOfPointsArg);
                    customFreeForMat(diagonalDegreeMat, numOfPointsArg);
                    customFreeForMat(LnormMat, numOfPointsArg);
                }
                else /* spk */
                {
                    performJacobiAlg(LnormMat, numOfPointsArg, &(*k), &eigenVectorsMat, &sortedEigensPtr);
                    createRenormalizedMat(&finalMat, &eigenVectorsMat, **(&k), numOfPointsArg);
                }
            }
        }
    }

    if (!strcmp(goal, "spk"))
        return finalMat;
    else
        return NULL;
}

void createWeightedAdjMat(double ***weightedAdjMat, double ***pointArrPtr, int *numOfPoints, int *numOfCords)
{
    /* creates weighted adjecency matrix */
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
    /* creates diagonal degree matrix */
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

void performJacobiAlg(double **LnormMat, int numOfPoints, int *k, double ***eigenVecsMat, EIGEN **sortedEigensPtr)
{
    /* performs the Jacobi algorithm and gets the eigenvales and eigenvectors of Lnrom */
    int rotIdx, isDiagMat;
    double **P, **PTranspose, **A, **ATag, **productOfPs,
        **kVecsMat, **PtransMultA, **tempProdOfP;
    double pivotValue;
    EIGEN *sortedEIGENS;
    productOfPs = NULL;
    rotIdx = 1;
    A = LnormMat;
    pivotValue = 0;
    isDiagMat = checkIfDiagMat(LnormMat, numOfPoints, numOfPoints);
    if (isDiagMat)
    {
        P = buildRotMatP(A, numOfPoints, &pivotValue);
        productOfPs = P;
    }
    else
    {
        while (rotIdx <= 100)
        {
            P = buildRotMatP(A, numOfPoints, &pivotValue);
            PTranspose = transpose(P, numOfPoints, numOfPoints);
            PtransMultA = multiplyMats(PTranspose, A, numOfPoints);
            ATag = multiplyMats(PtransMultA, P, numOfPoints);
            if (rotIdx == 1)
                productOfPs = P;
            else
            {
                tempProdOfP = productOfPs;
                productOfPs = multiplyMats(productOfPs, P, numOfPoints);
                customFreeForMat(tempProdOfP, numOfPoints);
            }
            if (getSumOfSquaresOffDiag(A, numOfPoints) - getSumOfSquaresOffDiag(ATag, numOfPoints) <= EPSILON)
            {
                if (rotIdx > 1)
                    customFreeForMat(P, numOfPoints);
                customFreeForMat(PTranspose, numOfPoints);
                customFreeForMat(PtransMultA, numOfPoints);
                break;
            }
            if (rotIdx > 1)
                customFreeForMat(P, numOfPoints);
            customFreeForMat(PTranspose, numOfPoints);
            customFreeForMat(PtransMultA, numOfPoints);
            customFreeForMat(A, numOfPoints);
            A = ATag;
            rotIdx++;
        }
    }
    sortedEIGENS = buildEIGENArr(productOfPs, A, numOfPoints);
    quickSortByEigenVal(sortedEIGENS, 0, numOfPoints - 1);
    if (!(*k))
        *k = getKeigengapHeuristic(sortedEIGENS, numOfPoints);
    kVecsMat = createKVecsMat(sortedEIGENS, numOfPoints, *k);
    *eigenVecsMat = kVecsMat;
    *sortedEigensPtr = sortedEIGENS;
}

void createRenormalizedMat(double ***mat, double ***jacobi, int k, int n)
{
    /* creates renormalized matrix from a given one */
    int i, j;
    double sumRow = 0;

    *mat = (double **)calloc(n, sizeof(double *));
    customAssert(*mat != NULL);
    for (i = 0; i < n; i++)
    {
        (*mat)[i] = (double *)calloc(k, sizeof(double));

        customAssert((*mat)[i] != NULL);
        sumRow = normalizedSumRow(k, (*jacobi)[i]);
        for (j = 0; j < k; j++)
        {
            if (!sumRow)
                (*mat)[i][j] = 0;
            else
                (*mat)[i][j] = (*jacobi)[i][j] / sumRow;
        }
    }
}

void createTheNormalizedGraphLaplacian(double ***lnorm, double ***wam, double ***ddg, int n)
{
    /*creates normalized graph laplacian*/
    int i, j, k;
    double **tempLnorm;
    *lnorm = (double **)calloc(n, sizeof(double *));
    customAssert(*lnorm != NULL);
    for (i = 0; i < n; i++)
    {
        (*lnorm)[i] = (double *)calloc(n, sizeof(double));
        customAssert((*lnorm)[i] != NULL);
    }
    *ddg = hofchit(*ddg, n);
    *lnorm = multiplyMats(*ddg, *wam, n);
    tempLnorm = *lnorm;
    *lnorm = multiplyMats(*lnorm, *ddg, n);
    customFreeForMat(tempLnorm, n);

    for (j = 0; j < n; j++)
    {
        for (k = 0; k < n; k++)
        {
            if (j == k)
            {
                if ((*lnorm)[j][k])
                    (*lnorm)[j][k] = 1;
                else
                    (*lnorm)[j][k] = 1 - (*lnorm)[j][k];
            }
            else
            {

                if (((*lnorm)[j][k]) == 0)
                {
                    (*lnorm)[j][k] = 0;
                    continue;
                }
                (*lnorm)[j][k] = -(*lnorm)[j][k];
            }
        }
    }
}
