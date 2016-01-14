cmake_minimum_required(VERSION 3.3)
project(maths)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

set(SOURCE_FILES main.cpp vector.hpp matrix.hpp)
add_executable(maths ${SOURCE_FILES})