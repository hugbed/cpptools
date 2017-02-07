cmake_minimum_required(VERSION 3.2)
project(tests)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package (Threads)

FIND_PACKAGE(GTest REQUIRED)
include_directories(GTEST_INCLUDE_DIRS)
SET(env{CTEST_OUTPUT_ON_FAILURE} TRUE)
SET(env{GTEST_COLOR} TRUE)

find_package (Threads)

# Dependencies
include_directories(${cpptools_INCLUDE_DIR})
set(TEST_LIBRARIES ${GTEST_BOTH_LIBRARIES} cpptools ${CMAKE_THREAD_LIBS_INIT})

file(GLOB TESTS_SOURCE
        *.cpp)
add_executable(Tests ${TESTS_SOURCE})
target_link_libraries(Tests ${TEST_LIBRARIES})
configure_file(data/data.bin data/data.bin COPYONLY)
