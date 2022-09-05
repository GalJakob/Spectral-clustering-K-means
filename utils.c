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
#define EPSILON 0.00001

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
           /*  printf("aaa   %s",splittedLine);
            exit(1); */
            cordinateVal = strtod(splittedLine,&splittedLine);
            (*pointArrPtr)[numOfPointsIdx][numOfCordsIdx] = cordinateVal;
        }

        numOfPointsIdx++;
        numOfCordsIdx = 0;
    }
    fclose(filePtr);
    /* printMat(*pointArrPtr,numOfPoints,numOfPoints);
    exit(1); */
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
        abort();
    }
}

int getNumOfPoints(char *fileName)
{
    /* gets only the number of points from a given file */
    int numOfPoints;
    char line[LINE_LENGTH];
    FILE *filePtr;

    filePtr = fopen(fileName, "r");
    customAssert(filePtr != NULL);

    numOfPoints = 0;

    while (fgets(line, LINE_LENGTH, filePtr))
        numOfPoints++;
    if (fclose(filePtr) != 0)
    {
        printf("%s", "An Error Has Occurred");
        abort();
    }
    return numOfPoints;
}

double getEuclideanNorm(double *point1, double *point2, int numOfCords)
{
    /* calculates the euclidean norm between two points */
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
    /* calculates the sum of row in matrix */
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
    /* changes given matrix to be the inverse matrix */
    int i;
    for (i = 0; i < n; i++)
    {
        ddg[i][i] = 1 / sqrt(ddg[i][i]);
    }
    return ddg;
}

