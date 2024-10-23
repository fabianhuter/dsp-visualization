cmake_minimum_required(VERSION 3.10)
project(dsp_visualizer)

set(CMAKE_CXX_STANDARD 14)

# Add Restbed subdirectory
add_subdirectory(libs/restbed)

# Include Restbed headers
include_directories(${CMAKE_SOURCE_DIR}/libs/restbed/source)

# Add your executable
add_executable(dsp_visualizer src/main.cpp)

# Link against Restbed
target_link_libraries(dsp_visualizer restbed-static)