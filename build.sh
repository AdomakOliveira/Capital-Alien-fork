#!/usr/bin/env bash
set -e

echo "Configurando o projeto (CMake)..."
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

echo "Compilando (CapitalAlien)..."
cmake --build build --config Debug -- -j"$(nproc)"

echo
echo "Build concluido!"
echo

./build/CapitalAlien
