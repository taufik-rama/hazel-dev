cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

project(Sandbox)

add_executable(sandbox src/sandbox-app.cpp)

find_library(HAZEL_DEV_LIB hazel-dev ../hazel-dev/build/)

target_include_directories(sandbox PRIVATE ../hazel-dev/include/)

target_link_libraries(sandbox PRIVATE "${HAZEL_DEV_LIB}")

target_compile_options(sandbox PRIVATE -Wall -Wextra -pedantic -Werror)
