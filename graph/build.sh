#!/bin/bash
set -euo pipefail

rm -rf build
mkdir build

clear

# g++ -g -O0 $(find . -name "*.cpp") -o build/bin;

g++ -g -O0 weighted.cpp -o build/bin
