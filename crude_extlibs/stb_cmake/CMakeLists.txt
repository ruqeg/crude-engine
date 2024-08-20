cmake_minimum_required(VERSION 3.25)

project(stb CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(stb INTERFACE)

set(STB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stb")

target_include_directories(stb INTERFACE 
  ${STB_SOURCE_DIR}/stb_image.h) 

install(TARGETS stb
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
