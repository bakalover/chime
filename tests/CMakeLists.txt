ProjectLog("Tests")

# List sources

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# All tests target

add_executable(meijic_tests ${TEST_SOURCES})
target_link_libraries(meijic_tests meijic)