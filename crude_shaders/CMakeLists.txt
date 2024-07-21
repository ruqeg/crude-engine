cmake_minimum_required(VERSION 3.25)

project(crude_shaders)

set(CMAKE_CXX_STANDARD 20)

add_library(crude_shaders)

find_package(Vulkan REQUIRED)

include(${CMAKE_CURRENT_SOURCE_DIR}/compile_spirv_shader.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/compile_shader.cmake)

COMPILE_SPIRV_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/shader.frag")
COMPILE_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/shader.frag")
COMPILE_SPIRV_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/shader.mesh")
COMPILE_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/shader.mesh")
COMPILE_SPIRV_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/shader.frag")
COMPILE_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/shader.frag")
COMPILE_SPIRV_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/shader.task")
COMPILE_SHADER("${CMAKE_CURRENT_SOURCE_DIR}/shader.task")

set_target_properties(crude_shaders PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(crude_shaders PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
  
source_group("shaders" FILES "config.h mesh.h")

install(TARGETS crude_shaders
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
