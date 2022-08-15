#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "spkmeans.h"
#define LINE_LENGTH 256

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

double SumMatRow(double * matRow, int n) {
    double res;
    res = 0;
    int i;
    for (i = 0; i < n; i++){
        res += matRow[i];
    }

    return res;
}

double ** hofchit (double ** ddg, int n){
    int i;
    for (i = 0; i < n; i++){
        ddg[i][i] = 1 / sqrt(ddg[i][i]);
    }
    return ddg;
}