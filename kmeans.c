#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define LINE_LENGTH 256
#define PERCISION 10000

void assignVars(int *, int *, char **, char **, char **, int);
void assignPoints(double ***, char **, int *, int *);
void computeNumOfCordsAndPoints(FILE **, int *, int *, char ***);
void initializeCentroids(double ***, double ***, int, int, int);
void mainAlgorithm(double ***, double ***);

int main(int argc, char *argv[])
{
    int K;
    int max_iter;
    int numOfPoints;
    int numOfCords;
    double **pointsArr;
    double **centroids;
    char *inFileNamePtr;
    char *outFileNamePtr;

    /* reminder:arr == &arr[0] :*/
    assignVars(&K, &max_iter, &inFileNamePtr, &outFileNamePtr, argv, argc);
    assignPoints(&pointsArr, &inFileNamePtr, &numOfPoints, &numOfCords);
    initializeCentroids(&pointsArr, &centroids, K, numOfPoints, numOfCords);
    mainAlgorithm(&pointsArr, &centroids);

    return 0;
}

/* functions :*/

void assignVars(int *kPtr, int *max_iterPtr, char **inFileNamePtr, char **outFileNamePtr, char **args, int numOfArgs)
{
    if (numOfArgs == 5)
    {
        *kPtr = atoi(args[1]);
        *max_iterPtr = atoi(args[2]);
        *inFileNamePtr = args[3];
        *outFileNamePtr = args[4];
    }
    else
    {
        *kPtr = atoi(args[1]);
        *inFileNamePtr = args[2];
        *outFileNamePtr = args[3];
    }
}

void assignPoints(double ***pointArrPtr, char **inFileNamePtr, int *numOfCordsArg, int *numOfPointsArg)
{
    char line[LINE_LENGTH];
    char *splittedLine;

    double cordinateVal;
    double *pointPtr;
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
            cordinateVal = strtof(splittedLine, NULL);
            (*pointArrPtr)[numOfPointsIdx][numOfCordsIdx] = cordinateVal;
        }
        numOfPointsIdx++;
        numOfCordsIdx = 0;
    }
    fclose(filePtr);
}

void computeNumOfCordsAndPoints(FILE **filePtr, int *numOfCords, int *numOfPoints, char ***inFileNamePtr)
{
    char line[LINE_LENGTH];
    char *splittedLine;
    *filePtr = fopen(**inFileNamePtr, "r");
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
    fclose(*filePtr);
}

void initializeCentroids(double ***pointArrPtr, double ***centroidsPtr, int K, int numOfCords, int numOfPoints)
{
    int pointIdx;
    int cord;

    *centroidsPtr = (double **)malloc((sizeof(double *)) * K);
    for (pointIdx = 0; pointIdx < K; pointIdx++)
    {
        (*centroidsPtr)[pointIdx] = (double *)malloc(numOfCords * sizeof(double));
        for (cord = 0; cord < numOfCords; cord++)
            (*centroidsPtr)[pointIdx][cord] = (*pointArrPtr)[pointIdx][cord];
    }

    printf("%f", (*centroidsPtr)[K - 1][2]);
}
void output(double ***centroidsPtr, int k)
{
    /*  for (int i = 0; i < k; i++)
     {
          for (int j; j < )
          *centroidsPtr[i][0] =
     } */
}
void mainAlgorithm(double ***pointsArrPtr, double ***centroidsArrPtr)
{
    
}
