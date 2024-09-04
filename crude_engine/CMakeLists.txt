cmake_minimum_required(VERSION 3.25)

project(crude_engine CXX)

add_library(crude_engine)

find_package(Vulkan REQUIRED)

target_include_directories(crude_engine PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(crude_engine PUBLIC stb imgui flecs DirectXMath meshoptimizer tinygltf SDL3::SDL3 ${Vulkan_LIBRARY})
get_target_property(CRUDE_SHADERS_INCLUDE crude_shaders INCLUDE_DIRECTORIES)
target_include_directories(crude_engine PUBLIC ${CRUDE_SHADERS_INCLUDE})

if(MSVC)
 target_compile_options(crude_engine PRIVATE "/MP")
endif()

add_compile_definitions(_CRUDE_SSE_INTRINSICS _CRUDE_NO_INTRINSICS)

target_sources(crude_engine PRIVATE engine.cpp)
target_sources(crude_engine PUBLIC FILE_SET cxx_modules_engine TYPE CXX_MODULES FILES engine.ixx)

add_subdirectory(core)
add_subdirectory(network)
add_subdirectory(platform)
add_subdirectory(graphics)
add_subdirectory(scene)
add_subdirectory(resources)
add_subdirectory(gui)
add_subdirectory(scripts)

source_group(core REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/core/*")
source_group(network REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/network/*")
source_group(platform REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/platform/*")
source_group(graphics REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/graphics/*")
source_group(scene REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/scene/*")
source_group(resources REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")
source_group(gui REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/gui/*")
source_group(scripts REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/scripts/*")
source_group(crude_engine FILES engine.cpp engine.ixx)

install(TARGETS crude_engine
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
