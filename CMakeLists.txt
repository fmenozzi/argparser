cmake_minimum_required(VERSION 3.0)

project(argparser)

# Tests (off by default)
option(ARGPARSER_BUILD_TESTS "Build unit tests" OFF)
if(ARGPARSER_BUILD_TESTS)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    enable_testing()
    add_subdirectory(test/)
endif()
