@echo off
setlocal enabledelayedexpansion
:: Configuration
set PROJECT_DIR=%~dp0
set SOURCE_DIR=src
set BUILD_DIR=build
set LIB_DIR=lib
set MEDIA_DIR=%PROJECT_DIR%media
set EXECUTABLE=EndlessCavess.exe
set COMPILER=g++
set LINKER=g++
set MEDIA_DEFINE=-Dmedia="\"!MEDIA_DIR:\=/!/\""
set COMPILER_FLAGS=-std=c++20 -c -I%SOURCE_DIR% %MEDIA_DEFINE% -DDEBUG
set LINKER_FLAGS=-L%LIB_DIR% -lMyGraphics2 -lsoil -lopengl32 -lglew32 -lglfw3

:: Check media folder
if not exist "%MEDIA_DIR%" (
    echo Error: Media folder not found at %MEDIA_DIR%
    exit /b 1
)

:: Create build directory if it doesn't exist
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: Compile all .cpp files from src
echo Compiling source files...
for %%f in (%SOURCE_DIR%\*.cpp) do (
    echo Compiling %%~nxf...
    %COMPILER% %COMPILER_FLAGS% %%f -o %BUILD_DIR%\%%~nf.o
    if errorlevel 1 (
        echo Error compiling %%~nxf
        exit /b 1
    )
)

:: Link all object files
echo Linking...
%LINKER% %BUILD_DIR%\*.o %LINKER_FLAGS% -o %BUILD_DIR%\%EXECUTABLE%
if errorlevel 1 (
    echo Error linking
    exit /b 1
)

:: Clean up object files
del %BUILD_DIR%\*.o

%BUILD_DIR%\%EXECUTABLE%