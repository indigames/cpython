@echo off

SET CALL_DIR=%CD%
SET PROJECT_DIR=%~dp0..
SET OUTPUT_DIR=%PROJECT_DIR%\Release\include

echo Fetching headers...

if exist "%OUTPUT_DIR%" (
	rmdir /s /q %OUTPUT_DIR%
)
mkdir %OUTPUT_DIR%
mkdir %OUTPUT_DIR%\Android
mkdir %OUTPUT_DIR%\PC
mkdir %OUTPUT_DIR%\IOS
mkdir %OUTPUT_DIR%\Mac

xcopy /q /s /y %~dp0..\Include\*.h?? %OUTPUT_DIR%
xcopy /q /s /y %~dp0..\Android\*.h?? %OUTPUT_DIR%\Android
xcopy /q /s /y %~dp0..\PC\*.h?? %OUTPUT_DIR%\PC
xcopy /q /s /y %~dp0..\IOS\*.h?? %OUTPUT_DIR%\IOS
xcopy /q /s /y %~dp0..\Mac\*.h?? %OUTPUT_DIR%\Mac

cd %CALL_DIR%
echo Fetching headers DONE!
