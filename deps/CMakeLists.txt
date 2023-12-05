include(FetchContent)

FetchContent_Declare(
        sure
        GIT_REPOSITORY https://gitlab.com/bakalover/sure.git
        GIT_TAG master
)
FetchContent_MakeAvailable(sure)

FetchContent_Declare(
        wheels
        GIT_REPOSITORY https://gitlab.com/bakalover/wheels.git
        GIT_TAG master
)
FetchContent_MakeAvailable(wheels)

FetchContent_Declare(
        twist
        GIT_REPOSITORY https://gitlab.com/bakalover/twist.git
        GIT_TAG master
)
FetchContent_MakeAvailable(twist)