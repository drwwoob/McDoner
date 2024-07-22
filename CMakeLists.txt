cmake_minimum_required(VERSION 3.10)
project(McDoner)

set(CMAKE_CXX_STANDARD 17)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# SDL2_image
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/backends)

# Mac RPath fix
set(CMAKE_BUILD_RPATH /Library/Frameworks)

include_directories(${IMGUI_DIR} ${IMGUI_BACKEND_DIR})

file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_sdl2.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp
)

# Add executable
add_executable(${PROJECT_NAME} main.cpp ${IMGUI_SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${OPENGL_LIBRARIES}
)

# Explicitly link SDL2_image library (for diagnostic purposes)
find_library(SDL2_IMAGE_LIB NAMES SDL2_image)
message(STATUS "Found SDL2_image library: ${SDL2_IMAGE_LIB}")
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIB})
