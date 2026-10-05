#!/bin/bash
set -euo pipefail

rm -rf build
mkdir build

clear

g++ -g -O0 radixMSD.cpp -o build/bin
