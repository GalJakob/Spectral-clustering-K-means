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
    int i;
    res = 0;
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

double **multiplyMats(double **mat1, double **mat2, int n)
{
    int i, j, k;
    double **res;
    res = (double **)calloc(n, sizeof(double *));
    assert(res != NULL);
    for (i = 0; i < n; i++)
    {
        res[i] = (double *)calloc(n, sizeof(double));
        assert(res[i] != NULL);
        for (j = 0; j < n; j++)
        {
            for (k = 0; k < n; k++)
            {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return res;
}

double **transpose(double **mat, int n, int k)
{
    int i, j;
    double **transposeMat;
    transposeMat = (double **)calloc(k, sizeof(double *));
    assert(transposeMat != NULL);
    for (i = 0; i < k; i++)
    {
        transposeMat[i] = (double *)calloc(n, sizeof(double));
        assert(transposeMat[i] != NULL);
        for (j = 0; j < n; j++)
        {
            transposeMat[i][j] = mat[j][i];
        }
    }
    return transposeMat;
}

double normalizedSumRow(int k, double *row)
{
    double res = 0;
    int i;
    for (i = 0; i < k; i++)
    {
        res += pow(row[i], 2);
    }
    res = sqrt(res);
    return res;
}

double **buildRotMatP(double **LnormMat, int numOfPoints)
{
    /* builds the rotation matrix of the Jacobi algorithm */
    int pivRow, pivCol;
    double phiAngle;

    getPivotAndHisIdxs(LnormMat, numOfPoints, &pivRow, &pivCol);
    phiAngle = getPhiAngle(LnormMat, pivRow, pivCol);
    /*   printf("%d\n", pivRow);
      printf("%d\n", pivCol); */

    return allocateAndCreateP(phiAngle, numOfPoints, pivRow, pivCol);
}

void getPivotAndHisIdxs(double **mat, int numOfPoints, int *pivRow, int *pivCol)
{
    /* gets the off-diagonal element of mat with largest absolute value and his row and column */
    int row, col, pivRow2, pivCol2;
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
                pivRow2 = row;
                pivCol2 = col;
            }
        }
    }
    *pivRow = pivRow2;
    *pivCol = pivCol2;
}

EIGEN *buildEIGENArr(double **productOfPs, double **A, int numOfPoints)
{

    int idx;
    EIGEN *eigenArr;
    eigenArr = (EIGEN *)calloc(numOfPoints, sizeof(EIGEN));
    customAssert(eigenArr != NULL);
    for (idx = 0; idx < numOfPoints; idx++)
    {
        eigenArr[idx].eigenVec = &productOfPs[0][idx];
        eigenArr[idx].eigenVal = A[idx][idx];
    }
    return eigenArr;
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
    P = calloc(numOfPoints, sizeof(double *));
    customAssert(P != NULL);
    for (row = 0; row < numOfPoints; row++)
    {
        P[row] = calloc(numOfPoints, sizeof(double));
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
            sum += (mat[row][col]) * (mat[row][col]);
        }
    }
    return sum;
}

void customFreeForMat(double **mat)
{
    /* frees memory for mat*/
    free(mat[0]);
    free(mat);
}
double **multiplyMatWithVector(double **mat1, double **vec, int n)
{

    int i, j, k, p;
    double **res;
    res = (double **)calloc(n, sizeof(double *));
    assert(res != NULL);
    for (i = 0; i < n; i++)
    {
        res[i] = (double *)calloc(1, sizeof(double));
        assert(res[i] != NULL);
    }
    for (p = 0; p < n; p++)
    {
        for (j = 0; j < 1; j++)
        {
            for (k = 0; k < n; k++)
            {
                res[p][j] += mat1[p][k] * vec[k][j];
            }
        }
    }
    return res;
}

/*
void appendRotMat(double ****allRotMatsPtr, double **P)
{
   *allRotMatsPtr = calloc(sizeof(double **), 1);
}
*/

void swap(EIGEN *a, EIGEN *b)
{
   EIGEN* temp;
    temp = a;
    *a = *b;
    *b = *temp;
}

int partition(EIGEN *arr, int low, int high)
{
    double pivot;
    int i, j;
    i = low - 1;
    pivot = arr[high].eigenVal;
    for (j = low; j < high; j++)
    {
        if (arr[j].eigenVal <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);

    return i + 1;
}

void quickSortByEigenVal(EIGEN *arr, int low, int high)
{
    if (low < high)
    {
        int pivotIdx;
        pivotIdx = partition(arr, low, high);
        quickSort(arr, low, pivotIdx - 1);
        quickSort(arr, pivotIdx + 1, high);
    }
}