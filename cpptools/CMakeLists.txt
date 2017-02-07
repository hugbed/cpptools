cmake_minimum_required(VERSION 3.2)

project(cpptools)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(include)
set(cpptools_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include PARENT_SCOPE)

file(GLOB SOURCE_FILES
        src/*.cpp
        include/*.h)
add_library(cpptools ${SOURCE_FILES})

add_executable(demo demo/main.cpp)
target_link_libraries(demo cpptools)