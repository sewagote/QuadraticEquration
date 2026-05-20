#!/bin/bash
set -e

g++ -O2 -std=c++17 \
    tests/test_QuadSolver.cpp QuadSolver.cpp BigDecimal.cpp UnsignedArith.cpp \
    -o tests/run_QuadSolver

./tests/run_QuadSolver
