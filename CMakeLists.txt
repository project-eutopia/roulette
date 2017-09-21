# make      #=> build library
# make test #=> build tests executable
# make main #=> build main executable
cmake_minimum_required (VERSION 3.2.2)
project (roulette CXX)

set (CMAKE_C_COMPILER "/usr/bin/clang")
set (CMAKE_CXX_COMPILER "/usr/bin/clang++")

if (NOT CMAKE_BUILD_TYPE MATCHES DEBUG)
  add_definitions(-DNDEBUG)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Ofast -pthread" )

find_package(Boost REQUIRED)

# Include headers
include_directories (include)
include_directories ("vendor/rapidjson/include")

# Source codes (except main.cpp)
file (GLOB_RECURSE SOURCES "src/*.cpp")
list (REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Main executable
add_executable (main EXCLUDE_FROM_ALL "src/main.cpp")
target_link_libraries (main ${PROJECT_NAME})

# Google test library
# Setup on Ubuntu like (see: https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/)
#
# sudo apt-get install libgtest-dev
# sudo apt-get install cmake # install cmake
# cd /usr/src/gtest
# sudo cmake CMakeLists.txt
# sudo make
# sudo cp *.a /usr/lib
find_package(GTest REQUIRED)

# Test command
if (GTest_FOUND)
  include_directories(${GTest_INCLUDE_DIRS})

  # # NOTE: This is an example of running each test as a separate executable
  # # But we lose fine grained details about failing tests doing it this way
  # # that are typically provided by GTest
  # include (CTest)
  # enable_testing()

  # # Gets just filenames
  # file (GLOB_RECURSE test_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/test/src/" "*_test.cpp")
  # foreach(filename ${test_files})
  #   get_filename_component(test_filename "${filename}" NAME)
  #   string(REPLACE "\.cpp" "" test_name ${test_filename})

  #   get_filename_component(test_directory "${filename}" DIRECTORY)

  #   add_executable(${test_name} "${CMAKE_CURRENT_SOURCE_DIR}/test/src/${filename}")
  #   set_target_properties(${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests/${test_directory}")
  #   target_link_libraries(${test_name} ${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)
  #   add_test(NAME ${test_name} COMMAND ${test_name})
  # endforeach()

  add_executable(tests "test/tests.cpp")
  target_link_libraries(tests ${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)
endif()
