/* Generated automatically from ./Modules/config.c.in by makesetup. */
/* -*- C -*- ***********************************************
Copyright (c) 2000, BeOpen.com.
Copyright (c) 1995-2000, Corporation for National Research Initiatives.
Copyright (c) 1990-1995, Stichting Mathematisch Centrum.
All rights reserved.

See the file "Misc/COPYRIGHT" for information on usage and
redistribution of this file, and for a DISCLAIMER OF ALL WARRANTIES.
******************************************************************/

/* Module configuration */

/* !!! !!! !!! This file is edited by the makesetup script !!! !!! !!! */

/* This file contains the table of built-in modules.
   See create_builtin() in import.c. */

#include "Python.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern PyObject* PyInit_posix(void);
	extern PyObject* PyInit_errno(void);
	extern PyObject* PyInit_pwd(void);
	extern PyObject* PyInit__sre(void);
	extern PyObject* PyInit__codecs(void);
	extern PyObject* PyInit__weakref(void);
	extern PyObject* PyInit__functools(void);
	extern PyObject* PyInit__operator(void);
	extern PyObject* PyInit__collections(void);
	extern PyObject* PyInit__abc(void);
	extern PyObject* PyInit_itertools(void);
	extern PyObject* PyInit_atexit(void);
	extern PyObject* PyInit__signal(void);
	extern PyObject* PyInit__stat(void);
	extern PyObject* PyInit_time(void);
	extern PyObject* PyInit__thread(void);
	extern PyObject* PyInit__locale(void);
	extern PyObject* PyInit__io(void);
	extern PyObject* PyInit_zipimport(void);
	extern PyObject* PyInit_faulthandler(void);
	extern PyObject* PyInit__tracemalloc(void);
	extern PyObject* PyInit__symtable(void);
	extern PyObject* PyInit_xxsubtype(void);
	/* -- ADDMODULE MARKER 1 -- */

	extern PyObject* PyMarshal_Init(void);
	extern PyObject* PyInit__imp(void);
	extern PyObject* PyInit_gc(void);
	extern PyObject* PyInit__ast(void);
	extern PyObject* _PyWarnings_Init(void);
	extern PyObject* PyInit__string(void);

	extern PyObject* PyInit_array(void);
	extern PyObject* PyInit_audioop(void);
	extern PyObject* PyInit_binascii(void);
	extern PyObject* PyInit_cmath(void);
	extern PyObject* PyInit_math(void);
	extern PyObject* PyInit__md5(void);
	extern PyObject* PyInit__sha1(void);
	extern PyObject* PyInit__sha256(void);
	extern PyObject* PyInit__sha512(void);
	extern PyObject* PyInit__sha3(void);
	extern PyObject* PyInit__blake2(void);

#ifdef WIN32
	extern PyObject* PyInit_msvcrt(void);
	extern PyObject* PyInit__locale(void);
	extern PyObject* PyInit_nt(void);
	extern PyObject* PyInit__winapi(void);
	extern PyObject* PyInit_winreg(void);
#endif

	/* XXX: This one should really be extracted to standalone extension. */
	extern PyObject* PyInit__random(void);
	extern PyObject* PyInit__heapq(void);
	extern PyObject* PyInit__bisect(void);
	extern PyObject* PyInit_mmap(void);
	extern PyObject* PyInit__csv(void);
	extern PyObject* PyInit_parser(void);
	extern PyObject* PyInit__struct(void);
	extern PyObject* PyInit__datetime(void);
	extern PyObject* PyInit__json(void);
#ifdef _Py_HAVE_ZLIB
	extern PyObject* PyInit_zlib(void);
#endif

	extern PyObject* PyInit__multibytecodec(void);
	extern PyObject* PyInit__codecs_cn(void);
	extern PyObject* PyInit__codecs_hk(void);
	extern PyObject* PyInit__codecs_iso2022(void);
	extern PyObject* PyInit__codecs_jp(void);
	extern PyObject* PyInit__codecs_kr(void);
	extern PyObject* PyInit__codecs_tw(void);
	extern PyObject* PyInit__lsprof(void);
	extern PyObject* PyInit__pickle(void);
	extern PyObject* PyInit__opcode(void);

	extern PyObject* PyInit__contextvars(void);



