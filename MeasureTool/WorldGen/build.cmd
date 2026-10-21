@echo off
rem AI (Claude) measurement tool: build and run. Not part of the game build. Build output goes to bin\ (ignored by git).
rem Usage: build.cmd <level>    e.g. build.cmd L1_0
set "HERE=%~dp0"
set "GAME=%HERE%..\..\dontstarveCopy\dontstarveCopy"
set "DOCS=%HERE%..\..\docs\WorldGen"
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -property installationPath`) do set "VSROOT=%%i"
call "%VSROOT%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
if not exist "%HERE%bin" mkdir "%HERE%bin"
if not exist "%HERE%results" mkdir "%HERE%results"
if not exist "%DOCS%" mkdir "%DOCS%"
cl /nologo /EHsc /std:c++17 /O2 /W3 /utf-8 /DUNICODE /D_UNICODE /I"%GAME%" /I"%GAME%\Core" "%GAME%\Core\WorldGenerator.cpp" "%GAME%\Core\Terrain.cpp" "%HERE%WorldGenMeasure.cpp" /Fo"%HERE%bin\\" /Fe"%HERE%bin\WorldGenMeasure.exe" /link user32.lib gdiplus.lib > "%HERE%bin\compile.log"
if errorlevel 1 (type "%HERE%bin\compile.log" & exit /b 1)
"%HERE%bin\WorldGenMeasure.exe" %1 "%HERE%results\%1.txt" "%DOCS%\%1.png"
