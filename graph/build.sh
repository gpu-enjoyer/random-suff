#!/bin/bash
set -euo pipefail

rm -rf build
mkdir build

clear

g++ -g -O0 main.cpp graph/graph.cpp traversal/traversal.cpp graph_weighted/graph_weighted.cpp -o build/bin
