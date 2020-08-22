cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

project(app)

add_executable(app 
    "src/app.cpp"
    "src/layer.cpp"
    "src/level.cpp"
    "src/player.cpp"
)

target_include_directories(app
    PRIVATE 
        "include/"

        # Hazel API
        "../hazel-dev/include/"
        "../hazel-dev/vendor/spdlog/include/"

        # Math
        "../glm/"

        # Dear ImGUI
        "../imgui"
)

find_library(HAZEL_DEV_LIB
    NAMES
        hazel-dev
    HINTS    
        "../hazel-dev/build/"
)

target_link_libraries(app
    PRIVATE
        "${HAZEL_DEV_LIB}"
)

target_compile_options(app
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -pedantic
)

target_link_options(app
    PRIVATE
        -no-pie # GCC will set the binary to "shared-library" if "pie" is set
)

add_compile_definitions(

    # Debug mode
    HAZEL_DEBUG
)