struct _inittab _PyImport_Inittab[] = {

	{"posix", PyInit_posix},
	{"errno", PyInit_errno},
	{"pwd", PyInit_pwd},
	{"_sre", PyInit__sre},
	{"_codecs", PyInit__codecs},
	{"_weakref", PyInit__weakref},
	{"_functools", PyInit__functools},
	{"_operator", PyInit__operator},
	{"_collections", PyInit__collections},
	{"_abc", PyInit__abc},
	{"itertools", PyInit_itertools},
	{"atexit", PyInit_atexit},
	{"_signal", PyInit__signal},
	{"_stat", PyInit__stat},
	{"time", PyInit_time},
	{"_thread", PyInit__thread},
	{"_locale", PyInit__locale},
	{"_io", PyInit__io},
	{"zipimport", PyInit_zipimport},
	{"faulthandler", PyInit_faulthandler},
	{"_tracemalloc", PyInit__tracemalloc},
	{"_symtable", PyInit__symtable},
	{"xxsubtype", PyInit_xxsubtype},
	/* -- ADDMODULE MARKER 2 -- */

	/* This module lives in marshal.c */
	{"marshal", PyMarshal_Init},

	/* This lives in import.c */
	{"_imp", PyInit__imp},

	/* This lives in Python/Python-ast.c */
	{"_ast", PyInit__ast},

	/* These entries are here for sys.builtin_module_names */
	{"builtins", NULL},
	{"sys", NULL},

	/* This lives in gcmodule.c */
	{"gc", PyInit_gc},

	/* This lives in _warnings.c */
	{"_warnings", _PyWarnings_Init},

	/* This lives in Objects/unicodeobject.c */
	{"_string", PyInit__string},


	{"array", PyInit_array},
//	{"audioop", PyInit_audioop},
	{"binascii", PyInit_binascii},
//	{"cmath", PyInit_cmath},
	{"math", PyInit_math},
	{"_md5", PyInit__md5},
	{"_sha1", PyInit__sha1},
	{"_sha256", PyInit__sha256},
	{"_sha512", PyInit__sha512},
	{"_sha3", PyInit__sha3},
	{"_blake2", PyInit__blake2},
#ifdef WIN32
	{"msvcrt", PyInit_msvcrt},
	{"_locale", PyInit__locale},
	{"nt", PyInit_nt}, /* Use the NT os functions, not posix */
	/* XXX Should _winapi go in a WIN32 block?  not WIN64? */
	{"_winapi", PyInit__winapi},
	{"winreg", PyInit_winreg},
#endif
	{"_random", PyInit__random},
//	{"_bisect", PyInit__bisect},
//	{"_heapq", PyInit__heapq},
	{"_lsprof", PyInit__lsprof},
//	{"mmap", PyInit_mmap},
//	{"_csv", PyInit__csv},
//	{"parser", PyInit_parser},
	{"_struct", PyInit__struct},
	{"_datetime", PyInit__datetime},
	{"_json", PyInit__json},

#ifdef _Py_HAVE_ZLIB
	{"zlib", PyInit_zlib},
#endif

	/* CJK codecs */
//	{"_multibytecodec", PyInit__multibytecodec},
//	{"_codecs_cn", PyInit__codecs_cn},
//	{"_codecs_hk", PyInit__codecs_hk},
//	{"_codecs_iso2022", PyInit__codecs_iso2022},
//	{"_codecs_jp", PyInit__codecs_jp},
//	{"_codecs_kr", PyInit__codecs_kr},
//	{"_codecs_tw", PyInit__codecs_tw},


//	{"_pickle", PyInit__pickle},
//	{"_opcode", PyInit__opcode},

//	{"_contextvars", PyInit__contextvars},

	/* Sentinel */
    {0, 0}
};


#ifdef __cplusplus
}
#endif
