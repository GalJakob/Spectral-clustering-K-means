#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define LINE_LENGTH 256
#define DEFAULT_MAX_ITER 200

static PyObject *fit(PyObject *self, PyObject *args);
PyObject *mainAlgorithm2(double ***, double ***, int, int, int, int, float);
PyObject *convertDPToPyObj(double ***, int, int);
double **convertPyObjToDP(PyObject *, int, int);

PyObject *mainAlgorithm2(double ***pointsArrPtr, double ***centroidsArrPtr, int max_iter, int numOfPoints, int numOfCords, int K, float eps)
{
    int iterCnt = 1;
    int isAllEuclidiansUnderEps = 0; /*boolean*/
    int pointIdx = 0;

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

        isAllEuclidiansUnderEps = 1;
        for (idxForNormCalcs = 0; idxForNormCalcs < K; idxForNormCalcs++)
        {
            if (normDistances[idxForNormCalcs] > eps)
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
    return convertDPToPyObj(&*centroidsArrPtr, K, numOfCords);
}

/* python */
static PyObject *fit(PyObject *self, PyObject *args)
{
    PyObject *pyPointsArrPtr;
    PyObject *pyCentroidsArrPtr;
    PyObject *returnedCentroids;

    double **pointsArr;
    double **centroidsArr;
    int max_iter;
    int K;
    int numOfPoints;
    int numOfCords;
    float eps;

    if (!PyArg_ParseTuple(args, "OOiiiif", &pyPointsArrPtr, &pyCentroidsArrPtr, &K, &max_iter, &numOfPoints, &numOfCords, &eps))
        return NULL;
    pointsArr = convertPyObjToDP(pyPointsArrPtr, numOfPoints, numOfCords);
    centroidsArr = convertPyObjToDP(pyCentroidsArrPtr, K, numOfCords);
    returnedCentroids = Py_BuildValue("O", mainAlgorithm2(&pointsArr, &centroidsArr, max_iter, numOfPoints, numOfCords, K, eps));
    return returnedCentroids;
}

static PyMethodDef myMethods[] = {
    {"fit",
     (PyCFunction)fit,
     METH_VARARGS,
     PyDoc_STR("kmeans implement")},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef kmeansMod =
    {
        PyModuleDef_HEAD_INIT,
        "mykmeanssp",
        "",
        -1,
        myMethods};

PyMODINIT_FUNC PyInit_mykmeanssp()
{
    return PyModule_Create(&kmeansMod);
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
