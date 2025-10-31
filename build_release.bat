@echo off
echo Building AntsDistSat Debug version...
echo.

REM Change to Visual Studio solution directory
cd /d "%~dp0Builds\VisualStudio2022"

REM Build the solution in Debug mode
echo Building Debug x64 configuration...
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" AntsDistSat.sln /p:Configuration=Debug /p:Platform=x64 /t:Build /m

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
    echo VST3 plugin location: Builds\VisualStudio2022\x64\Debug\VST3\AntsDistSat.vst3
    echo.
    echo You can now create the installer using Inno Setup with Setup.iss
) else (
    echo.
    echo Build failed! Check the error messages above.
    exit /b %ERRORLEVEL%
)

pause

