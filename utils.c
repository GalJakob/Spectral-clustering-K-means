#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "spkmeans.h"

#define LINE_LENGTH 256
#define PI 3.141592653589793

void customAssert(int booleanVal)
{
    /* asserting that no unexpected behavior is in the program*/
    if (!booleanVal)
    {
        printf("An Error Has Occured");
        abort();
    }
}
void assignPoints(double ***pointArrPtr, char **inFileNamePtr, int *numOfPointsArg, int *numOfCordsArg)
{

    /* this functions gets the points from the input file and puts them into an array.
    in addition,computes number of points and point dimension */

    char line[LINE_LENGTH];
    char *splittedLine;

    double cordinateVal;
    int numOfCords = 0;
    int numOfCordsIdx = 0;
    int numOfPoints = 0;
    int numOfPointsIdx = 0;

    FILE *filePtr;
    computeNumOfCordsAndPoints(&filePtr, &numOfCords, &numOfPoints, &inFileNamePtr);

    *numOfCordsArg = numOfCords;
    *numOfPointsArg = numOfPoints;
    *pointArrPtr = (double **)malloc((sizeof(double *)) * numOfPoints);
    customAssert(*pointArrPtr != NULL);
    filePtr = fopen(*inFileNamePtr, "r");
    customAssert(filePtr != NULL);
    while (fgets(line, LINE_LENGTH, filePtr))
    {
        (*pointArrPtr)[numOfPointsIdx] = (double *)malloc(numOfCords * sizeof(double));
        customAssert((*pointArrPtr)[numOfPointsIdx] != NULL);
        for (numOfCordsIdx = 0; numOfCordsIdx < numOfCords; numOfCordsIdx++)
        {
            if (numOfCordsIdx == 0)
                splittedLine = strtok(line, ",");
            else
                splittedLine = strtok(NULL, ",");
            cordinateVal = atof(splittedLine);
            (*pointArrPtr)[numOfPointsIdx][numOfCordsIdx] = cordinateVal;
        }
        numOfPointsIdx++;
        numOfCordsIdx = 0;
    }
    fclose(filePtr);
}

void computeNumOfCordsAndPoints(FILE **filePtr, int *numOfCords, int *numOfPoints, char ***inFileNamePtr)
{
    /* this function computes the dimension and number of points */
    char line[LINE_LENGTH];
    char *splittedLine;
    *filePtr = fopen(**inFileNamePtr, "r");
    customAssert(*filePtr != NULL);

    while (fgets(line, LINE_LENGTH, *filePtr))
    {
        *numOfPoints = *numOfPoints + 1;
        if (*numOfCords != 0)
            continue;
        else
        {
            do
            {
                if (*numOfCords == 0)
                    splittedLine = strtok(line, ",");
                else
                    splittedLine = strtok(NULL, ",");
                *numOfCords = *numOfCords + 1;
            } while (splittedLine != NULL);
            *numOfCords = *numOfCords - 1;
        }
    }
    if (fclose(*filePtr) != 0)
    {
        printf("%s", "An Error Has Occurred");
        return;
    }
}

double getEuclideanNorm(double *point1, double *point2, int numOfCords)
{
    int idx;
    double diffBetweenCords;
    double sum = 0;

    for (idx = 0; idx < numOfCords; idx++)
    {

        diffBetweenCords = point1[idx] - point2[idx];
        diffBetweenCords *= diffBetweenCords;
        sum += diffBetweenCords;
    }

    return sqrt(sum);
}

double SumMatRow(double *matRow, int n)
{
    double res;
    res = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        res += matRow[i];
    }

    return res;
}

double **hofchit(double **ddg, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        ddg[i][i] = 1 / sqrt(ddg[i][i]);
    }
    return ddg;
}

double **buildRotMatP(double **LnormMat, double numOfPoints)
{
    /* builds the rotation matrix of the Jacobi algorithm */
    int pivRow, pivCol;
    double pivot, phiAngle;

    pivot = getPivotAndHisIdxs(LnormMat, numOfPoints, &pivRow, &pivCol);
    phiAngle = getPhiAngle(LnormMat, pivRow, pivCol);
    return allocateAndCreateP(phiAngle, numOfPoints, pivRow, pivCol);
}

double getPivotAndHisIdxs(double **mat, double numOfPoints, int *pivRow, int *pivCol)
{
    /* gets the off-diagonal element of mat with largest absolute value and his row and column */
    int row, col, pivRow, pivCol;
    double elWithMaxAbsVal = 0;
    for (row = 0; row < numOfPoints; row++)
    {
        for (col = row; col < numOfPoints; col++)
        {
            if (row == col)
                continue;
            if (fabs(mat[row][col]) > fabs(elWithMaxAbsVal))
            {
                elWithMaxAbsVal = mat[row][col];
                pivRow = row;
                pivCol = col;
            }
        }
    }
    *pivRow = pivRow;
    *pivCol = pivCol;
    return elWithMaxAbsVal;
}

double arcCot(double x)
{
    /* gets the arc cot value of x */
    return PI / 2 - atan(x);
}

double getPhiAngle(double **LnormMat, int pivRow, int pivCol)
{
    /* gets the phi angle  */
    double PhiAngle2Times = (LnormMat[pivCol][pivCol] - LnormMat[pivRow][pivRow]) / (2 * LnormMat[pivRow][pivCol]);
    return arcCot(PhiAngle2Times) / 2;
}
double **allocateAndCreateP(double phiAngle, int numOfPoints, int pivRow, int pivCol)
{
    /* creates the mat in memory and completes it's build  */
    double **P;
    int row;
    P = calloc(sizeof(double *), numOfPoints);
    customAssert(P != NULL);
    for (row = 0; row < numOfPoints; row++)
    {
        P[row] = calloc(sizeof(double), numOfPoints);
        customAssert(P[row] != NULL);
        P[row][row] = 1;
    }
    P[pivRow][pivRow] = cos(phiAngle); /* c */   /* TODO:check extreme cases and division by 0 */
    P[pivCol][pivCol] = cos(phiAngle); /* c */   /* TODO:check extreme cases and division by 0 */
    P[pivRow][pivCol] = sin(phiAngle); /* s */   /* TODO:check extreme cases */
    P[pivCol][pivRow] = -sin(phiAngle); /* -s */ /* TODO:check extreme cases */

    return P;
}

double getSumOfSquaresOffDiag(double **mat, int numOfPoints)
{
    int row, col;
    double sum = 0;
    for (row = 0; row < numOfPoints; row++)
    {
        for (col = 0; col < numOfPoints; col++)
        {
            if (row == col)
                continue;
            sum+=(mat[row][col])*(mat[row][col]);
        }
    }
    return sum;
}