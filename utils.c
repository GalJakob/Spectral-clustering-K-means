#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#define LINE_LENGTH 256

void customAssert(int booleanVal) {
    /* asserting that no unexpected behavior is in the program*/
    if (!booleanVal) {
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
    filePtr = fopen(*inFileNamePtr, "r");
    while (fgets(line, LINE_LENGTH, filePtr))
    {
        (*pointArrPtr)[numOfPointsIdx] = (double *)malloc(numOfCords * sizeof(double));

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
    if (*filePtr == NULL)
    {
        printf("%s", "An Error Has Occurred");
        return;
    }

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