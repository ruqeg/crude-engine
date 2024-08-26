cmake_minimum_required(VERSION 3.25)

project(crude_editor CXX)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
 target_compile_options(crude_engine PRIVATE "/MP")
endif()
 
add_executable(crude_editor WIN32)

target_link_libraries(crude_editor PUBLIC crude_engine)

target_sources(crude_editor
  PUBLIC
  source/main.cpp
  source/application.cpp)

target_sources(crude_editor 
  PUBLIC
  FILE_SET cxx_modules_crude_editor TYPE CXX_MODULES FILES
  source/application.ixx)
