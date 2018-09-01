cmake_minimum_required(VERSION 2.8.4)
project(Conditional)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")
ADD_LIBRARY(Conditional SHARED Conditional.cpp)

add_executable(Example1.out Example1.cpp)
target_link_libraries(Example1.out Conditional pthread)

