ProjectLog("Tests")

option(DTWIST_FAULTY ON)

# List sources
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# All tests target

add_executable(chime_tests ${TEST_SOURCES})
# target_link_libraries(chime_tests chime)