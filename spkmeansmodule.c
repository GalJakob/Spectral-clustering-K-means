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
PyObject *KmeansAlgorithm(double ***pointsArrPtr, double ***centroidsArrPtr, int numOfPoints, int numOfCords, int K);
PyObject *convertDPToPyObj(double ***centroidsArrPtr, int K, int numOfCords);
double **convertPyObjToDP(PyObject *pyPointsArrPtr, int numOfPoints, int numOfCords);



static PyObject *createPyMat(double **matC, int size, int cords)
{
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
    int i, j;
    PyObject *V;
    PyObject *cord;
    double **matC = (double **)calloc(size, sizeof(double *));
    assert(matC != NULL);
    for (i = 0; i < size; i++)
    {
        matC[i] = (double *)calloc(cords, sizeof(double));
        assert(matC[i] != NULL);
        V = PyList_GetItem(matP, (Py_ssize_t)i);
        for (j = 0; j < cords; j++)
        {
            cord = PyList_GetItem(V, (Py_ssize_t)j);
            matC[i][j] = PyFloat_AsDouble(cord);
        }
    }
    return matC;
}

static PyObject *renomlizedMatToPy(PyObject *self, PyObject *args)
{

    int k, size, cords, *d;
    double **vec;
    double **mat;
    char *file;
    PyObject *matP;
    FILE *test;
    test = fopen("log", "w");
    fprintf(test, "%s", "1");
    fclose(test);

    /* if (!PyArg_ParseTuple(args, "si", &filename, &k)){
        return NULL;
    } */
    d; // need to calculate
    size = d[0];
    cords = d[1];

    vec; // need to read from file
         /*   mat = finalMat // needs to be here but can't assign
           matP = createPyMat(mat, size, k);
        */
    return matP;
}


static PyObject *execByGoalFromPy(PyObject *self, PyObject *args)
{
    /*executes the spectral clustering algorithm. called from python*/
    int k;
    char *goal;
    char *fileName;
    double **nKMatForKmeansPP;
    int numOfPoints;

    if (!PyArg_ParseTuple(args, "iss", &k, &goal, &fileName))
        return NULL;
    numOfPoints = getNumOfPoints(fileName);
    nKMatForKmeansPP = execByGoal(k, goal, fileName);
    if (nKMatForKmeansPP == NULL)
        Py_RETURN_NONE;
    else
        return createPyMat(nKMatForKmeansPP, numOfPoints, k); /*  k rows and numofpoints cols */
}

double **convertPyObjToDP(PyObject *pyPointsArrPtr, int numOfPoints, int numOfCords)
{
    double **pointsArr;
    PyObject *point;

    int numOfCordsIdx = 0;
    int numOfPointsIdx = 0;
    pointsArr = (double **)malloc((sizeof(double *)) * numOfPoints);
    if (pointsArr == NULL)
    {
        printf("%s", "An Error Has Occurred ");
    }

    for (numOfPointsIdx = 0; numOfPointsIdx < numOfPoints; numOfPointsIdx++)
    {
        (pointsArr)[numOfPointsIdx] = (double *)malloc(numOfCords * sizeof(double));
        if ((pointsArr)[numOfPointsIdx] == NULL)
        {
            printf("%s", "An Error Has Occurred ");
        }
        point = PyList_GetItem(pyPointsArrPtr, numOfPointsIdx);
        for (numOfCordsIdx = 0; numOfCordsIdx < numOfCords; numOfCordsIdx++)
        {
            (pointsArr)[numOfPointsIdx][numOfCordsIdx] = PyFloat_AsDouble(PyList_GetItem(point, numOfCordsIdx));
        }
    }

    return pointsArr;
}

static PyObject *fit(PyObject *self, PyObject *args)
{
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
    pointsArr = convertPyObjToDP(pyPointsArrPtr, numOfPoints, numOfCords);
    centroidsArr = convertPyObjToDP(pyCentroidsArrPtr, K, numOfCords);
    returnedCentroids = Py_BuildValue("O", KmeansAlgorithm(&pointsArr, &centroidsArr, numOfPoints, numOfCords, K));
    /* printCentroids(clusters, numOfFeatures); */
    return returnedCentroids;
    /* Py_RETURN_NONE; */
}
PyObject *convertDPToPyObj(double ***centroidsArrPtr, int K, int numOfCords)
{
    PyObject *pythonCent;
    PyObject *pythonCord;
    PyObject *pythonCentroids;
    int cordIdx;
    int centIdx;

    pythonCentroids = PyList_New(K);
    for (centIdx = 0; centIdx < K; centIdx++)
    {
        pythonCent = PyList_New(numOfCords);
        for (cordIdx = 0; cordIdx < numOfCords; cordIdx++)
        {
            pythonCord = Py_BuildValue("d", (*centroidsArrPtr)[centIdx][cordIdx]);
            PyList_SetItem(pythonCent, cordIdx, pythonCord);
        }
        PyList_SetItem(pythonCentroids, centIdx, pythonCent);
    }
    return pythonCentroids;
}

PyObject *KmeansAlgorithm(double ***pointsArrPtr, double ***centroidsArrPtr, int numOfPoints, int numOfCords, int K)
{
    int iterCnt = 1;
    int pointIdx = 0;

    while (iterCnt != DEFAULT_MAX_ITER)
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
            printf("%s", "An Error Has Occurred");
            exit(0);
        }
        for (pointIdxForInitCluster = 0; pointIdxForInitCluster < K; pointIdxForInitCluster++)
        {
            clustersSumArrPtr[pointIdxForInitCluster] = (double *)malloc(numOfCords * sizeof(double));
            if (clustersSumArrPtr[pointIdxForInitCluster] == NULL)
            {
                printf("%s", "An Error Has Occurred");
                exit(0);
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
                printf("%s", "An Error Has Occurred ");
                exit(0);
            }

            for (idxForCent = 0; idxForCent < numOfCords; idxForCent++)
                prevCentroid[idxForCent] = (*centroidsArrPtr)[idxForNormCalcs][idxForCent];
            if (!(numOfPointsInCluster[idxForNormCalcs])) /* prevents devision by 0 */
            {
                printf("%s ", "An Error Has Occurred ");
                exit(0);
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
        for (idxForNormCalcs = 0; idxForNormCalcs < K; idxForNormCalcs++)
            free(clustersSumArrPtr[idxForNormCalcs]);
        free(clustersSumArrPtr);
        free(numOfPointsInCluster);
        free(normDistances);

        iterCnt++;
    }
    return convertDPToPyObj(&*centroidsArrPtr, K, numOfCords);
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

