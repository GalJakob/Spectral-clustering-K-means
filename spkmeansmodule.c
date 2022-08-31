#define PY_SSIZE_T_CLEAN
#define SIZEOF(arr) sizeof(arr) / sizeof(double)

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "spkmeans.h"
#include <Python.h>

static PyObject *execByGoalFromPy(PyObject *self, PyObject *args);

/* ########## gal ########## */

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

static PyObject *execKmeans(PyObject *self, PyObject *args)
{

    int i, size, cords;
    double **mat;
    double **startPoints;
    double **kmeansRes;
    PyObject *matP;
    PyObject *startPointsPy;
    PyObject *finalPointsPy;

    if (!PyArg_ParseTuple(args, "OOiii", &matP, &startPointsPy, &i, &cords, &size))
    {
        return NULL;
    }

    startPoints = createCMat(startPointsPy, size, cords);
    mat = createCMat(matP, size, i);
    kmeansRes = kmeans(mat, startPoints, i, size);
    finalPointsPy = createPyMat(kmeansRes, i, cords);

    return finalPointsPy;
}

static PyObject *execGoal(PyObject *self, PyObject *args)
{
    int size, cords;
    char *goal, *file;
    int *d;
    double **goalMat;
    double **vecs;
    PyObject *goalMatPy;

    if (!PyArg_ParseTuple(args, "ss", &goal, &file))
    {
        return NULL;
    }

    /*  d; // need to calculate
     vAmount = d[0];
     vLength = d[1];
  */
    vecs =       // need to read from file
        goalMat; // goal mat needs to be here
    if (!strcmp(goal, "jacobi"))
    {
        goalMatPy = createPyMat(goalMat, size + 1, size);
    }
    else
    {
        goalMatPy = createPyMat(goalMat, size, size);
    }
    return goalMatPy;
}

/* ########## noa ########## */

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
    {
        printf("%d\n", numOfPoints);
        /* return createPyMat(nKMatForKmeansPP); k rows and numofpoints cols */
    }
}

// needs to complete this
static PyMethodDef _methods[] = {

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
