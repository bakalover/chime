cmake_minimum_required(VERSION 3.20)
project(meijic)
set(CMAKE_CXX_STANDARD C++20)
set(CMAKE_CXX_STANDARD_REQUIRED C++20)

add_subdirectory(executors)
add_subdirectory(fibers)
add_subdirectory(futures)
add_subdirectory(primitives)