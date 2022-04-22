#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define LINE_LENGTH 256
#define DEFAULT_MAX_ITER 200
#define EPSILON 0.001

void assignVars(int *, int *, char **, char **, char **, int, int *);
void assignPoints(double ***, char **, int *, int *, int *);
void computeNumOfCordsAndPoints(FILE **, int *, int *, char ***, int **);
void initializeCentroids(double ***, double ***, int, int, int *);
void mainAlgorithm(double ***, double ***, int, int, int, int, int *);
void output(double ***, int, int, char **, int *);

int main(int argc, char *argv[])
{
    int K;
    int shouldTerminate = 0; /* boolean */
    int max_iter = DEFAULT_MAX_ITER;
    int numOfPoints;
    int numOfCords;
    double **pointsArr;
    double **centroids;
    char *inFileNamePtr;
    char *outFileNamePtr;

    
    assignVars(&K, &max_iter, &inFileNamePtr, &outFileNamePtr, argv, argc, &shouldTerminate);
    assignPoints(&pointsArr, &inFileNamePtr, &numOfPoints, &numOfCords, &shouldTerminate);
    initializeCentroids(&pointsArr, &centroids, K, numOfCords, &shouldTerminate);
    mainAlgorithm(&pointsArr, &centroids, max_iter, numOfPoints, numOfCords, K, &shouldTerminate);
    output(&centroids, K, numOfCords, &outFileNamePtr, &shouldTerminate);
    
    return shouldTerminate;
}

/* functions :*/

void assignVars(int *kPtr, int *max_iterPtr, char **inFileNamePtr, char **outFileNamePtr, char **args, int numOfArgs, int *shouldTerminate)
{
    if (numOfArgs == 5)
    {
        *kPtr = atoi(args[1]);
        *max_iterPtr = atoi(args[2]);
        *inFileNamePtr = args[3];
        *outFileNamePtr = args[4];

        if (*kPtr == 0 || *max_iterPtr == 0)
        {
            *shouldTerminate = 1;
            printf("%s", "Invalid input!");
            return;
        }
    }
    else if (numOfArgs == 4)
    {
        *kPtr = atoi(args[1]);
        *inFileNamePtr = args[2];
        *outFileNamePtr = args[3];
    }
    else
    {
        *shouldTerminate = 1;
        printf("%s", "Invalid input!");
        return;
    }
}

