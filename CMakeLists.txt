cmake_minimum_required(VERSION 3.13)
project(oop_lab4_matrix)

set(CMAKE_CXX_STANDARD 17)

add_executable(oop_lab4_matrix main.cpp Bignum.h Bignum.cpp Matrix.cpp Matrix.h Symbol.cpp Symbol.h)