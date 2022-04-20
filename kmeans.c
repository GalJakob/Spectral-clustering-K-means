#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define LINE_LENGTH 256
#define PERCISION 10000

void assignVars(int *, int *, char **, char **, char **, int);
void assignPoints(float **, char **);
void computeNumOfCordsAndPoints(FILE **, int *, int *, char ***);

int main(int argc, char *argv[])
{
    int K;
    int max_iter;
    float *pointsArr[1];
    char *inFileNamePtr;
    char *outFileNamePtr;

    /* reminder:arr == &arr[0] :*/
    assignVars(&K, &max_iter, &inFileNamePtr, &outFileNamePtr, argv, argc);
    assignPoints(pointsArr, &inFileNamePtr);

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

void assignPoints(float **pointArrPtr, char **inFileNamePtr)
{
    char line[LINE_LENGTH];
    char *splittedLine;

    float cordinateVal;
    float *pointPtr;
    int numOfCords = 0;
    int numOfCordsIdx = 0;
    int numOfPoints = 0;
    int numOfPointsIdx = 0;

    FILE *filePtr;

    computeNumOfCordsAndPoints(&filePtr, &numOfCords, &numOfPoints, &inFileNamePtr);

    *pointArrPtr = (float *)malloc((sizeof(float *)) * numOfPoints);

    filePtr = fopen(*inFileNamePtr, "r");
    while (fgets(line, LINE_LENGTH, filePtr))
    {
        pointArrPtr[numOfPointsIdx] = malloc(numOfCords * sizeof(float));
        printf("%s", line);

        for (numOfCordsIdx = 0; numOfCordsIdx < numOfCords; numOfCordsIdx++)
        {
            if (numOfCordsIdx == 0)
                splittedLine = strtok(line, ",");
            else
                splittedLine = strtok(NULL, ",");
            cordinateVal = strtof(splittedLine, NULL);
           /*  free(splittedLine); */
           /*  pointArrPtr[numOfPointsIdx][numOfCordsIdx] = cordinateVal; */
        }
       /*  free(line); */
        printf(" %d \n", numOfPointsIdx);
        numOfPointsIdx++;
        numOfCordsIdx = 0;
    }
    printf("%f", pointArrPtr[2][1]);
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
