#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define LINE_LENGTH 256
#define DEFAULT_MAX_ITER 10000
#define EPSILON 0.001
#define PERCISION 10000

void assignVars(int *, int *, char **, char **, char **, int);
void assignPoints(double ***, char **, int *, int *);
void computeNumOfCordsAndPoints(FILE **, int *, int *, char ***);
void initializeCentroids(double ***, double ***, int, int);
void mainAlgorithm(double ***, double ***, int, int, int, int);

int main(int argc, char *argv[])
{
    int K;
    int max_iter = DEFAULT_MAX_ITER;
    int numOfPoints;
    int numOfCords;
    double **pointsArr;
    double **centroids;
    char *inFileNamePtr;
    char *outFileNamePtr;

    /* reminder:arr == &arr[0] :*/
    assignVars(&K, &max_iter, &inFileNamePtr, &outFileNamePtr, argv, argc);
    assignPoints(&pointsArr, &inFileNamePtr, &numOfPoints, &numOfCords);
    initializeCentroids(&pointsArr, &centroids, K, numOfCords);
    mainAlgorithm(&pointsArr, &centroids, max_iter, numOfPoints, numOfCords, K);

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

void assignPoints(double ***pointArrPtr, char **inFileNamePtr, int *numOfPointsArg, int *numOfCordsArg)
{
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

void initializeCentroids(double ***pointArrPtr, double ***centroidsPtr, int K, int numOfCords)
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
}
void output(double ***centroidsPtr, int k, int numOfCords, char **outFileNamePtr)
{
    FILE *res;
    char comma = ',';
    int i = 0;
    res = fopen(*outFileNamePtr, "w");
    for (; i < k; i++)
    {
        int j = 0;
        for (; j < numOfCords - 1; j++)
        {
            fprintf(res, "%.4f", *centroidsPtr[i][j]);
            fprintf(res, "%s", ",");
        }
        fprintf(res, "%.4f", *centroidsPtr[i][j]);
        fprintf(res, "\n");
    }
    fclose(res);
}
void mainAlgorithm(double ***pointsArrPtr, double ***centroidsArrPtr, int max_iter, int numOfPoints, int numOfCords, int K)
{
    int iterCnt = 1;
    int isAllEuclidiansUnderEps = 0; /*boolean*/
    int pointIdx = 0;
    int pointIdxForInitCluster;
    int cordForCluster;

    double **clustersSumArrPtr;
    int *numOfPointsInCluster = (int *)malloc(K * sizeof(int));

    clustersSumArrPtr = (double **)malloc((sizeof(double *)) * K);
    for (pointIdxForInitCluster = 0; pointIdxForInitCluster < K; pointIdxForInitCluster++)
    {
        clustersSumArrPtr[pointIdxForInitCluster] = (double *)malloc(numOfCords * sizeof(double));
        for (cordForCluster = 0; cordForCluster < numOfCords; cordForCluster++)
            clustersSumArrPtr[pointIdxForInitCluster][cordForCluster] = 0;
    }

    /* numOfPointsInCluster[1] = 2;
    printf("%p \n",numOfPointsInCluster);
    printf("%p",numOfPointsInCluster+1); */
    while ((iterCnt != max_iter) && (!isAllEuclidiansUnderEps))
    {
        for (pointIdx = 0; pointIdx < numOfPoints; pointIdx++)
        {
            double *point = (*pointsArrPtr)[pointIdx];
            double minDistance = DBL_MAX;
            int chosenClusterIdx = 0;
            int clusterIdx = 0;
            int cordIdx = 0;

            for (clusterIdx = 0; clusterIdx < K; clusterIdx++)
            {
                double tempDist = 0;

                for (cordIdx = 0; cordIdx < numOfCords; cordIdx++)
                {
                    printf("%f \n", point[cordIdx]);
                    printf("%f \n", (*centroidsArrPtr)[clusterIdx][cordIdx]);
                    tempDist += pow(point[cordIdx] - ((*centroidsArrPtr)[clusterIdx][cordIdx]), 2);
                }

                if (tempDist < minDistance)
                {
                    chosenClusterIdx = clusterIdx;
                    minDistance = tempDist;
                }
            }
            for (cordIdx = 0; cordIdx < numOfCords; cordIdx++)
                clustersSumArrPtr[chosenClusterIdx][cordIdx] += point[cordIdx];
            /*  printf("%d \n", chosenClusterIdx); */
            /*  printf("%f \n", clustersSumArrPtr[chosenClusterIdx][cordIdx]); */
            /* printf("%p", numOfPointsInCluster + 1); */

            exit(0);

            /*
                for v in range(point_len):
                  c_sum[cluster][v] += float(point[v]) */
        }

        /*  point_len = len(datapoints_arr[0]) */
    }
}