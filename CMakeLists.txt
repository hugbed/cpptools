cmake_minimum_required(VERSION 3.2)


# LIBRARY
add_subdirectory(cpptools)

# TESTS
add_subdirectory(tests)
enable_testing ()
add_test (NAME AllTests COMMAND Tests)