double **multiplyMats(double **mat1, double **mat2, int n)
{
    /* calculates the mult of the mats and returns it */
    int i, j, k;
    double **res;
    res = (double **)calloc(n, sizeof(double *));
    customAssert(res != NULL);
    for (i = 0; i < n; i++)
    {
        res[i] = (double *)calloc(n, sizeof(double));
        customAssert(res[i] != NULL);
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
    /* calculates the transpose of a matrix and returns it */
    int i, j;
    double **transposeMat;
    transposeMat = (double **)calloc(k, sizeof(double *));
    customAssert(transposeMat != NULL);
    for (i = 0; i < k; i++)
    {
        transposeMat[i] = (double *)calloc(n, sizeof(double));
        customAssert(transposeMat[i] != NULL);
        for (j = 0; j < n; j++)
        {
            transposeMat[i][j] = mat[j][i];
        }
    }
    return transposeMat;
}

double normalizedSumRow(int k, double *row)
{
    /* normalize the row and returns result */
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

    return allocateAndCreateP(phiAngle, numOfPoints, pivRow, pivCol);
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
    P[pivRow][pivRow] = fabs(cos(phiAngle)) <= 0.001? 0 : cos(phiAngle); /* c */         /* TODO:check extreme cases and division by 0 */
    P[pivCol][pivCol] = fabs(cos(phiAngle)) <= 0.001 ? 0 : cos(phiAngle); /* c */         /* TODO:check extreme cases and division by 0 */
    P[pivRow][pivCol] = fabs(sin(phiAngle)) <= 0.001 ? 0 : sin(phiAngle);                /* TODO:check extreme cases */
    P[pivCol][pivRow] = fabs(sin(phiAngle)) <= 0.001 ? 0 : -sin(phiAngle); /* -s */ /* TODO:check extreme cases */
    /* TODO:check where can free */

    return P;
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
    /* builds the array of EIGENS objects  */
    int idx, idxForTempVec;
    EIGEN *eigenArr;
    double *tempVec;
    eigenArr = (EIGEN *)calloc(numOfPoints, sizeof(EIGEN));
    customAssert(eigenArr != NULL);
    for (idx = 0; idx < numOfPoints; idx++)
    {
        tempVec = calloc(numOfPoints, sizeof(double));
        for (idxForTempVec = 0; idxForTempVec < numOfPoints; idxForTempVec++)
            tempVec[idxForTempVec] = productOfPs[idxForTempVec][idx];

        eigenArr[idx].eigenVec = tempVec;
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

double getSumOfSquaresOffDiag(double **mat, int numOfPoints)
{
    /* sqaures each element only if he is not in main diagonal and sums it  */

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

void customFreeForMat(double **mat, int numOf1Dptrs)
{
    /* frees memory for mat*/

    int ptrIdx;
    for (ptrIdx = 0; ptrIdx < numOf1Dptrs; ptrIdx++)
        free(mat[ptrIdx]);
    free(mat);
}

void swap(EIGEN *a, EIGEN *b)
{
    /* swaps the EIGENS in quicksort algorithm */
    EIGEN *temp;
    temp = malloc(sizeof(EIGEN));
    customAssert(temp != NULL);
    *temp = *a;
    *a = *b;
    *b = *temp;
    free(temp);
}

int partition(EIGEN *arr, int low, int high)
{
    /* partition of quicksort */

    double pivot;
    int i, j;
    i = low - 1;
    pivot = arr[high].eigenVal;
    for (j = low; j < high; j++)
    {
        if (arr[j].eigenVal >= pivot)
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
    /* quick sort algorithm on EIGENS array by their eigen value value */

    if (low < high)
    {
        int pivotIdx;
        pivotIdx = partition(arr, low, high);
        quickSortByEigenVal(arr, low, pivotIdx - 1);
        quickSortByEigenVal(arr, pivotIdx + 1, high);
    }
}

int getKeigengapHeuristic(EIGEN *EIGENS, int numOfPoints)
{
    /* gets K by the eigen gap heuristic */

    int maxDiffIdx, idx;
    double maxDiff, currDiff;
    maxDiffIdx = 0;
    maxDiff = EIGENS[0].eigenVal - EIGENS[1].eigenVal;
    for (idx = 1; idx < numOfPoints / 2; idx++)
    {
        currDiff = EIGENS[idx].eigenVal - EIGENS[idx + 1].eigenVal;
        if (currDiff > maxDiff)
        {
            maxDiffIdx = idx;
            maxDiff = currDiff;
        }
    }
    return maxDiffIdx + 1;
}

double **createKVecsMat(EIGEN *EIGENS, int numOfPoints, int k)
{
    /* creates a matrix that has K columns as vectors */
    int idx, rowIdx, colIdx;
    double **kVecsMat;
    kVecsMat = (double **)calloc(numOfPoints, sizeof(double *));
    customAssert(kVecsMat != NULL);
    for (idx = 0; idx < numOfPoints; idx++)
    {
        kVecsMat[idx] = (double *)calloc(k, sizeof(double));
        customAssert(kVecsMat[idx] != NULL);
    }
    for (colIdx = 0; colIdx < k; colIdx++)
    {
        for (rowIdx = 0; rowIdx < numOfPoints; rowIdx++)
            kVecsMat[rowIdx][colIdx] = EIGENS[colIdx].eigenVec[rowIdx];
    }
    return kVecsMat;
}

void printJacobiResults(int numOfPointsArg, int k, double **eigenVectorsMat, EIGEN *sortedEigensPtr)
{
    /* prints the eigen values and then the eigen vectors */

    int sortedEigIdx;
    for (sortedEigIdx = 0; sortedEigIdx < numOfPointsArg; sortedEigIdx++)
        sortedEigIdx != numOfPointsArg - 1 ? printf("%.4f,", sortedEigensPtr[sortedEigIdx].eigenVal) : printf("%.4f\n", sortedEigensPtr[sortedEigIdx].eigenVal);
    printMat(eigenVectorsMat, numOfPointsArg, k);
}

void printMat(double **mat, int numOfRows, int numOfCols)
{
    /* prints 2D matrix */
    int rowIdx;
    int colIdx;
    for (rowIdx = 0; rowIdx < numOfRows; rowIdx++)
    {
        for (colIdx = 0; colIdx < numOfCols; colIdx++)
        {
            if (colIdx != numOfCols - 1)
                printf("%.4f,", mat[rowIdx][colIdx]);
            else
                rowIdx != numOfRows - 1 ? printf("%.4f\n", mat[rowIdx][colIdx]) : printf("%.4f", mat[rowIdx][colIdx]);
        }
    }
}
