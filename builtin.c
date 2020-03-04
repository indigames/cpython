#include "Python.h"

/// BEGIN - Internal modules
extern PyObject* PyInit__sqlite3(void);
extern PyObject* PyInit__ssl(void);
extern PyObject* PyInit__socket(void);
extern PyObject* PyInit_select(void);
extern PyObject* PyInit_unicodedata(void);
extern PyObject* PyInit__queue(void);
extern PyObject* PyInit__ctypes(void);
extern PyObject* PyInit_pyexpat(void);
extern PyObject* PyInit__asyncio(void);
extern PyObject* PyInit_zlib(void);
extern PyObject* PyInit__contextvars(void);
#if !defined(WIN32)
    extern PyObject* PyInit__posixsubprocess(void);
#endif
/// END - Internal Modules


// Custom modules, declared in igeLauncher project
extern struct _inittab g_customInitTabs[];

// Built-In modules
struct _inittab g_builtInInitTabs[] = {
    {"_sqlite3", PyInit__sqlite3},
    {"_ssl", PyInit__ssl},
    {"_socket", PyInit__socket},
    {"select", PyInit_select},
    {"unicodedata", PyInit_unicodedata},
    {"_queue", PyInit__queue},
    {"_ctypes", PyInit__ctypes},    
    {"pyexpat", PyInit_pyexpat},
    {"_asyncio", PyInit__asyncio},
    {"zlib", PyInit_zlib},
    {"_contextvars", PyInit__contextvars},
#if !defined(WIN32)
    {"_posixsubprocess", PyInit__posixsubprocess},
#endif
    {0, 0}
};

PyObject* createSpec(const char* name, const char* origin)
{
    PyObject* pArgs, * pValue, * pFunc, * pModule, * pGlobal, * pLocal;

    //pGlobal = PyDict_New();
    pGlobal = PyEval_GetGlobals();

    //Create a new module object
    pModule = PyModule_New("getspec");
    PyModule_AddStringConstant(pModule, "__file__", "");

    //Get the dictionary object from my module so I can pass this to PyRun_String
    pLocal = PyModule_GetDict(pModule);

    //Define my function in the newly created module
    pValue = PyRun_String(
        "def createModuleSpec(name, origin) :\n"\
        "    from importlib.machinery import ModuleSpec, BuiltinImporter\n"\
        "    return ModuleSpec(name = name, loader = BuiltinImporter, origin = origin)",
        Py_file_input, pGlobal, pLocal);

    //pValue would be null if the Python syntax is wrong, for example
    if (pValue == NULL) {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        return NULL;
    }

    //pValue is the result of the executing code,
    //chuck it away because we've only declared a function
    Py_DECREF(pValue);

    //Get a pointer to the function I just defined
    pFunc = PyObject_GetAttrString(pModule, "createModuleSpec");

    //Double check we have actually found it and it is callable
    if (!pFunc || !PyCallable_Check(pFunc)) {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        fprintf(stderr, "Cannot find function \"createModuleSpec\"\n");
        return NULL;
    }

    //Build a tuple to hold my arguments (just the number 4 in this case)
    pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, _PyUnicode_FromASCII(name, strlen(name)));
    PyTuple_SetItem(pArgs, 1, _PyUnicode_FromASCII(origin, strlen(origin)));

    //Call my function, passing it the number four
    pValue = PyObject_CallObject(pFunc, pArgs);
    if (PyErr_Occurred()) {
        PyErr_Print();
    }

    //Py_DECREF(pValue);
    Py_XDECREF(pFunc);

    return pValue;
}

PyObject* FindModule(PyObject* abs_name, void* inittab) {
    if(!inittab) return 0;

    PyObject* spec;
    PyModuleDef* def;

    const char* name = PyUnicode_AsUTF8(abs_name);
    PyErr_Clear();
    struct _inittab* tab = inittab;
    while (tab->name) {
        if (strcmp(tab->name, name) == 0) {
            //PyImport_AddModule(name);
            PyObject* pyModule = (*tab->initfunc)();

            if (PyObject_TypeCheck(pyModule, &PyModuleDef_Type)) {
                spec = createSpec(name, "built-in");
                def = (PyModuleDef*)pyModule;
                pyModule = PyModule_FromDefAndSpec(def, spec);
                PyObject* sys_modules = PyImport_GetModuleDict();
                PyDict_SetItemString(sys_modules, name, pyModule);
                PyModule_ExecDef(pyModule, def);
                //Py_DECREF(pyModule);
                //assert(!PyErr_Occurred());
            }
            else {
                PyObject* sys_modules = PyImport_GetModuleDict();
                _PyImport_FixupExtensionObject(pyModule, abs_name, abs_name, sys_modules);

                //PyDict_SetItemString(sys_modules, name, pyModule);
                //Py_DECREF(pyModule);
            }
            return pyModule;
        }
        tab++;
    }
    return 0;
}

PyObject* FindBuildtinModule(PyObject* abs_name) {
    _Py_IDENTIFIER(__spec__);
    
    // Find built-in modules
    PyObject* pyModule = FindModule(abs_name, (struct _inittab*)g_builtInInitTabs);
    if(pyModule != 0)
        return pyModule;

    // Find custome modules
    pyModule = FindModule(abs_name, (struct _inittab*)g_customInitTabs);
    if(pyModule != 0)
        return pyModule;

    return 0;
}
