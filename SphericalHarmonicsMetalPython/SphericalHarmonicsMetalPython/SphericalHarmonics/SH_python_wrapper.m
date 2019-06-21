//
//  SH_python_wrapper.m
//  SphericalHarmonics
//
//  Created by asd on 20/06/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

// important project configuration issues:

// 1. code sign identity : local
// 2. manually define python include & library search paths
// 3. in bundle contents copy MacOS/SphericalHarmonicsMetalPython to SphericalHarmonics.so in Python folder
// 4. copy metal compiler file: Resources/default.metallib to Python folder


#import "SH_python_wrapper.h"
#import "SphericalHarmonics.h"
#include <Python.h>

@implementation SH_python_wrapper
// c funcs
static SphericalHarmonics*sh=NULL;

static void c_sphericalHarmonics(int res, int colorMap, int code) {
    sh=[SphericalHarmonics init: res colorMap:colorMap code:code];
    [sh genCoords];
}

static void c_read_code(int code) {
    [sh readCode:code];
    [sh genCoords];
}

static int c_n_codes() {
    return N_SH_CODES;
}

static char*c_get_code() {
    return sh->code;
}


static PyObject*gen_python_mesh() { // create list of coords, normals, colors, textures
    PyObject *coord_list   = PyList_New(sh->nCoords);
    PyObject *norm_list    = PyList_New(sh->nCoords);
    PyObject *color_list   = PyList_New(sh->nCoords);
    PyObject *texture_list = PyList_New(sh->nCoords);
    
    for(unsigned i = 0; i < sh->nCoords; i++) { // fill tham
        XYZ c=sh->coords[i];
        PyList_SET_ITEM(coord_list, i, Py_BuildValue("fff", c.x, c.y, c.z));
        
        c=sh->normals[i];
        PyList_SET_ITEM(norm_list, i, Py_BuildValue("fff", c.x, c.y, c.z));
        
        c=sh->colors[i];
        PyList_SET_ITEM(color_list, i, Py_BuildValue("fff", c.x, c.y, c.z));
        
        Texture t=sh->textures[i];
        PyList_SET_ITEM(texture_list, i, Py_BuildValue("ff", t.x, t.y));
    }
    
    PyObject *mesh_list = PyList_New(4); // [coords, normal, color, textures]
    PyList_SetItem(mesh_list, 0, coord_list);
    PyList_SetItem(mesh_list, 1, norm_list);
    PyList_SetItem(mesh_list, 2, color_list);
    PyList_SetItem(mesh_list, 3, texture_list);
    
    return mesh_list;
}

// c <-> python wrappers
static PyObject* sphericalHarmonics(PyObject* self,PyObject* args) {
    int res, colorMap, code;
    if (!PyArg_ParseTuple(args,"iii",&res, &colorMap, &code)) return NULL;
    
    c_sphericalHarmonics(res, colorMap, code);
    
    return gen_python_mesh();
}

static PyObject* read_code(PyObject* self,PyObject* args) {
    int code;
    if (!PyArg_ParseTuple(args,"i",&code)) return NULL;
    c_read_code(code);
    
    return gen_python_mesh();
}

static PyObject* n_codes(PyObject* self,PyObject* args) {
    return Py_BuildValue("i",c_n_codes());
}

static PyObject* get_code(PyObject* self,PyObject* args) {
    return Py_BuildValue("s",c_get_code());
}




// method defs
static PyMethodDef module_methods[] = {
    {"SphericalHarmonics",(PyCFunction)sphericalHarmonics, METH_VARARGS, "SphericalHarmonics(resol, colorMap)"},
    {"read_code",         (PyCFunction)read_code, METH_VARARGS, "generate SH based on code"},
    {"n_codes",           (PyCFunction)n_codes, METH_VARARGS, "number of SH codes"},
    {"get_code",          (PyCFunction)get_code, METH_VARARGS, "current code string"},

    {NULL}
};

// module def.
static struct PyModuleDef Fib = {
    PyModuleDef_HEAD_INIT,
    "SphericalHarmonics", // name of module
    NULL, // module documentation, may be NULL
    -1,   // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    module_methods
};

// module init
PyMODINIT_FUNC PyInit_SphericalHarmonics(void) {
    return PyModule_Create(&Fib);
}
@end

