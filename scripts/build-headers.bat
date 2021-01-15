@echo off

SET LIB_NAME=Python

SET CALL_DIR=%CD%
SET PROJECT_DIR=%~dp0..
SET OUTPUT_HEADER=%IGE_LIBS%\%LIB_NAME%

echo Fetching %LIB_NAME% headers...

if exist "%OUTPUT_HEADER%\Include" (
	rmdir /s /q %OUTPUT_HEADER%\Include
	rmdir /s /q %OUTPUT_HEADER%\Android
	rmdir /s /q %OUTPUT_HEADER%\IOS
	rmdir /s /q %OUTPUT_HEADER%\Mac
	rmdir /s /q %OUTPUT_HEADER%\PC
)
if not exist "%OUTPUT_HEADER%" (
    mkdir %OUTPUT_HEADER%
)

xcopy /q /s /y %~dp0..\Include\*.h?? %OUTPUT_HEADER%\Include\
xcopy /q /s /y %~dp0..\Android\*.h?? %OUTPUT_HEADER%\Android\
xcopy /q /s /y %~dp0..\IOS\*.h?? %OUTPUT_HEADER%\IOS\
xcopy /q /s /y %~dp0..\Mac\*.h?? %OUTPUT_HEADER%\Mac\
xcopy /q /s /y %~dp0..\PC\*.h?? %OUTPUT_HEADER%\PC\

cd %CALL_DIR%
echo Fetching %LIB_NAME% headers DONE!
