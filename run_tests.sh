#!/bin/bash
set -e

g++ -O2 -std=c++17 -Iinclude \
    tests/test_QuadSolver.cpp src/QuadSolver.cpp src/BigDecimal.cpp src/UnsignedArith.cpp \
    -o tests/run_QuadSolver

./tests/run_QuadSolver
