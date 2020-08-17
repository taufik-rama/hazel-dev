cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

project(Sandbox)

add_executable(sandbox 
    "src/sandbox-2d.cpp"
    "src/sandbox-app.cpp"

    # Dear ImGUI widgets implementation
    "vendor/imgui-widgets/imgui-flame-graph/imgui_widget_flamegraph.cpp"
    "vendor/imgui-widgets/widgets.cpp"
)

target_include_directories(sandbox
    PRIVATE 
        "include/"
        "vendor/"
        "../hazel-dev/include/"
        "../hazel-dev/vendor/glfw/include/"
        "../hazel-dev/vendor/spdlog/include/"
        "../glm/"
        "../glad/include/"
        "../imgui"
)

find_library(HAZEL_DEV_LIB
    NAMES
        hazel-dev
    HINTS    
        "../hazel-dev/build/"
)

target_link_libraries(sandbox
    PRIVATE
        "${HAZEL_DEV_LIB}"
)

target_compile_options(sandbox
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -pedantic
)

target_link_options(sandbox
    PRIVATE
        -no-pie # GCC will set the binary to "shared-library" if "pie" is set
)
