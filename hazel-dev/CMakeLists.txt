cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

project(HazelDev VERSION 0.0.1)

add_library(hazel-dev SHARED 
    src/application.cpp 
    src/entrypoint.cpp 
    src/logger.cpp
    src/platform/linux/window.cpp
)

configure_file(meta.hpp.in ${CMAKE_SOURCE_DIR}/include/hazel/meta.hpp)

target_precompile_headers(hazel-dev 
    PUBLIC
        include/std-pch.hpp
        include/spdlog-pch.hpp
)

target_include_directories(hazel-dev
    PRIVATE 
        include/
        vendor/glfw/include/
        vendor/spdlog/include/
)

find_library(GLFW_LIB
    NAMES
        glfw3
    HINTS
        "vendor/glfw/build"
)

target_link_libraries(hazel-dev
    PRIVATE
        "${GLFW_LIB}"
        GL
        X11
        pthread
)

target_compile_options(hazel-dev
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -pedantic
)
