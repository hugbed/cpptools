cmake_minimum_required(VERSION 3.2)
project(FileRead)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(include)

# EXECUTABLE
file(GLOB SOURCE_FILES
        src/*.cpp)
add_executable(FileRead ${SOURCE_FILES})

# TESTS
enable_testing()
FIND_PACKAGE(GTest REQUIRED)
SET(env{CTEST_OUTPUT_ON_FAILURE} TRUE)
SET(env{GTEST_COLOR} TRUE)

file(GLOB TESTS_SOURCE
        tests/*.cpp)

add_executable(Tests ${TESTS_SOURCE})
target_link_libraries(Tests gtest gtest_main)
add_test(AllTests Tests)