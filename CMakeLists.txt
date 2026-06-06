cmake_minimum_required(VERSION 3.16)
project(QuadraticSolver CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Библиотека длинной арифметики + решателя (общая для программы и тестов)
add_library(quadcore
    src/UnsignedArith.cpp
    src/BigDecimal.cpp
    src/QuadSolver.cpp
)

# Основная программа
add_executable(QuadraticSolver src/main.cpp)
target_link_libraries(QuadraticSolver PRIVATE quadcore)

# Тесты
enable_testing()
foreach(suite QuadSolver BigDecimal UnsignedArith)
    add_executable(test_${suite} tests/test_${suite}.cpp)
    target_link_libraries(test_${suite} PRIVATE quadcore)
    add_test(NAME ${suite} COMMAND test_${suite})
endforeach()
