cmake_minimum_required(VERSION 3.25)

project(crude_engine CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME})

add_compile_definitions(_CRT_SECURE_NO_DEPRECATE  _CRUDE_SSE_INTRINSICS _CRUDE_NO_INTRINSICS WIN32_LEAN_ CRUDE_ENGINE_VULKAN_WIN32)

add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(math)
add_subdirectory(network)
add_subdirectory(system)
add_subdirectory(graphics)

source_group(core REGULAR_EXPRESSION "core/*")
source_group(ecs REGULAR_EXPRESSION "ecs/*")
source_group(math REGULAR_EXPRESSION "math/*")
source_group(network REGULAR_EXPRESSION "network/*")
source_group(system REGULAR_EXPRESSION "system/*")
source_group(graphics REGULAR_EXPRESSION "graphics/*")

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})