cmake_minimum_required(VERSION 3.3)
project(squid)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

include_directories(BEFORE .)

set(SOURCE_FILES main.cpp squid/vector/vector.hpp squid/matrix/matrix.hpp squid/vector/impl.hpp squid/matrix/impl.hpp squid/matrix/iterator.hpp)
add_executable(squid ${SOURCE_FILES})