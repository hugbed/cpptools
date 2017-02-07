cmake_minimum_required(VERSION 3.2)
project(tests)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

enable_testing()
FIND_PACKAGE(GTest REQUIRED)
SET(env{CTEST_OUTPUT_ON_FAILURE} TRUE)
SET(env{GTEST_COLOR} TRUE)

# Dependencies
include_directories(${cpptools_INCLUDE_DIR})
set(TEST_LIBRARIES gtest gtest_main cpptools)

file(GLOB TESTS_SOURCE
        *.cpp)
add_executable(Tests ${TESTS_SOURCE})
target_link_libraries(Tests ${TEST_LIBRARIES})
configure_file(data/data.bin data/data.bin COPYONLY)
