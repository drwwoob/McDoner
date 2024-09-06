cmake_minimum_required(VERSION 3.10)
project(McDoner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/backends)

# other libs
set(OTHER_LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)
set(STRUCT_LIBS_DIR ${CMAKE_SOURCE_DIR}/src/structures)

set(CMAKE_BUILD_RPATH /Library/Frameworks)

# Include directories
include_directories(${IMGUI_DIR} ${IMGUI_BACKEND_DIR} ${OTHER_LIBS_DIR} ${STRUCT_LIBS_DIR})

# Collect source files
file(GLOB LIB_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_sdl2.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp
    ${OTHER_LIBS_DIR}/*.cpp
    ${STRUCT_LIBS_DIR}/*.cpp
)

# Add executable
add_executable(${PROJECT_NAME} main.cpp ${LIB_SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2
    SDL2_image::SDL2_image
    OpenGL::GL
    nlohmann_json::nlohmann_json
)
