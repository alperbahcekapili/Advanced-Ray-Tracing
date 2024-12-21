# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(RayTracer LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compile options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -ansi -g -O0 -fopenmp")

# Source files (recursively find all .cpp files under src/)
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Add Miniz source file

# Include directories for headers
include_directories(${CMAKE_SOURCE_DIR}/src/util)

# Add executable with sources and Miniz
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

# Link OpenMP if available
find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
endif()
