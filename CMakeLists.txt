# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(RayTracer LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compile options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -ansi -g -O0 -fopenmp") #  -Wall -Wextra -pedantic 

# Source files (recursively find all .cpp files under src/)
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Add Miniz source file

# Include directories for headers
include_directories(${CMAKE_SOURCE_DIR}/src/util)

# Main executable
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

# Link OpenMP if available
find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
endif()

# Add test executables
file(GLOB_RECURSE TEST_FILES /src/test/*.cpp)

# Create executables for each test file
foreach(testFile ${TEST_FILES})
    get_filename_component(testName ${testFile} NAME_WE)
    add_executable(${testName} ${testFile} ${SRC_FILES})
    # Optionally, link against any necessary libraries for tests
    target_link_libraries(${testName} OpenMP::OpenMP_CXX)
endforeach()
