cmake_minimum_required(VERSION 3.25)

project(crude_example CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(crude_example)

target_link_libraries(crude_example PRIVATE crude_engine)

target_sources(crude_example 
  PUBLIC
  basic_triangle_examle/main.cpp)
