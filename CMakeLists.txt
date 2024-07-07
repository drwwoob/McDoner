cmake_minimum_required(VERSION 3.10)
project(McDoner)

set(CMAKE_CXX_STANDARD 11)

# Add GLFW
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Find and include GLEW
find_path(GLEW_INCLUDE_DIR GL/glew.h /usr/local/include)
find_library(GLEW_LIBRARY GLEW /usr/local/lib)
include_directories(${GLEW_INCLUDE_DIR})

# Add ImGui
include_directories(libs/imgui)
include_directories(libs/imgui/backends)
add_library(imgui
    libs/imgui/imgui.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
)

# Add OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Add the executable
add_executable(McDoner main.cpp)
target_link_libraries(McDoner glfw ${GLEW_LIBRARY} imgui ${OPENGL_LIBRARIES})
