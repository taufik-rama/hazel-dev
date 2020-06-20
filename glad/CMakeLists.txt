cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

project(GLAD)

add_library(glad SHARED 
    "src/glad.c"
)

target_include_directories(glad
    PRIVATE 
        "include/"
)
