ProjectLog("Tests")

# List sources

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# All tests target

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests meijic)