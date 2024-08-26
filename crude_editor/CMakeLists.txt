cmake_minimum_required(VERSION 3.25)

project(crude_editor CXX)

set(CMAKE_CXX_STANDARD 20)
 
add_executable(crude_editor WIN32)

target_link_libraries(crude_editor PUBLIC crude_engine)

add_subdirectory(source)

if(MSVC)
 target_compile_options(crude_editor PRIVATE "/MP")
endif()
