@echo off

echo COMPILING ...
SET CALL_DIR=%CD%
SET PROJECT_DIR=%~dp0..
SET BUILD_DIR=%PROJECT_DIR%\build\pc
SET OUTPUT_DIR=%PROJECT_DIR%\Release\libs\pc

echo Compiling ...
if not exist %OUTPUT_DIR% (
    mkdir %OUTPUT_DIR%
)
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

echo Cleaning up...
    if exist %OUTPUT_DIR% (
        rmdir /s /q %OUTPUT_DIR%
    )
    mkdir %OUTPUT_DIR%

cd %PROJECT_DIR%
echo Compiling x64...
    if not exist %BUILD_DIR%\x64 (
        mkdir %BUILD_DIR%\x64
    )
    echo Generating x64 CMAKE project ...
    cd %BUILD_DIR%\x64
    cmake %PROJECT_DIR% -A x64
    if %ERRORLEVEL% NEQ 0 goto ERROR

    echo Compiling x64 - Release...
    cmake --build . --config Release -- -m
    if %ERRORLEVEL% NEQ 0 goto ERROR
    xcopy /q /s /y Release\*.lib %OUTPUT_DIR%\x64\
    xcopy /q /s /y Release\*.dll %OUTPUT_DIR%\x64\
echo Compiling x64 DONE

goto ALL_DONE

:ERROR
    echo ERROR OCCURED DURING COMPILING!

:ALL_DONE
    cd %CALL_DIR%
    echo COMPILING DONE!