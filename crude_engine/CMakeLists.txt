cmake_minimum_required(VERSION 3.25)

project(crude_engine CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(crude_engine)

find_package(Vulkan REQUIRED)

target_include_directories(crude_engine PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(crude_engine PUBLIC tinygltf SDL3::SDL3 ${Vulkan_LIBRARY})

add_compile_definitions(_CRUDE_SSE_INTRINSICS _CRUDE_NO_INTRINSICS)

target_sources(crude_engine PRIVATE engine.cpp)
target_sources(crude_engine PUBLIC FILE_SET cxx_modules_engine TYPE CXX_MODULES FILES engine.ixx)

# TODO
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(math)
add_subdirectory(network)
add_subdirectory(system)
add_subdirectory(graphics)
add_subdirectory(scene)
add_subdirectory(utils)

source_group(core REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/core/*")
source_group(ecs REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/ecs/*")
source_group(math REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/math/*")
source_group(network REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/network/*")
source_group(system REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/system/*")
source_group(graphics REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/graphics/*")
source_group(scene REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/scene/*")
source_group(utils REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/utils/*")

install(TARGETS crude_engine
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
