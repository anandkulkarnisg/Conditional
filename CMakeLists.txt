cmake_minimum_required(VERSION 2.8.4)
project(Conditional)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")
ADD_LIBRARY(Conditional SHARED Conditional.cpp)

add_executable(Example1.out Example1.cpp)
add_executable(Example2.out Example2.cpp)

target_link_libraries(Example1.out Conditional pthread)
target_link_libraries(Example2.out Conditional pthread)
