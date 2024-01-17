include(FetchContent)
# --------------------------------------------------------------------

FetchContent_Declare(
        wheels
        GIT_REPOSITORY https://gitlab.com/bakalover/wheels.git
        GIT_TAG 74ae096b83365f193cb8d8f75dc047fecff4df01
)
FetchContent_MakeAvailable(wheels)

# --------------------------------------------------------------------

FetchContent_Declare(
        sure
        GIT_REPOSITORY https://gitlab.com/bakalover/sure.git
        GIT_TAG 7288a84642784e1c31401a3835bdc95e9bbf3de7
)
FetchContent_MakeAvailable(sure)

#  -------------------------------------------------------------------

FetchContent_Declare(
        twist
        GIT_REPOSITORY https://gitlab.com/bakalover/twist.git
        GIT_TAG 831f73060fff8c596d50942a1b920c1215c0b220
)
FetchContent_MakeAvailable(twist)

# --------------------------------------------------------------------


FetchContent_Declare(
        expected
        GIT_REPOSITORY https://github.com/TartanLlama/expected.git
        GIT_TAG cmake_love
)
FetchContent_MakeAvailable(expected)

# --------------------------------------------------------------------
