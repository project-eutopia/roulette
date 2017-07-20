cmake_minimum_required (VERSION 3.5.2)
project (roulette CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

# Google test library
# Setup on Ubuntu like (see: https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/)
#
# sudo apt-get install libgtest-dev
# sudo apt-get install cmake # install cmake
# cd /usr/src/gtest
# sudo cmake CMakeLists.txt
# sudo make
# sudo cp *.a /usr/lib
enable_testing()
find_package(GTest REQUIRED)

# Include headers
include_directories (include)
# Source codes (except main.cpp)
file (GLOB_RECURSE SOURCES "src/*.cpp")

# Main compile
add_executable (main ${SOURCES})

# Test command
if (GTest_FOUND)
  include_directories(${GTest_INCLUDE_DIRS})
  add_executable(tests "test/tests.cpp")
  target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
endif()
