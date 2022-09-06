#define PY_SSIZE_T_CLEAN

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <Python.h>
#include "spkmeans.h"

#define DEFAULT_MAX_ITER 300
#define DEFAULT_EPS 0

static PyObject *execByGoalFromPy(PyObject *self, PyObject *args);
static PyObject *fit(PyObject *self, PyObject *args);
static PyObject *createPyMat(double **matC, int size, int cords);
static PyObject *KmeansAlgorithm(double ***pointsArrPtr, double ***centroidsArrPtr, int numOfPoints, int numOfCords, int K);
static double **createCMat(PyObject *matP, int size, int cords);

static PyObject *createPyMat(double **matC, int size, int cords)
{
    /* gets matrix from C and converts it to python object matrix */
    int i, j;
    PyObject *temp;
    PyObject *matP;
    matP = PyList_New(size);

    for (i = 0; i < size; i++)
    {
        temp = PyList_New(cords);

        for (j = 0; j < cords; j++)
        {
            PyList_SetItem(temp, j, PyFloat_FromDouble(matC[i][j]));
        }
        PyList_SetItem(matP, i, temp);
    }
    return matP;
}

static double **createCMat(PyObject *matP, int size, int cords)
{
    /* gets python object that is matrix and returns the matrix in C */
    int i, j;
    PyObject *V;
    PyObject *cord;

    double **matC = (double **)calloc(size, sizeof(double *));
    customAssert(matC != NULL);

    for (i = 0; i < size; i++)
    {
        matC[i] = (double *)calloc(cords, sizeof(double));
        customAssert(matC[i] != NULL);
        V = PyList_GetItem(matP, (Py_ssize_t)i);
        for (j = 0; j < cords; j++)
        {
            cord = PyList_GetItem(V, (Py_ssize_t)j);
            matC[i][j] = PyFloat_AsDouble(cord);
        }
    }

    return matC;
}

static PyObject *execByGoalFromPy(PyObject *self, PyObject *args)
{
    /*executes by goal from cmd. called from python*/
    int k;
    char *goal;
    char *fileName;
    double **nKMatForKmeansPP;
    int numOfPoints;

    if (!PyArg_ParseTuple(args, "iss", &k, &goal, &fileName))
        return NULL;

    numOfPoints = getNumOfPoints(fileName);
    nKMatForKmeansPP = execByGoal(&k, goal, fileName);
   /*  printMat(nKMatForKmeansPP,numOfPoints,k); */
    if (nKMatForKmeansPP == NULL)
        Py_RETURN_NONE;
    else
        return createPyMat(nKMatForKmeansPP, numOfPoints, k); /*  k rows and numofpoints cols */
}

static PyObject *fit(PyObject *self, PyObject *args)
{
    /* like in 2nd assignment, this function calls kmeans algorithm with given centroids */
    PyObject *pyPointsArrPtr;
    PyObject *pyCentroidsArrPtr;
    PyObject *returnedCentroids;

    double **pointsArr;
    double **centroidsArr;
    int K;
    int numOfPoints;
    int numOfCords;

    if (!PyArg_ParseTuple(args, "OOiii", &pyPointsArrPtr, &pyCentroidsArrPtr, &K, &numOfPoints, &numOfCords))
        return NULL;

    pointsArr = createCMat(pyPointsArrPtr, numOfPoints, numOfCords);
    centroidsArr = createCMat(pyCentroidsArrPtr, K, numOfCords);
    returnedCentroids = Py_BuildValue("O", KmeansAlgorithm(&pointsArr, &centroidsArr, numOfPoints, numOfCords, K));
    /* printCentroids(clusters, numOfFeatures); */
    return returnedCentroids;
    /* Py_RETURN_NONE; */
}

static PyObject *KmeansAlgorithm(double ***pointsArrPtr, double ***centroidsArrPtr, int numOfPoints, int numOfCords, int K)
{
    int iterCnt;
    int pointIdx;

    iterCnt = 1;
    pointIdx = 0;

    while (iterCnt < DEFAULT_MAX_ITER)
    {
        int pointIdxForInitCluster;
        int cordForCluster;
        int idxForNormCalcs;
        int *numOfPointsInCluster;
        double *normDistances;
        double **clustersSumArrPtr;

        numOfPointsInCluster = (int *)malloc(K * sizeof(int));
        customAssert(numOfPointsInCluster != NULL);

        normDistances = (double *)malloc(K * sizeof(double));
        customAssert(normDistances != NULL);

        clustersSumArrPtr = (double **)malloc((sizeof(double *)) * K);
        customAssert(clustersSumArrPtr != NULL);

        idxForNormCalcs = 0;

        for (pointIdxForInitCluster = 0; pointIdxForInitCluster < K; pointIdxForInitCluster++)
        {
            clustersSumArrPtr[pointIdxForInitCluster] = (double *)malloc(numOfCords * sizeof(double));
            customAssert(clustersSumArrPtr[pointIdxForInitCluster] != NULL);

            for (cordForCluster = 0; cordForCluster < numOfCords; cordForCluster++)
                clustersSumArrPtr[pointIdxForInitCluster][cordForCluster] = 0;

            numOfPointsInCluster[pointIdxForInitCluster] = 0;
            normDistances[pointIdxForInitCluster] = 0;
        }

        for (pointIdx = 0; pointIdx < numOfPoints; pointIdx++)
        {
            double *point = (*pointsArrPtr)[pointIdx];
            double minDistance = DBL_MAX;
            int clusterIdx;
            int chosenClusterIdx = 0;
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
            double *prevCentroid;
            double *newCentroid;
            int idxForCent = 0;
            int cordIdxForNorm = 0;
            double distance = 0;

            prevCentroid = (double *)malloc(numOfCords * sizeof(double));
            customAssert(prevCentroid!=NULL);

            newCentroid = (double *)malloc(numOfCords * sizeof(double));
            customAssert(newCentroid!=NULL);

            for (idxForCent = 0; idxForCent < numOfCords; idxForCent++)
                prevCentroid[idxForCent] = (*centroidsArrPtr)[idxForNormCalcs][idxForCent];

            if (!(numOfPointsInCluster[idxForNormCalcs])) /* prevents devision by 0 */
                customAssert(0);

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

        customFreeForMat(clustersSumArrPtr,K);
        free(numOfPointsInCluster);
        free(normDistances);
        iterCnt++;
    }
    printMat(*centroidsArrPtr,K,numOfCords);
    return createPyMat(*centroidsArrPtr, K, numOfCords);
}

static PyMethodDef _methods[] = {
    {"fit",
     (PyCFunction)fit,
     METH_VARARGS,
     PyDoc_STR("kmeans implement")},
    {"execByGoalFromPy",
     (PyCFunction)execByGoalFromPy,
     METH_VARARGS,
     PyDoc_STR(
         "this func is executed by given goal")},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef _moduledef = {
    PyModuleDef_HEAD_INIT,
    "spkmeansmodule",
    NULL,
    -1,
    _methods};

PyMODINIT_FUNC
PyInit_spkmeansmodule(void)
{
    PyObject *m;
    m = PyModule_Create(&_moduledef);
    if (!m)
    {
        return NULL;
    }
    return m;
}
