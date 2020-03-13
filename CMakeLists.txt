cmake_minimum_required(VERSION 2.8.12)

project(Python)

set(IGE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/igeLibs)

# [IGE]: tweaks for platforms
if(DEFINED ANDROID_PLATFORM)
    set (ANDROID 1)
    if(ANDROID_ABI STREQUAL "armeabi-v7a")
        add_compile_options(-marm -march=armv7-a -mtune=cortex-a15 -mfpu=neon-fp16 -mfloat-abi=softfp)
    elseif(ANDROID_ABI STREQUAL "x86")
        add_compile_options(-march=atom -m32 -msse -msse2 -msse3 -mssse3 -mfpmath=sse -mstackrealign)
    elseif(ANDROID_ABI STREQUAL "arm64-v8a")
        add_compile_options(-march=armv8-a+simd+sve -mtune=cortex-a53 -D__ARM_NEON -D__ARM_NEON__)
    elseif(ANDROID_ABI STREQUAL "x86_64")
        add_compile_options(-march=atom -m64 -msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -mfpmath=sse)
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-Ofast)
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-Og)
    endif()
elseif(MSVC)
    add_compile_options(/MP)
endif()
# [/IGE]

set(PYTHON_LIB Python37)
set(PYTHON_INCLUDE_DIRS "Include")

if (MSVC)
    list(APPEND PYTHON_INCLUDE_DIRS "PC")
elseif (OSX)
    list(APPEND PYTHON_INCLUDE_DIRS "Mac")
elseif (ANDROID)
    list(APPEND PYTHON_INCLUDE_DIRS "Android")
elseif (IOS)
    list(APPEND PYTHON_INCLUDE_DIRS "IOS")
endif ()

# Force static runtime libraries
if(MSVC)
  set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL
        CMAKE_C_FLAGS_RELWITHDEBINFO
        )
  foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
      set(${CompilerFlag} "${${CompilerFlag}}" CACHE STRING "msvc compiler flags" FORCE)
      message("MSVC flags: ${CompilerFlag}:${${CompilerFlag}}")
  endforeach()
endif()

################# BUILT-IN MODULES ########################

### 01: _sqlite3
add_library(_sqlite OBJECT
    Modules/_sqlite/cache.c
    Modules/_sqlite/connection.c
    Modules/_sqlite/cursor.c
    Modules/_sqlite/microprotocols.c
    Modules/_sqlite/module.c
    Modules/_sqlite/prepare_protocol.c
    Modules/_sqlite/row.c
    Modules/_sqlite/statement.c
    Modules/_sqlite/util.c
    Modules/_sqlite/sqlite3.c # [IGE]: manual added to Python
)
target_include_directories(_sqlite PRIVATE ${PYTHON_INCLUDE_DIRS} Modules/_sqlite)
target_compile_definitions(_sqlite PRIVATE MODULE_NAME="sqlite3" )
if(ANDROID)
    target_compile_options(_sqlite PUBLIC "-fno-fast-math")
endif()

### 02: _ssl
if(MSVC)
    set(sslsrc Modules/_ssl.c ${IGE_ROOT}/openssl/libs/pc/Release/x64/applink.c)
    set(ssllib
        debug ${IGE_ROOT}/openssl/libs/pc/Debug/x64/libcrypto.lib
        debug${IGE_ROOT}/openssl/libs/pc/Debug/x64/libssl.lib
        optimized ${IGE_ROOT}/openssl/libs/pc/Release/x64/libcrypto.lib
        optimized ${IGE_ROOT}/openssl/libs/pc/Release/x64/libssl.lib
    )
elseif(OSX)
    set(sslsrc Modules/_ssl.c)
    set(ssllib
        ${IGE_ROOT}/openssl/libs/macos/x64/libcrypto.a
        ${IGE_ROOT}/openssl/libs/macos/x64/libssl.a)
elseif(IOS)
    set(sslsrc Modules/_ssl.c)
    set(ssllib
        ${IGE_ROOT}/openssl/libs/ios/arm64/libcrypto.a
        ${IGE_ROOT}/openssl/libs/ios/arm64/libssl.a)
elseif(ANDROID)
    set(sslsrc Modules/_ssl.c)
    set(ssllib
        ${IGE_ROOT}/openssl/libs/android/${ANDROID_ABI}/libssl.a
        ${IGE_ROOT}/openssl/libs/android/${ANDROID_ABI}/libcrypto.a)
endif()
add_library(_ssl OBJECT ${sslsrc})
target_include_directories(_ssl PRIVATE ${PYTHON_INCLUDE_DIRS} ${IGE_ROOT}/openssl/include)
target_link_libraries(_ssl ${ssllib})

### 03. _socket
add_library(_socket OBJECT Modules/socketmodule.c)
target_include_directories(_socket PRIVATE ${PYTHON_INCLUDE_DIRS})

### 04. select
add_library(_select OBJECT Modules/selectmodule.c)
target_include_directories(_select PRIVATE ${PYTHON_INCLUDE_DIRS})

