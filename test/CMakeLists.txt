set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wpedantic")

# Build Catch once as a shared lib so that we don't recompile
# it for every test (which is slow)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(catch_config_main SHARED catch_config_main.cpp)
target_link_libraries(catch_config_main)

# Tests
file(GLOB test_files "test_*")
foreach(test_file ${test_files})
    get_filename_component(test_file_name ${test_file} NAME_WE)
    add_executable(${test_file_name} ${test_file})
    target_link_libraries(${test_file_name} catch_config_main)
    add_test(${test_file_name} ${test_file_name})
endforeach()
