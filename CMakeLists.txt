cmake_minimum_required(VERSION 3.20)

#infinite loop issue

include(cmake_config/Compiler.cmake)

project(chime)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cmake_config/CompilerVersion.cmake)
include(cmake_config/CompilerArgs.cmake)
include(cmake_config/Sanitize.cmake)
add_subdirectory(deps)
add_subdirectory(chime)
add_subdirectory(tests)