### 05. unicodedata
add_library(_unicodedata OBJECT Modules/unicodedata.c)
target_include_directories(_unicodedata PRIVATE ${PYTHON_INCLUDE_DIRS})

### 06. _queue
add_library(_queue OBJECT Modules/_queuemodule.c)
target_include_directories(_queue PRIVATE ${PYTHON_INCLUDE_DIRS})

### 07. _ctypes
set(CTYPES_SOURCE
    Modules/_ctypes/_ctypes.c
    Modules/_ctypes/_ctypes_test.c
    Modules/_ctypes/callbacks.c
    Modules/_ctypes/callproc.c
    Modules/_ctypes/cfield.c
    Modules/_ctypes/malloc_closure.c
    Modules/_ctypes/stgdict.c
    Modules/_ctypes/libffi_dummy/ffi_dummy.c
)

set(CTYPES_INCLUDES
    Modules/_ctypes
    Modules/_ctypes/libffi_dummy
    ${PYTHON_INCLUDE_DIRS}
)

add_library(_ctypes OBJECT ${PYTHON_INCLUDE_DIRS} ${CTYPES_SOURCE})
target_include_directories(_ctypes PRIVATE ${CTYPES_INCLUDES})


### 08. pyexpat
add_library(_pyexpat OBJECT Modules/pyexpat.c
    Modules/expat/xmlparse.c
    Modules/expat/xmlrole.c
    Modules/expat/xmltok.c
    Modules/expat/xmltok_impl.c
    Modules/expat/xmltok_ns.c
)
target_compile_definitions(_pyexpat PUBLIC HAVE_EXPAT_CONFIG_H=1)
target_include_directories(_pyexpat PRIVATE ${PYTHON_INCLUDE_DIRS}  ${CMAKE_CURRENT_SOURCE_DIR}/Modules/expat)

### 09. _asyncio
add_library(_asyncio OBJECT Modules/_asynciomodule.c)
target_include_directories(_asyncio PRIVATE ${PYTHON_INCLUDE_DIRS})

### 10. zlibmodule
add_library(_zlib OBJECT Modules/zlibmodule.c)
target_include_directories(_zlib PRIVATE ${PYTHON_INCLUDE_DIRS} ${IGE_ROOT}/zlib/include)
if(MSVC)
    target_link_libraries(_zlib ${IGE_ROOT}/zlib/libs/Release/pc/x64/zlib.lib)
endif()

### 11. Other modules and Python core
IF(MSVC)
    set(SOURCES
        PC/config.c
        PC/getpathp.c
        PC/invalid_parameter_handler.c
        PC/msvcrtmodule.c
        PC/winreg.c
        Python/dynload_win.c
        Modules/_winapi.c
        Modules/_io/winconsoleio.c
    )
    set (INCLUDES
            PC
    )
ELSEIF(Linux)
ELSEIF(OSX)
    set(SOURCES
        Mac/config.c
        Python/dynload_shlib.c
        Python/frozenmain.c
        Modules/getpath.c
        Modules/pwdmodule.c
        Parser/pgen.c
        Modules/_posixsubprocess.c
    )
ELSEIF(IOS)
    set(SOURCES
        IOS/config.c
        Python/dynload_shlib.c
        Modules/getpath.c
        Modules/pwdmodule.c
        Modules/_posixsubprocess.c
    )
ELSEIF(ANDROID)
    set(SOURCES
        Android/config.c
        Python/dynload_shlib.c
        Modules/getpath.c
        Modules/pwdmodule.c
        Modules/_posixsubprocess.c
    )
ENDIF()

