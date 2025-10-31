@echo off
echo Creating Windows Installer for AntsDistSat...
echo.

REM Check if Inno Setup is installed
set "INNO_SETUP_PATH="
if exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" (
    set "INNO_SETUP_PATH=C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
) else if exist "C:\Program Files\Inno Setup 6\ISCC.exe" (
    set "INNO_SETUP_PATH=C:\Program Files\Inno Setup 6\ISCC.exe"
) else (
    echo Error: Inno Setup not found!
    echo Please install Inno Setup from https://jrsoftware.org/isdl.php
    echo.
    pause
    exit /b 1
)

REM Check if Debug build exists
if not exist "Builds\VisualStudio2022\x64\Debug\VST3\AntsDistSat.vst3" (
    echo Error: Debug build not found!
    echo Please run build_release.bat first to build the plugin.
    echo.
    pause
    exit /b 1
)

echo Building installer...
call "%INNO_SETUP_PATH%" "%~dp0Setup.iss"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Installer created successfully!
    echo.
    echo The installer should be in:
    echo C:\Users\%USERNAME%\Documents\Inno Setup Compiler\Output\
    echo.
) else (
    echo.
    echo Installer creation failed!
    echo.
)

pause

