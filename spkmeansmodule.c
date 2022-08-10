#define PY_SSIZE_T_CLEAN

#include "spkmeans.h"
#include <Python.h>

static PyObject *execByGoal(PyObject *self, PyObject *args);

static PyObject *execSpkAlg(PyObject *self, PyObject *args);


static PyObject *execSpkAlg(PyObject *self, PyObject *args) {
  /*executes the spectral clustering algorithm*/
    
}



static PyMethodDef _methods[] = {
        {"execSpkAlg",
                (PyCFunction) execSpkAlg,
                      METH_VARARGS,
                         PyDoc_STR("main func of the algorithm")},
                                   
        {"execByGoal",
                (PyCFunction) execByGoal,
                      METH_VARARGS,
                         PyDoc_STR(
                                 "this func is executed by given goal")},
        {NULL,  NULL, 0, NULL}
};

static struct PyModuleDef _moduledef = {
        PyModuleDef_HEAD_INIT,
        "spkmeansmodule",
        NULL,
        -1,
        _methods
};

PyMODINIT_FUNC
PyInit_spkmeansmodule(void) {
    PyObject *m;
    m = PyModule_Create(&_moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}
