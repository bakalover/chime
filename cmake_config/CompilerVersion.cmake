set(REQUIRED_CXX_COMPILER "Clang")
set(CXX_COMPILER_MIN_VERSION 18.0)

message("Checking Clang compiler (${CMAKE_CXX_COMPILER})")

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS CXX_COMPILER_MIN_VERSION)
        message(FATAL_ERROR
                "Old version of ${REQUIRED_CXX_COMPILER} compiler: ${CMAKE_CXX_COMPILER_VERSION}, required ${CXX_COMPILER_MIN_VERSION}."
        )
        endif()