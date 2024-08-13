cmake_minimum_required(VERSION 3.25)

project(crude_shaders)

set(CMAKE_CXX_STANDARD 20)

add_library(crude_shaders)

find_package(Vulkan REQUIRED)

set(CRUDE_SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include(${CMAKE_CURRENT_SOURCE_DIR}/compile_spirv_shader.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/compile_shader.cmake)

COMPILE_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/source/deferred_gbuffer.mesh")
COMPILE_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/source/deferred_gbuffer.frag")
COMPILE_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/source/deferred_gbuffer.task")

COMPILE_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/source/fullscreen.mesh")
COMPILE_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/source/fullscreen_pbr.frag")

set_target_properties(crude_shaders PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(crude_shaders PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

install(TARGETS crude_shaders
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
