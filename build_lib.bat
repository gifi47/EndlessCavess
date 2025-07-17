@echo off
setlocal enabledelayedexpansion
:: Configuration
set PROJECT_DIR=%~dp0
set SOURCE_DIR1=src/Rendering
set SOURCE_DIR2=src/Test
set SOURCE_DIR3=src/TestObjects
set SOURCE_DIR4=src/Utils
set BUILD_DIR=lib
set STATIC_LIB=libMyGraphics2.a
set COMPILER=g++
set LINKER=ar
set COMPILER_FLAGS=-std=c++20 -c -I%SOURCE_DIR1% -I%SOURCE_DIR2% -I%SOURCE_DIR3% -I%SOURCE_DIR4% -DGLEW_STATIC
set LINKER_FLAGS=rcs

:: Create build directory if it doesn't exist
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: Compile all .cpp files from src
echo Compiling source files...
for %%f in (%SOURCE_DIR1%\*.cpp) do (
    echo Compiling %%~nxf...
    %COMPILER% %COMPILER_FLAGS% %%f -o %BUILD_DIR%\%%~nf.o
    if errorlevel 1 (
        echo Error compiling %%~nxf
        exit /b 1
    )
)

:: Compile all .cpp files from src
echo Compiling source files...
for %%f in (%SOURCE_DIR2%\*.cpp) do (
    echo Compiling %%~nxf...
    %COMPILER% %COMPILER_FLAGS% %%f -o %BUILD_DIR%\%%~nf.o
    if errorlevel 1 (
        echo Error compiling %%~nxf
        exit /b 1
    )
)

:: Compile all .cpp files from src
echo Compiling source files...
for %%f in (%SOURCE_DIR3%\*.cpp) do (
    echo Compiling %%~nxf...
    %COMPILER% %COMPILER_FLAGS% %%f -o %BUILD_DIR%\%%~nf.o
    if errorlevel 1 (
        echo Error compiling %%~nxf
        exit /b 1
    )
)

:: Compile all .cpp files from src
echo Compiling source files...
for %%f in (%SOURCE_DIR4%\*.cpp) do (
    echo Compiling %%~nxf...
    %COMPILER% %COMPILER_FLAGS% %%f -o %BUILD_DIR%\%%~nf.o
    if errorlevel 1 (
        echo Error compiling %%~nxf
        exit /b 1
    )
)

:: Link all object files
echo Linking...
%LINKER% %LINKER_FLAGS% %BUILD_DIR%\%STATIC_LIB% %BUILD_DIR%\*.o
if errorlevel 1 (
    echo Error linking
    exit /b 1
)

:: Clean up object files
del %BUILD_DIR%\*.o

echo Build successful! Lib: %BUILD_DIR%\%STATIC_LIB%