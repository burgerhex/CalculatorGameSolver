cmake_minimum_required(VERSION 3.20)
project(CalculatorGameSolver)

set(CMAKE_CXX_STANDARD 23)

add_executable(CalculatorGameSolver main.cpp Operation.cpp Operation.h Solver.cpp Solver.h)
