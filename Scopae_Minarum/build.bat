@echo off
echo ==========================
echo Building Scopae Minarum
echo ==========================
cd /d %~dp0

if not exist "build" mkdir "build"

gcc src/*.c ^
    -o build/scopae_minarum.exe ^
    -Iinclude ^
    -IC:\raylib\w64devkit\x86_64-w64-mingw32\include ^
    -LC:\raylib\w64devkit\x86_64-w64-mingw32\lib ^
    -lraylib -lopengl32 -lgdi32 -lwinmm

if %errorlevel% neq 0 (
    echo.
    echo BUILD FAILED
    pause
    exit /b 1
)

echo.
echo BUILD SUCCESS
echo.
pause