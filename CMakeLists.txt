cmake_minimum_required (VERSION 3.5.2)
project (roulette CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

# Include headers
include_directories (include)
# Source codes (except main.cpp)
file (GLOB_RECURSE SOURCES "src/*.cpp")

# Main compile
add_executable (main ${SOURCES})
