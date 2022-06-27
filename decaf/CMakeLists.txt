cmake_minimum_required(VERSION 3.14)

project(decaf VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    tl-expected
    URL https://github.com/TartanLlama/expected/archive/refs/tags/v1.0.0.tar.gz # Release 1.0.0
    )

FetchContent_MakeAvailable(tl-expected)

include_directories(include)

enable_testing()

add_subdirectory(src)
add_subdirectory(unittest)

