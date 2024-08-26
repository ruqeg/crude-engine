cmake_minimum_required(VERSION 3.25)

project(crude_editor CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(crude_editor WIN32)

target_link_libraries(crude_editor PUBLIC crude_engine)

if(MSVC)
 target_compile_options(crude_editor PRIVATE "/MP")
endif()

target_sources(crude_editor
  PUBLIC
  main.cpp
  application.cpp)

target_sources(crude_editor 
  PUBLIC
  FILE_SET cxx_modules_crude_editor TYPE CXX_MODULES FILES
  application.ixx)

add_subdirectory(gui)

source_group(crude_editor FILES application.cpp application.ixx main.cpp)
source_group(gui REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/gui/*")
