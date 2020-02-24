@echo off

SET LIB_NAME=Python

echo COMPILING PC...
SET PROJECT_DIR=%~dp0..

SET OUTPUT_DIR=%PROJECT_DIR%\igeLibs\%LIB_NAME%
SET OUTPUT_HEADER=%OUTPUT_DIR%
SET OUTPUT_LIBS_DEBUG=%OUTPUT_DIR%\libs\Debug\pc
SET OUTPUT_LIBS_RELEASE=%OUTPUT_DIR%\libs\Release\pc

if not exist %OUTPUT_DIR% (
    mkdir %OUTPUT_DIR%
)

echo Cleaning up...
    if exist %OUTPUT_HEADER% (
        rmdir /s /q %OUTPUT_HEADER%
    )
    mkdir %OUTPUT_HEADER%

    if exist %OUTPUT_LIBS_DEBUG% (
        rmdir /s /q %OUTPUT_LIBS_DEBUG%
    )
    mkdir %OUTPUT_LIBS_DEBUG%

    if exist %OUTPUT_LIBS_RELEASE% (
        rmdir /s /q %OUTPUT_LIBS_RELEASE%
    )
    mkdir %OUTPUT_LIBS_RELEASE%

echo Fetching include headers...
    xcopy /h /i /c /k /e /r /y %~dp0..\Include\*.h %OUTPUT_HEADER%\Include\
    xcopy /h /i /c /k /e /r /y %~dp0..\Android\*.h %OUTPUT_HEADER%\Android\
    xcopy /h /i /c /k /e /r /y %~dp0..\IOS\*.h %OUTPUT_HEADER%\IOS\
    xcopy /h /i /c /k /e /r /y %~dp0..\Mac\*.h %OUTPUT_HEADER%\Mac\
    xcopy /h /i /c /k /e /r /y %~dp0..\PC\*.h %OUTPUT_HEADER%\PC\

cd %PROJECT_DIR%
echo Compiling x86...
    if not exist build\x86 (
        mkdir build\x86
    )
    cd build\x86
    echo Generating x86 CMAKE project ...
    cmake ..\.. -A Win32
    if errorlevel 1 goto ERROR

    echo Compiling x86 - Debug...
    cmake --build . --config Debug -- -m
    if errorlevel 1 goto ERROR
    xcopy /q /e /y Debug\*.lib %OUTPUT_LIBS_DEBUG%\x86\
    xcopy /q /e /y Debug\*.dll %OUTPUT_LIBS_DEBUG%\x86\

    echo Compiling x86 - Release...
    cmake --build . --config Release -- -m
    if errorlevel 1 goto ERROR
    xcopy /q /e /y Release\*.lib %OUTPUT_LIBS_RELEASE%\x86\
    xcopy /q /e /y Release\*.dll %OUTPUT_LIBS_RELEASE%\x86\
echo Compiling x86 DONE

cd %PROJECT_DIR%
echo Compiling x64...
    if not exist build\x64 (
        mkdir build\x64
    )
    echo Generating x64 CMAKE project ...
    cd build\x64
    cmake ..\.. -A x64
    if errorlevel 1 goto ERROR

    echo Compiling x64 - Debug...
    cmake --build . --config Debug -- -m
    if errorlevel 1 goto ERROR
    xcopy /q /e /y Debug\*.lib %OUTPUT_LIBS_DEBUG%\x64\
    xcopy /q /e /y Debug\*.dll %OUTPUT_LIBS_DEBUG%\x64\

    echo Compiling x64 - Release...
    cmake --build . --config Release -- -m
    if errorlevel 1 goto ERROR
    xcopy /q /e /y Release\*.lib %OUTPUT_LIBS_RELEASE%\x64\
    xcopy /q /e /y Release\*.dll %OUTPUT_LIBS_RELEASE%\x64\
echo Compiling x64 DONE
goto ALL_DONE

:ERROR
	echo ERROR OCCURED DURING COMPILING PC

:ALL_DONE
	cd %PROJECT_DIR%
	echo COMPILING PC DONE!