void assignPoints(double ***pointArrPtr, char **inFileNamePtr, int *numOfPointsArg, int *numOfCordsArg, int *shouldTerminate)
{
    char line[LINE_LENGTH];
    char *splittedLine;

    double cordinateVal;
    int numOfCords = 0;
    int numOfCordsIdx = 0;
    int numOfPoints = 0;
    int numOfPointsIdx = 0;

    FILE *filePtr;
    if (*shouldTerminate)
        return;
    computeNumOfCordsAndPoints(&filePtr, &numOfCords, &numOfPoints, &inFileNamePtr, &shouldTerminate);
    *numOfCordsArg = numOfCords;
    *numOfPointsArg = numOfPoints;
    *pointArrPtr = (double **)malloc((sizeof(double *)) * numOfPoints);
    if (*shouldTerminate)
        return;
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

void computeNumOfCordsAndPoints(FILE **filePtr, int *numOfCords, int *numOfPoints, char ***inFileNamePtr, int **shouldTerminate)
{
    char line[LINE_LENGTH];
    char *splittedLine;
    *filePtr = fopen(**inFileNamePtr, "r");
    if (*filePtr == NULL)
    {
        **shouldTerminate = 1;
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
        **shouldTerminate = 1;
        printf("%s", "An Error Has Occurred");
        return;
    }
}

void initializeCentroids(double ***pointArrPtr, double ***centroidsPtr, int K, int numOfCords, int *shouldTerminate)
{

    int pointIdx = 0;
    int cord = 0;
    if (*shouldTerminate)
        return;
    *centroidsPtr = (double **)malloc((sizeof(double *)) * K);
    for (pointIdx = 0; pointIdx < K; pointIdx++)
    {
        (*centroidsPtr)[pointIdx] = (double *)malloc(numOfCords * sizeof(double));
        for (cord = 0; cord < numOfCords; cord++)
            (*centroidsPtr)[pointIdx][cord] = (*pointArrPtr)[pointIdx][cord];
    }
}
void output(double ***centroidsPtr, int k, int numOfCords, char **outFileNamePtr, int *shouldTerminate)
{
    FILE *res;
    int i = 0;
    if (*shouldTerminate)
        return;
    res = fopen(*outFileNamePtr, "w");

    for (; i < k; i++)
    {
        int j = 0;
        for (; j < numOfCords - 1; j++)
        {
            fprintf(res, "%.4f", (*centroidsPtr)[i][j]);
            fprintf(res, "%s", ",");
        }
        fprintf(res, "%.4f", (*centroidsPtr)[i][j]);
        fprintf(res, "\n");
    }
    fclose(res);
}
void mainAlgorithm(double ***pointsArrPtr, double ***centroidsArrPtr, int max_iter, int numOfPoints, int numOfCords, int K, int *shouldTerminate)
{

    int iterCnt = 1;
    int isAllEuclidiansUnderEps = 0; /*boolean*/
    int pointIdx = 0;
    if (*shouldTerminate)
        return;
    while ((iterCnt != max_iter) && (!isAllEuclidiansUnderEps))
    {
        int pointIdxForInitCluster;
        int cordForCluster;
        double **clustersSumArrPtr;
        int *numOfPointsInCluster = (int *)malloc(K * sizeof(int));
        double *normDistances = (double *)malloc(K * sizeof(double));
        int idxForNormCalcs = 0;
        clustersSumArrPtr = (double **)malloc((sizeof(double *)) * K);

        if (numOfPointsInCluster == NULL || normDistances == NULL || clustersSumArrPtr == NULL)
        {
            *shouldTerminate = 1;
            printf("%s", "An Error Has Occurred");
            return;
        }
        for (pointIdxForInitCluster = 0; pointIdxForInitCluster < K; pointIdxForInitCluster++)
        {
            clustersSumArrPtr[pointIdxForInitCluster] = (double *)malloc(numOfCords * sizeof(double));
            if (clustersSumArrPtr[pointIdxForInitCluster] == NULL)
            {
                *shouldTerminate = 1;
                printf("%s", "An Error Has Occurred");
                return;
            }

            for (cordForCluster = 0; cordForCluster < numOfCords; cordForCluster++)
                clustersSumArrPtr[pointIdxForInitCluster][cordForCluster] = 0;
            numOfPointsInCluster[pointIdxForInitCluster] = 0;
            normDistances[pointIdxForInitCluster] = 0;
        }

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
                    tempDist += pow(point[cordIdx] - ((*centroidsArrPtr)[clusterIdx][cordIdx]), 2);

                if (tempDist < minDistance)
                {
                    chosenClusterIdx = clusterIdx;
                    minDistance = tempDist;
                }
            }
            for (cordIdx = 0; cordIdx < numOfCords; cordIdx++)
                clustersSumArrPtr[chosenClusterIdx][cordIdx] += point[cordIdx];

            numOfPointsInCluster[chosenClusterIdx] = numOfPointsInCluster[chosenClusterIdx] + 1;
        }

        for (idxForNormCalcs = 0; idxForNormCalcs < K; idxForNormCalcs++)
        {
            int idxForCent = 0;
            int cordIdxForNorm = 0;
            double distance = 0;
            double *prevCentroid = (double *)malloc(numOfCords * sizeof(double));
            double *newCentroid = (double *)malloc(numOfCords * sizeof(double));

            if (newCentroid == NULL || prevCentroid == NULL)
            {
                *shouldTerminate = 1;
                printf("%s", "An Error Has Occurred");
                return;
            }

            for (idxForCent = 0; idxForCent < numOfCords; idxForCent++)
                prevCentroid[idxForCent] = (*centroidsArrPtr)[idxForNormCalcs][idxForCent];
            if (!(numOfPointsInCluster[idxForNormCalcs])) /* prevents devision by 0 */
            {
                *shouldTerminate = 1;
                printf("%s", "An Error Has Occurred");
                return;
            }

            for (cordIdxForNorm = 0; cordIdxForNorm < numOfCords; cordIdxForNorm++)
                newCentroid[cordIdxForNorm] = clustersSumArrPtr[idxForNormCalcs][cordIdxForNorm] / numOfPointsInCluster[idxForNormCalcs];

            for (idxForCent = 0; idxForCent < numOfCords; idxForCent++)
                (*centroidsArrPtr)[idxForNormCalcs][idxForCent] = newCentroid[idxForCent];

            for (cordIdxForNorm = 0; cordIdxForNorm < numOfCords; cordIdxForNorm++)
                distance += pow(prevCentroid[cordIdxForNorm] - newCentroid[cordIdxForNorm], 2);

            normDistances[idxForNormCalcs] = pow(distance, 0.5);

            free(prevCentroid);
            free(newCentroid);
        }

        isAllEuclidiansUnderEps = 1;
        for (idxForNormCalcs = 0; idxForNormCalcs < K; idxForNormCalcs++)
        {
            if (normDistances[idxForNormCalcs] > EPSILON)
            {
                isAllEuclidiansUnderEps = 0;
                break;
            }
        }

        for (idxForNormCalcs = 0; idxForNormCalcs < K; idxForNormCalcs++)
            free(clustersSumArrPtr[idxForNormCalcs]);
        free(clustersSumArrPtr);
        free(numOfPointsInCluster);
        free(normDistances);

        iterCnt++;
    }
}