cmake_minimum_required(VERSION 3.20)

#infinite loop issue

include(cmake_config/Compiler.cmake)

project(chime)
include(cmake_config/CompilerVersion.cmake)
include(cmake_config/CompilerArgs.cmake)
include(cmake_config/Sanitize.cmake)
add_subdirectory(deps)
add_subdirectory(chime)
add_subdirectory(tests)
