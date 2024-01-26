ProjectLog("Tests")


file(GLOB_RECURSE UNIT_TEST_SOURCES "./unit/*.cpp")
file(GLOB_RECURSE STRESS_TEST_SOURCES "./stress/*.cpp")

add_executable(chime_unit_tests ${UNIT_TEST_SOURCES})
add_executable(chime_stress_tests ${STRESS_TEST_SOURCES})

target_link_libraries(chime_unit_tests chime)
target_link_libraries(chime_stress_tests chime)
