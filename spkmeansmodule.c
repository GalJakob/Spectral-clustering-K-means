#define PY_SSIZE_T_CLEAN

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

static PyObject* createPythonMat(double **matC, int size, int cords){
    int i,j;
    PyObject *temp;
    PyObject *matP;
    matP = PyList_New(size);

    for (i = 0; i < size; i++){
        temp = PyList_New(cords);

        for (j = 0; j < cords; j++){
            PyList_SetItem(temp, j, PyFloat_FromDouble(matC[i][j]));
        }
        PyList_SetItem(matP, i, temp);
    }
    return matP;
}

static double** createCMat(PyObject *matP, int size, int cords){
    int i, j;
    PyObject *V;
    PyObject *cord;
    double ** matC = (double **)calloc(size, sizeof(double *));
    assert(matC != NULL);
    for (i = 0; i < size; i++){
        matC[i] = (double *)calloc(cords, sizeof(double));
        assert(matC[i] != NULL);
        V = PyList_GetItem(matP, (Py_ssize_t)i);
        for (j = 0; j < cords; j++){
            cord = PyList_GetItem(V, (Py_ssize_t)j);
            matC[i][j] = PyFloat_AsDouble(cord);
        }
    }
    return matC;
}



// don't know if we need this. plz check :)
static PyObject *execByGoalFromPy(PyObject *self, PyObject *args)
{
    
        /*executes the spectral clustering algorithm. called from python*/
        int k;
        char *goal;
        char *fileName;

        if (!PyArg_ParseTuple(args, "iss", &k, &goal, &fileName))
                return NULL;
        execByGoal(k, goal, fileName);
        Py_RETURN_NONE;
}

/* ########## noa ########## */

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