set(SOURCES ${SOURCES}
    Modules/posixmodule.c
    Modules/errnomodule.c
    Modules/_sre.c
    Modules/_codecsmodule.c
    Modules/_weakref.c
    Modules/_functoolsmodule.c
    Modules/_operator.c
    Modules/_collectionsmodule.c
    Modules/_abc.c
    Modules/itertoolsmodule.c
    Modules/atexitmodule.c
    Modules/signalmodule.c
    Modules/_stat.c
    Modules/timemodule.c
    Modules/_threadmodule.c
    Modules/_localemodule.c
    Modules/_io/_iomodule.c
    Modules/_io/iobase.c
    Modules/_io/fileio.c
    Modules/_io/bufferedio.c
    Modules/_io/textio.c
    Modules/_io/bytesio.c
    Modules/_io/stringio.c
    Modules/zipimport.c
    Modules/faulthandler.c
    Modules/_tracemalloc.c
    Modules/symtablemodule.c
    Modules/xxsubtype.c
    Modules/gcmodule.c
    Modules/getbuildinfo.c
    Modules/main.c
    Modules/hashtable.c
    Modules/_struct.c
    Modules/binascii.c
    Modules/cmathmodule.c
    Modules/_bisectmodule.c
    Modules/_blake2/blake2module.c
    Modules/_blake2/blake2b_impl.c
    Modules/_blake2/blake2s_impl.c
    Modules/_contextvarsmodule.c
    Modules/_csv.c
    Modules/_heapqmodule.c
    Modules/_json.c
    Modules/_lsprof.c
    Modules/_math.c
    Modules/_pickle.c
    Modules/_randommodule.c
    Modules/_sha3/sha3module.c
    Modules/arraymodule.c
    Modules/audioop.c
    Modules/_datetimemodule.c
    Modules/mathmodule.c
    Modules/md5module.c
    Modules/mmapmodule.c
    Modules/_opcode.c
    Modules/parsermodule.c
    Modules/rotatingtree.c
    Modules/sha1module.c
    Modules/sha256module.c
    Modules/sha512module.c
    Modules/cjkcodecs/_codecs_cn.c
    Modules/cjkcodecs/_codecs_hk.c
    Modules/cjkcodecs/_codecs_iso2022.c
    Modules/cjkcodecs/_codecs_jp.c
    Modules/cjkcodecs/_codecs_kr.c
    Modules/cjkcodecs/_codecs_tw.c
    Modules/cjkcodecs/multibytecodec.c

    Objects/abstract.c
    Objects/accu.c
    Objects/boolobject.c
    Objects/bytearrayobject.c
    Objects/bytes_methods.c
    Objects/bytesobject.c
    Objects/call.c
    Objects/capsule.c
    Objects/cellobject.c
    Objects/classobject.c
    Objects/codeobject.c
    Objects/complexobject.c
    Objects/descrobject.c
    Objects/dictobject.c
    Objects/enumobject.c
    Objects/exceptions.c
    Objects/fileobject.c
    Objects/floatobject.c
    Objects/frameobject.c
    Objects/funcobject.c
    Objects/genobject.c
    Objects/iterobject.c
    Objects/listobject.c
    Objects/longobject.c
    Objects/memoryobject.c
    Objects/methodobject.c
    Objects/moduleobject.c
    Objects/namespaceobject.c
    Objects/object.c
    Objects/obmalloc.c
    Objects/odictobject.c
    Objects/rangeobject.c
    Objects/setobject.c
    Objects/sliceobject.c
    Objects/structseq.c
    Objects/tupleobject.c
    Objects/typeobject.c
    Objects/unicodectype.c
    Objects/unicodeobject.c
    Objects/weakrefobject.c

    Parser/acceler.c
    Parser/bitset.c
    Parser/firstsets.c
    Parser/grammar.c
    Parser/grammar1.c
    Parser/listnode.c
    Parser/metagrammar.c
    Parser/myreadline.c
    Parser/node.c
    Parser/parser.c
    Parser/parsetok.c
    Parser/tokenizer.c

    Python/pyhash.c
    Python/_warnings.c
    Python/asdl.c
    Python/ast.c
    Python/ast_opt.c
    Python/ast_unparse.c
    Python/bltinmodule.c
    Python/bootstrap_hash.c
    Python/ceval.c
    Python/codecs.c
    Python/compile.c
    Python/context.c
    Python/dynamic_annotations.c
    Python/errors.c
    Python/fileutils.c
    Python/formatter_unicode.c
    Python/future.c
    Python/getargs.c
    Python/getcompiler.c
    Python/getcopyright.c
    Python/getopt.c
    Python/getplatform.c
    Python/getversion.c
    Python/graminit.c
    Python/hamt.c
    Python/import.c
    Python/importdl.c
    Python/marshal.c
    Python/modsupport.c
    Python/mysnprintf.c
    Python/mystrtoul.c
    Python/pathconfig.c
    Python/peephole.c
    Python/pyarena.c
    Python/pyctype.c
    Python/pyfpe.c
    Python/pylifecycle.c
    Python/pymath.c
    Python/pytime.c
    Python/pystate.c
    Python/pystrcmp.c
    Python/pystrhex.c
    Python/pystrtod.c
    Python/dtoa.c
    Python/Python-ast.c
    Python/pythonrun.c
    Python/structmember.c
    Python/symtable.c
    Python/sysmodule.c
    Python/thread.c
    Python/traceback.c
    Python/frozen.c
    builtin.c
)

add_library(${PYTHON_LIB} STATIC ${SOURCES})

set(DEFINITIONS Py_BUILD_CORE Py_NO_ENABLE_SHARED PYXIE_SKIP_GIL_CHECK USE_IGE)
if(MSVC)
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        set(DEFINITIONS ${DEFINITIONS} _AMD64_)
    else()
        set(DEFINITIONS ${DEFINITIONS} _X86_)
    endif()
endif()

target_compile_definitions(${PYTHON_LIB} PUBLIC ${DEFINITIONS})

target_include_directories(${PYTHON_LIB} PUBLIC
    ${IGE_ROOT}/pyxCore/include/pyxcore
    ${IGE_ROOT}/igeCore/include
    ${PYTHON_INCLUDE_DIRS}
    ${INCLUDES}
)

target_link_libraries(${PYTHON_LIB}
    _sqlite
    _ssl
    _socket
    _select
    _unicodedata
    _queue
    _ctypes
    _pyexpat
    _asyncio
    _zlib
)
