cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

project(HazelDev VERSION 0.0.1)

add_library(hazel-dev SHARED 
    "src/camera/orthographic.cpp"
    "src/input/input.cpp"
    "src/layer/collection.cpp"
    "src/layer/imgui.cpp"
    "src/layer/imgui-build.cpp"
    "src/platform/linux/input.cpp"
    "src/platform/linux/opengl_buffer.cpp"
    "src/platform/linux/opengl_context.cpp"
    "src/platform/linux/opengl_library.cpp"
    "src/platform/linux/opengl_log.cpp"
    "src/platform/linux/opengl_shader.cpp"
    "src/platform/linux/opengl_vertex_array.cpp"
    "src/platform/linux/window.cpp"
    "src/renderer/buffer.cpp"
    "src/renderer/command.cpp"
    "src/renderer/library.cpp"
    "src/renderer/renderer.cpp"
    "src/renderer/shader.cpp"
    "src/renderer/vertex_array.cpp"
    "src/application.cpp"
    "src/entrypoint.cpp"
    "src/logger.cpp"
)

configure_file(meta.hpp.in "${CMAKE_SOURCE_DIR}/include/hazel/meta.hpp")

target_precompile_headers(hazel-dev 
    PUBLIC
        "include/std-pch.hpp"
        "include/spdlog-pch.hpp"
)

target_include_directories(hazel-dev
    PRIVATE 
        "include/"
        "vendor/glfw/include/"
        "vendor/spdlog/include/"
        "../glm/"
        "../glad/include/"
        "../imgui/"
)

find_library(GLFW_LIB
    NAMES
        glfw3
    HINTS
        "vendor/glfw/build"
)

find_library(GLAD_LIB
    NAMES
        glad
    HINTS
        "../glad/build"
)

target_link_libraries(hazel-dev
    PRIVATE
        "${GLFW_LIB}"
        "${GLAD_LIB}"
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

add_compile_definitions(

    # Don't add OpenGL def on GLFW3
    GLFW_INCLUDE_NONE

    # Debug mode
    HAZEL_DEBUG
)
