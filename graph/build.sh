#!/bin/bash
set -euo pipefail

rm -rf build
mkdir build

g++ -g -O0 $(find . -name "*.cpp") -o build/bin;
