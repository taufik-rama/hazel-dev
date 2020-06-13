cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

project(HazelDev VERSION 0.0.1)

add_library(hazel-dev SHARED src/application.cpp src/entrypoint.cpp src/logger.cpp)

configure_file(meta.hpp.in ${CMAKE_SOURCE_DIR}/include/meta.hpp)

target_include_directories(hazel-dev PRIVATE include/ vendor/spdlog/include/)

target_compile_options(hazel-dev PRIVATE -Wall -Wextra -pedantic -Werror)
