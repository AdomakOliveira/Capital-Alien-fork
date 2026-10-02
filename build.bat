@echo off

cmake -S . -B build

if errorlevel 1 exit /b 1

cmake --build build --config Debug

if errorlevel 1 exit /b 1

build\Debug\CapitalAlien.exe

echo.

echo Codigo de saida: %errorlevel%

echo Build concluido!