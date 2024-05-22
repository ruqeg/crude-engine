cmake_minimum_required(VERSION 3.25)

project(crude CXX)

set(CRUDE_EXTLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extlibs")

add_subdirectory(crude_engine)
# !TODO crude_test
add_subdirectory(crude_example)
