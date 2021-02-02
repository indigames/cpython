cmake_minimum_required(VERSION 2.8.12)

project(Python)

file(TO_CMAKE_PATH $ENV{IGE_LIBS} IGE_LIBS)


IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OSX 1)
ELSEIF(IOS_PLATFORM)
    set(IOS 1)
ENDIF()

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
        add_compile_options(-g0 -Ofast)
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-Og)
    endif()
elseif(MSVC)
    add_compile_options(/MP)
elseif(IOS)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-g0 -Ofast)
    endif()
endif()
# [/IGE]

set(PYTHON_LIB Python39)
set(PYTHON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Include" "${CMAKE_CURRENT_SOURCE_DIR}/Include/internal")

if (MSVC)
    list(APPEND PYTHON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/PC")
elseif (OSX)
    list(APPEND PYTHON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Mac")
elseif (ANDROID)
    list(APPEND PYTHON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Android")
elseif (IOS)
    list(APPEND PYTHON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/IOS")
endif ()

if(BUILD_ALL_SRC)
    set(BUILD_TYPE STATIC)
else()
    set(BUILD_TYPE OBJECT)
endif()

# [IGE]: use default runtime
# Force static runtime libraries
# if(MSVC)
  # set(CompilerFlags
        # CMAKE_CXX_FLAGS
        # CMAKE_CXX_FLAGS_DEBUG
        # CMAKE_CXX_FLAGS_RELEASE
        # CMAKE_CXX_FLAGS_MINSIZEREL
        # CMAKE_CXX_FLAGS_RELWITHDEBINFO
        # CMAKE_C_FLAGS
        # CMAKE_C_FLAGS_DEBUG
        # CMAKE_C_FLAGS_RELEASE
        # CMAKE_C_FLAGS_MINSIZEREL
        # CMAKE_C_FLAGS_RELWITHDEBINFO
        # )
  # foreach(CompilerFlag ${CompilerFlags})
      # string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
      # set(${CompilerFlag} "${${CompilerFlag}}" CACHE STRING "msvc compiler flags" FORCE)
      # message("MSVC flags: ${CompilerFlag}:${${CompilerFlag}}")
  # endforeach()
# endif()
# [/IGE]

################# BUILT-IN MODULES ########################

### 01: _sqlite3
add_library(_sqlite ${BUILD_TYPE}
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite/cache.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite/connection.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite/cursor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite/microprotocols.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite/module.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite/prepare_protocol.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite/row.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite/statement.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite/util.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite/sqlite3.c # [IGE]: manual added to Python
)
target_include_directories(_sqlite PRIVATE ${PYTHON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sqlite)
target_compile_definitions(_sqlite PRIVATE MODULE_NAME="sqlite3" )
if(ANDROID OR IOS)
    target_compile_options(_sqlite PUBLIC "-fno-fast-math")
endif()

### 02: _ssl
if(MSVC)
    set(sslsrc ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ssl.c
        ${IGE_LIBS}/openssl/libs/pc/x64/applink.c)
    set(ssllib
        ${IGE_LIBS}/openssl/libs/pc/x64/crypto.lib
        ${IGE_LIBS}/openssl/libs/pc/x64/ssl.lib
    )
elseif(OSX)
    set(sslsrc ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ssl.c)
    set(ssllib
        ${IGE_LIBS}/openssl/libs/macos/x64/libcrypto.a
        ${IGE_LIBS}/openssl/libs/macos/x64/libssl.a)
elseif(IOS)
    set(sslsrc ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ssl.c)
    set(ssllib
        ${IGE_LIBS}/openssl/libs/ios/arm64/libcrypto.a
        ${IGE_LIBS}/openssl/libs/ios/arm64/libssl.a)
elseif(ANDROID)
    set(sslsrc ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ssl.c)
    set(ssllib
        ${IGE_LIBS}/openssl/libs/android/${ANDROID_ABI}/libssl.a
        ${IGE_LIBS}/openssl/libs/android/${ANDROID_ABI}/libcrypto.a)
endif()

add_library(_ssl ${BUILD_TYPE} ${sslsrc})
target_include_directories(_ssl PRIVATE ${PYTHON_INCLUDE_DIRS} ${IGE_LIBS}/openssl/include)
target_link_libraries(_ssl ${ssllib})

### _hashlib
add_library(_hashlib ${BUILD_TYPE} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_hashopenssl.c)
target_include_directories(_hashlib PRIVATE ${PYTHON_INCLUDE_DIRS} ${IGE_LIBS}/openssl/include)


### 03. _socket
add_library(_socket ${BUILD_TYPE} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/socketmodule.c)
target_include_directories(_socket PRIVATE ${PYTHON_INCLUDE_DIRS})

### 04. select
add_library(_select ${BUILD_TYPE} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/selectmodule.c)
target_include_directories(_select PRIVATE ${PYTHON_INCLUDE_DIRS})

### 05. unicodedata
add_library(_unicodedata ${BUILD_TYPE} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/unicodedata.c)
target_include_directories(_unicodedata PRIVATE ${PYTHON_INCLUDE_DIRS})
target_compile_definitions(_unicodedata PRIVATE Py_BUILD_CORE)

### 06. _queue
add_library(_queue ${BUILD_TYPE} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_queuemodule.c)
target_include_directories(_queue PRIVATE ${PYTHON_INCLUDE_DIRS})

### 07. _ctypes
set(CTYPES_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ctypes/_ctypes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ctypes/_ctypes_test.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ctypes/callbacks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ctypes/callproc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ctypes/cfield.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ctypes/malloc_closure.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ctypes/stgdict.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ctypes/libffi_dummy/ffi_dummy.c
)

set(CTYPES_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ctypes
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_ctypes/libffi_dummy
    ${PYTHON_INCLUDE_DIRS}
)
add_library(_ctypes ${BUILD_TYPE} ${PYTHON_INCLUDE_DIRS} ${CTYPES_SOURCE})
target_include_directories(_ctypes PRIVATE ${CTYPES_INCLUDES})
target_compile_definitions(_ctypes PRIVATE Py_BUILD_CORE)


### 08. pyexpat
add_library(_pyexpat ${BUILD_TYPE} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/pyexpat.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/expat/xmlparse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/expat/xmlrole.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/expat/xmltok.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/expat/xmltok_impl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/expat/xmltok_ns.c
)
target_compile_definitions(_pyexpat PUBLIC HAVE_EXPAT_CONFIG_H=1)
target_include_directories(_pyexpat PRIVATE ${PYTHON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/expat)

### 09. _asyncio
add_library(_asyncio ${BUILD_TYPE} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_asynciomodule.c)
target_include_directories(_asyncio PRIVATE ${PYTHON_INCLUDE_DIRS})
target_compile_definitions(_asyncio PRIVATE Py_BUILD_CORE)

### 10. zlibmodule
add_library(_zlib ${BUILD_TYPE} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/zlibmodule.c)
target_include_directories(_zlib PRIVATE ${PYTHON_INCLUDE_DIRS} ${IGE_LIBS}/zlib/include)
if(MSVC)
    target_link_libraries(_zlib ${IGE_LIBS}/zlib/libs/pc/x64/zlib.lib)
endif()


### _decimal
add_library(_decimal ${BUILD_TYPE} 
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/_decimal.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/basearith.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/constants.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/context.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/convolute.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/crt.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/difradix2.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/fnt.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/fourstep.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/io.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/mpalloc.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/mpdecimal.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/numbertheory.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/sixstep.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec/transpose.c
)
target_include_directories(_decimal PRIVATE ${PYTHON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_decimal/libmpdec)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	target_compile_definitions(_decimal PRIVATE CONFIG_64 ANSI)
else()
	target_compile_definitions(_decimal PRIVATE CONFIG_32 ANSI)
endif()



### 11. Other modules and Python core
IF(MSVC)
    set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/PC/config.c
        ${CMAKE_CURRENT_SOURCE_DIR}/PC/getpathp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/PC/invalid_parameter_handler.c
        ${CMAKE_CURRENT_SOURCE_DIR}/PC/msvcrtmodule.c
        ${CMAKE_CURRENT_SOURCE_DIR}/PC/winreg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Python/dynload_win.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_winapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_io/winconsoleio.c
    )
    set (INCLUDES
            PC
    )
ELSEIF(Linux)
ELSEIF(OSX)
    set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Mac/config.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Python/dynload_shlib.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Python/frozenmain.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/getpath.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/pwdmodule.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_posixsubprocess.c
    )
ELSEIF(IOS)
    set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/IOS/config.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Python/dynload_shlib.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/getpath.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/pwdmodule.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_posixsubprocess.c
    )
ELSEIF(ANDROID)
    set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Android/config.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Python/dynload_shlib.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/getpath.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/pwdmodule.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_posixsubprocess.c
    )
ENDIF()

set(SOURCES ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/arraymodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/atexitmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/audioop.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/binascii.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cmathmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/errnomodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/faulthandler.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/fcntlmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/gcmodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/getaddrinfo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/getbuildinfo.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/getnameinfo.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/getpath.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/grpmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/itertoolsmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/mathmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/md5module.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/mmapmodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/nismodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/ossaudiodev.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/overlapped.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/parsermodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/posixmodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/readline.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/resource.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/rotatingtree.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/selectmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/sha1module.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/sha256module.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/sha512module.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/signalmodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/spwdmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/symtablemodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/syslogmodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/termios.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/timemodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/tkappinit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/xxlimited.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/xxmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/xxsubtype.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_abc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_bisectmodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_bz2module.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_codecsmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_collectionsmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_contextvarsmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_cryptmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_csv.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_cursesmodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_curses_panel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_datetimemodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_dbmmodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_elementtree.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_functoolsmodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_gdbmmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_heapqmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_json.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_localemodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_lsprof.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_lzmamodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_math.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_opcode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_operator.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_peg_parser.c
${CMAKE_CURRENT_SOURCE_DIR}/Modules/_pickle.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_randommodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_scproxy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sre.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_stat.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_statisticsmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_struct.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_testbuffer.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_testcapimodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_testimportmultiple.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_testinternalcapi.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_testmultiphase.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_threadmodule.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_tkinter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_tracemalloc.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_uuidmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_weakref.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_xxsubinterpretersmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_zoneinfo.c

	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_blake2/blake2module.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_blake2/blake2b_impl.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_blake2/blake2s_impl.c
	

	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_multiprocessing/multiprocessing.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_multiprocessing/posixshmem.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_multiprocessing/semaphore.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/_sha3/sha3module.c

    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_io/_iomodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_io/bufferedio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_io/bytesio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_io/fileio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_io/iobase.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_io/stringio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/_io/textio.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Modules/_io/winconsoleio.c
	
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_cn.c	
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_iso2022.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_jp.c	
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_kr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_tw.c


    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_cn.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_hk.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_iso2022.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_jp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_kr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/_codecs_tw.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/cjkcodecs/multibytecodec.c

    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/abstract.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/accu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/boolobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/bytearrayobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/bytesobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/bytes_methods.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/call.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/capsule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/cellobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/classobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/codeobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/complexobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/descrobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/dictobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/enumobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/exceptions.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/fileobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/floatobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/frameobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/funcobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/genericaliasobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/genobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/interpreteridobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/iterobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/listobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/longobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/memoryobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/methodobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/moduleobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/namespaceobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/object.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/obmalloc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/odictobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/picklebufobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/rangeobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/setobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/sliceobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/structseq.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/tupleobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/typeobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/unicodectype.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/unicodeobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Objects/weakrefobject.c

    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/acceler.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/grammar1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/listnode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/myreadline.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/node.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/parser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/parsetok.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/token.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/tokenizer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/pegen/parse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/pegen/parse_string.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/pegen/pegen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/pegen/peg_api.c


    ${CMAKE_CURRENT_SOURCE_DIR}/Python/asdl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/ast.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/ast_opt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/ast_unparse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/bltinmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/bootstrap_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/ceval.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/codecs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/compile.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/dtoa.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Python/dup2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/dynamic_annotations.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/errors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/fileutils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/formatter_unicode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/frozen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/future.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/getargs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/getcompiler.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/getcopyright.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/getopt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/getplatform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/getversion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/graminit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/hamt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/hashtable.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/import.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/importdl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/initconfig.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/marshal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/modsupport.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/mysnprintf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/mystrtoul.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pathconfig.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/peephole.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/preconfig.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pyarena.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pyctype.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pyfpe.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pyhash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pylifecycle.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pymath.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pystate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pystrcmp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pystrhex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pystrtod.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/Python-ast.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pythonrun.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/pytime.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Python/strdup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/structmember.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/symtable.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/sysmodule.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/traceback.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Python/_warnings.c


    ${CMAKE_CURRENT_SOURCE_DIR}/builtin.c
)

add_library(${PYTHON_LIB} STATIC ${SOURCES})

set(DEFINITIONS Py_BUILD_CORE Py_BUILD_CORE_BUILTIN Py_NO_ENABLE_SHARED PYXIE_SKIP_GIL_CHECK USE_IGE)
if(MSVC)
	set(DEFINITIONS ${DEFINITIONS} PY3_DLLNAME="python3")
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        set(DEFINITIONS ${DEFINITIONS} _AMD64_)
    else()
        set(DEFINITIONS ${DEFINITIONS} _X86_)
    endif()
endif()

target_compile_definitions(${PYTHON_LIB} PUBLIC ${DEFINITIONS})

target_include_directories(${PYTHON_LIB} PUBLIC
    ${IGE_LIBS}/pyxCore/include/pyxcore
    ${IGE_LIBS}/igeCore/include
    ${PYTHON_INCLUDE_DIRS}
    ${INCLUDES}
)

target_link_libraries(${PYTHON_LIB}
	_decimal
    _sqlite
    _ssl
	_hashlib
    _socket
    _select
    _unicodedata
    _queue
    _ctypes
    _pyexpat
    _asyncio
    _zlib
)
