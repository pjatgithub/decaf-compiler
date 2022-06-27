include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz # Release 1.11.0
    )

FetchContent_MakeAvailable(googletest)

include(GoogleTest)
add_subdirectory(decaf)

