@echo off
setlocal enabledelayedexpansion

set CALL_DIR=%CD%
set PROJECT_DIR=%~dp0..

if not exist "%ANDROID_SDK_ROOT%" (
    if exist "%ANDROID_HOME%" (
        set ANDROID_SDK_ROOT=%ANDROID_HOME%
    ) else if exist "%ANDROID_SDK_HOME%" (
        set ANDROID_SDK_ROOT=%ANDROID_SDK_HOME%
    ) else if exist "%USERPROFILE%\AppData\Local\Android\sdk" (
        set ANDROID_SDK_ROOT=%USERPROFILE%\AppData\Local\Android\sdk
    ) else (
        echo ERROR: SDK NOT FOUND!
        goto ERROR
    )    
)
echo SDK: !ANDROID_SDK_ROOT!

if not exist "%ANDROID_NDK_ROOT%" (
    if exist "%ANDROID_NDK_HOME%" (
        set ANDROID_NDK_ROOT=%ANDROID_NDK_HOME%
    ) else if exist "%ANDROID_SDK_ROOT%\ndk-bundle" (
        set ANDROID_NDK_ROOT=%ANDROID_SDK_ROOT%\ndk-bundle
    ) else if exist "%ANDROID_SDK_ROOT%" (
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

cd %PROJECT_DIR%
python %PROJECT_DIR%/build.py

cd %CALL_DIR%
