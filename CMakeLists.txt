cmake_minimum_required(VERSION 3.26)
project(fightan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRECTORIES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(third_party)
add_subdirectory(src)