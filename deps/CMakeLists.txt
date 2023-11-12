include(FetchContent)
FetchContent_Declare(
        sure
        GIT_REPOSITORY https://gitlab.com/Lipovsky/sure.git
        GIT_TAG master
)
FetchContent_MakeAvailable(sure)

FetchContent_Declare(
        wheels
        GIT_REPOSITORY https://gitlab.com/Lipovsky/wheels.git
        GIT_TAG master
)
FetchContent_MakeAvailable(wheels)

