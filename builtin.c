#include "Python.h"

/// BEGIN - External Modules
#ifdef USE_IGE
extern PyObject* _PyInit__igeCore(void);
extern PyObject* PyInit__igeTools(void);
extern PyObject* PyInit_igeVmath(void);

extern PyObject* PyInit_igeBullet();

extern PyObject* PyInit_igefirebase(void);

extern PyObject* PyInit_imguicore(void);
extern PyObject* PyInit__Box2D(void);

extern PyObject* PyInit_cv2(void);
extern PyObject* PyInit_dlib();
extern PyObject* PyInit_igeWebview(void);
extern PyObject* PyInit_igeAds(void);
extern PyObject* PyInit_igeSocial(void);
extern PyObject* PyInit_igeGamesServices(void);
extern PyObject* PyInit_igeNanoGUI(void);
extern PyObject* PyInit_igeAppsFlyer(void);
extern PyObject* PyInit_igeNotify(void);
extern PyObject* PyInit_igeSound(void);
#endif
/// END - External Modules


/// BEGIN - Internal modules
extern PyObject* PyInit_pyexpat(void);
extern PyObject* PyInit__sqlite3(void);
extern PyObject* PyInit__ssl(void);
extern PyObject* PyInit__socket(void);
extern PyObject* PyInit_select(void);
extern PyObject* PyInit_unicodedata(void);
extern PyObject* PyInit__queue(void);
extern PyObject* PyInit__ctypes(void);
extern PyObject* PyInit__multiarray_umath(void);
extern PyObject* PyInit__multiarray_tests(void);
extern PyObject* PyInit__rational_tests(void);
extern PyObject* PyInit__operand_flag_tests(void);
extern PyObject* PyInit__struct_ufunc_tests(void);
extern PyObject* PyInit__umath_tests(void);
extern PyObject* PyInit_lapack_lite(void);
extern PyObject* PyInit__umath_linalg(void);
extern PyObject* PyInit_pocketfft_internal(void);
extern PyObject* PyInit_bit_generator(void);
extern PyObject* PyInit_bounded_integers(void);
extern PyObject* PyInit_common(void);
extern PyObject* PyInit_entropy(void);
extern PyObject* PyInit_generator(void);
extern PyObject* PyInit_mt19937(void);
extern PyObject* PyInit_mtrand(void);
extern PyObject* PyInit_pcg64(void);
extern PyObject* PyInit_philox(void);
extern PyObject* PyInit_sfc64(void);
extern PyObject* PyInit__posixsubprocess(void);
extern PyObject* PyInit__contextvars(void);
/// END - Internal Modules 


struct _inittab Custom_Inittab[] = {
#ifdef USE_IGE
	{"igeCore._igeCore", _PyInit__igeCore},
	{"igeCore.devtool._igeTools", PyInit__igeTools},
	{"igeVmath",PyInit_igeVmath},
    
    {"imguicore", PyInit_imguicore},
	{"_Box2D", PyInit__Box2D},
    
    {"igeOpenAL",PyInit_igeOpenAL},
    {"igefirebase",PyInit_igefirebase},
    
    {"cv2.cv2", PyInit_cv2},
	{"dlib", PyInit_dlib},
	{"igeBullet", PyInit_igeBullet},
    {"igeWebview", PyInit_igeWebview},
    {"igeAds", PyInit_igeAds},
    {"igeSocial", PyInit_igeSocial},
    {"igeGamesServices", PyInit_igeGamesServices},    
    {"igeNanoGUI", PyInit_igeNanoGUI},
    {"igeAppsFlyer", PyInit_igeAppsFlyer},
    {"igeNotify", PyInit_igeNotify},
	{"igeSound", PyInit_igeSound},
#endif

	{"_sqlite3", PyInit__sqlite3},
	{"_ssl", PyInit__ssl},
	{"_socket", PyInit__socket},
	{"select", PyInit_select},
	{"unicodedata", PyInit_unicodedata},
	{"_queue", PyInit__queue},
	{"_ctypes", PyInit__ctypes},
#if !defined(WIN32)
	{"_posixsubprocess", PyInit__posixsubprocess},
#endif
	{"_contextvars", PyInit__contextvars},
	{"pyexpat", PyInit_pyexpat},
	{"numpy.core._multiarray_umath", PyInit__multiarray_umath},
	{"numpy.core._multiarray_tests", PyInit__multiarray_tests},
	{"numpy.core._rational_tests", PyInit__rational_tests},
	{"numpy.core._operand_flag_tests", PyInit__operand_flag_tests},
	{"numpy.core._struct_ufunc_tests", PyInit__struct_ufunc_tests},
	{"numpy.core._umath_tests", PyInit__umath_tests},
	{"numpy.linalg.lapack_lite", PyInit_lapack_lite},
	{"numpy.linalg._umath_linalg", PyInit__umath_linalg},
	{"numpy.fft.pocketfft_internal", PyInit_pocketfft_internal},
	{"numpy.random.bit_generator", PyInit_bit_generator},
	{"numpy.random.bounded_integers", PyInit_bounded_integers},
	{"numpy.random.common", PyInit_common},
	{"numpy.random.entropy", PyInit_entropy},
	{"numpy.random.generator", PyInit_generator},
	{"numpy.random.mt19937", PyInit_mt19937},
	{"numpy.random.mtrand", PyInit_mtrand},
	{"numpy.random.pcg64", PyInit_pcg64},
	{"numpy.random.philox", PyInit_philox},
	{"numpy.random.sfc64", PyInit_sfc64},	
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

PyObject* FindBuildtinModule(PyObject*  abs_name){
	_Py_IDENTIFIER(__spec__);
	PyObject* spec;
	PyObject* globals;
	PyModuleDef* def;

	const char* name = PyUnicode_AsUTF8(abs_name);
	PyErr_Clear();
	struct _inittab* tab = Custom_Inittab;
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

