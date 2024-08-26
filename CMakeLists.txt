cmake_minimum_required(VERSION 3.25)

project(crude CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CRUDE_EXTLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extlibs")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(crude_extlibs)
add_subdirectory(crude_shaders)
add_subdirectory(crude_engine)
# !TODO crude_test
add_subdirectory(crude_editor)
