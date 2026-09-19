#!/bin/bash
set -euo pipefail

rm -rf build
mkdir build

clear

g++ -g -O0 graph_weighted/graph_weighted.cpp -o build/bin

# g++ -g -O0 $(find . -name "*.cpp") -o build/bin;
# g++ -g -O0 main.cpp graph/graph.cpp traversal/traversal.cpp
