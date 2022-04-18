#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define LINE_LENGTH 256
#define PERCISION 100

void assignVars(int *, int *, char **, char **, char **, int);
void assignPoints(float *, char **);
void computeNumOfCords(FILE **, int *, char ***);

int main(int argc, char *argv[])
{
    int K;
    int max_iter;
    float pointsArr[1][1];
    char *inFileNamePtr;
    char *outFileNamePtr;

    assignVars(&K, &max_iter, &inFileNamePtr, &outFileNamePtr, argv, argc);
    assignPoints(*pointsArr, &inFileNamePtr);

    printf("%d", pointsArr[0]);
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

void assignPoints(float *pointArrPtr, char **inFileNamePtr)
{
    char line[LINE_LENGTH];
    char *splittedLine;

    float cordinateVal;
    float *pointPtr;
    int numOfCords = 0;
    int numOfCordsIdx = 0;

    FILE *filePtr;

    computeNumOfCords(&filePtr, &numOfCords, &inFileNamePtr);
    pointPtr = malloc(sizeof(float) * numOfCords);

    filePtr = fopen(*inFileNamePtr, "r");

    while (fgets(line, LINE_LENGTH, filePtr))
    {
        for (numOfCordsIdx = 0; numOfCordsIdx < numOfCords; numOfCordsIdx++)
        {
            if (numOfCordsIdx == 0)
                splittedLine = strtok(line, ",");
            else
                splittedLine = strtok(NULL, ",");
            cordinateVal = strtof(splittedLine, NULL);
            pointPtr[numOfCordsIdx] = cordinateVal;
        }
        numOfCordsIdx = 0;
        printf("%f", pointPtr[4]);
        exit(0);
    }
    fclose(filePtr);
}

void computeNumOfCords(FILE **filePtr, int *numOfCords, char ***inFileNamePtr)
{
    char line[LINE_LENGTH];
    char *splittedLine;
    *filePtr = fopen(**inFileNamePtr, "r");
    fgets(line, LINE_LENGTH, *filePtr);
    do
    {
        if (*numOfCords == 0)
            splittedLine = strtok(line, ",");
        else
            splittedLine = strtok(NULL, ",");
        *numOfCords = *numOfCords + 1;
    } while (splittedLine != NULL);
    *numOfCords = *numOfCords - 1;
    fclose(*filePtr);
}

void appendCordsToPoint(float **pointPtr, float cordinateVal, int *numOfCordsPtr)
{
    *pointPtr = (float *)realloc(*pointPtr, sizeof(float) * (*numOfCordsPtr + 1));
    *pointPtr[*numOfCordsPtr] = cordinateVal;
    *numOfCordsPtr = *numOfCordsPtr + 1;
}
