ProjectLog("Tests")


file(GLOB_RECURSE TEST_SOURCES "./*.cpp")

add_executable(chime_tests ${TEST_SOURCES})
target_link_libraries(chime_tests chime)
