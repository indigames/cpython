@echo off
setlocal enabledelayedexpansion

echo COMPILING ...
SET CALL_DIR=%CD%
SET PROJECT_DIR=%~dp0..
SET BUILD_DIR=%PROJECT_DIR%\build\android
SET OUTPUT_DIR=%PROJECT_DIR%\Release\libs\android

if not exist "%ANDROID_SDK_ROOT%" (
    if exist "%ANDROID_HOME%" (
        set ANDROID_SDK_ROOT=%ANDROID_HOME%
    ) else (
        if exist "%USERPROFILE%\AppData\Local\Android\sdk" (
            set ANDROID_SDK_ROOT=%USERPROFILE%\AppData\Local\Android\sdk
        ) else (
            echo ERROR: SDK NOT FOUND!
            goto ERROR
        )
    )
)
echo SDK: !ANDROID_SDK_ROOT!

if not exist "%ANDROID_NDK_ROOT%" (
    if exist "%ANDROID_SDK_ROOT%\ndk-bundle" (
        set ANDROID_NDK_ROOT=%ANDROID_SDK_ROOT%\ndk-bundle
    ) else (
        for /d %%D in (%ANDROID_SDK_ROOT%\ndk\*) do (
            set ANDROID_NDK_ROOT=%%~fD
        )
    )
)

if not exist "%ANDROID_NDK_ROOT%" (
    echo ERROR: NDK NOT FOUND!
    goto ERROR
)
echo NDK: !ANDROID_NDK_ROOT!

set ANDROID_TOOLCHAIN=!ANDROID_NDK_ROOT!\build\cmake\android.toolchain.cmake

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

if exist %OUTPUT_DIR% (
    rmdir /s /q %OUTPUT_DIR%
)
mkdir %OUTPUT_DIR%

cd %PROJECT_DIR%
echo Compiling arm64-v8a...
    if not exist %BUILD_DIR%\arm64-v8a\Release (
        mkdir %BUILD_DIR%\arm64-v8a\Release
    )
    cd %BUILD_DIR%\arm64-v8a\Release
    echo Generating arm64-v8a Release CMAKE project ...
    cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=!ANDROID_TOOLCHAIN! -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-21 -DCMAKE_BUILD_TYPE=Release %PROJECT_DIR%

    echo Compiling arm64-v8a - Release...
    cmake --build .
    if %ERRORLEVEL% NEQ 0 goto ERROR
    xcopy /q /y *.a %OUTPUT_DIR%\arm64-v8a\
    xcopy /q /y *.so %OUTPUT_DIR%\arm64-v8a\
echo Compiling arm64-v8a DONE

cd %PROJECT_DIR%
echo Compiling x86_64...
    if not exist %BUILD_DIR%\x86_64\Release (
        mkdir %BUILD_DIR%\x86_64\Release
    )
    cd %BUILD_DIR%\x86_64\Release
    echo Generating x86_64 Release CMAKE project ...
    cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=!ANDROID_TOOLCHAIN! -DANDROID_ABI=x86_64 -DANDROID_PLATFORM=android-21 -DCMAKE_BUILD_TYPE=Release %PROJECT_DIR%

    echo Compiling x86_64 - Release...
    cmake --build .
    if %ERRORLEVEL% NEQ 0 goto ERROR
    xcopy /q /y *.a %OUTPUT_DIR%\x86_64\
    xcopy /q /y *.so %OUTPUT_DIR%\x86_64\
echo Compiling x86_64 DONE

goto ALL_DONE

:ERROR
    echo ERROR OCCURED DURING COMPILING!

:ALL_DONE
    cd %CALL_DIR%
    echo COMPILING DONE!