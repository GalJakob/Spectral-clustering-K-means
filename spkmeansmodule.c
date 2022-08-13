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
