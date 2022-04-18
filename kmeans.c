#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define LINE_LENGTH 256

void assignVars(int *, int *, char **, char **, char **, int);
void assignPoints(int *, char **);

int main(int argc, char *argv[])
{
    int K;
    int max_iter;
    int pointsArr[1];
    char *inFileNamePtr;
    char *outFileNamePtr;

    assignVars(&K, &max_iter, &inFileNamePtr, &outFileNamePtr, argv, argc);
    assignPoints(pointsArr, &inFileNamePtr);

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

void assignPoints(int *pointArrPtr, char **inFileNamePtr)
{
    char line[LINE_LENGTH];
    char *splittedLine;
    FILE *filePtr;

    filePtr = fopen(*inFileNamePtr, "r");

    while (fgets(line, LINE_LENGTH, filePtr))
    {
        splittedLine = strtok(line,",");
        printf("%s\n", splittedLine);
    }
    fclose(filePtr);
}

/* @pre:string must not contain letters */
int convertStrToNumber(char *strToConvert){
    
}
