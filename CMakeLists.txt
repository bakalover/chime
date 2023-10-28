cmake_minimum_required(VERSION 3.20)
project(meijic)
set(CMAKE_CXX_STANDARD 20)

# get_filename_component(LIB_SOURSES "." ABSOLUTE)

add_subdirectory(executors)
add_subdirectory(fibers)
add_subdirectory(futures)
add_subdirectory(support)

macro(CollectCxxSources Dir OutSources)
    file(GLOB_RECURSE DirSources ${Dir}/*.cpp)
    set(${OutSources} ${${OutSources}} ${DirSources})
endmacro()

CollectCxxSources("." LIB_SOURCES)

add_library(meijic STATIC "${LIB_SOURCES}")