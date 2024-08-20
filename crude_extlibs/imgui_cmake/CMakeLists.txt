cmake_minimum_required(VERSION 3.25)

project(imgui CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(imgui)

find_package(Vulkan REQUIRED)

target_include_directories(imgui PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(imgui PUBLIC SDL3::SDL3 ${Vulkan_LIBRARY})

if(MSVC)
 target_compile_options(imgui PRIVATE "/MP")
endif()

set(IMGUI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
set(IMGUI_BACKENDS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends")

target_include_directories(imgui PRIVATE ${IMGUI_SOURCE_DIR})

target_sources(imgui PRIVATE
  ${IMGUI_SOURCE_DIR}/imgui.cpp
  ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
  ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
  ${IMGUI_BACKENDS_SOURCE_DIR}/imgui_impl_sdl3.cpp
  ${IMGUI_BACKENDS_SOURCE_DIR}/imgui_impl_vulkan.cpp)

target_sources(imgui PUBLIC 
  ${IMGUI_SOURCE_DIR}/imconfig.h 
  ${IMGUI_SOURCE_DIR}/imgui.h
  ${IMGUI_SOURCE_DIR}/imgui_internal.h 
  ${IMGUI_SOURCE_DIR}/imstb_rectpack.h 
  ${IMGUI_SOURCE_DIR}/imstb_textedit.h 
  ${IMGUI_SOURCE_DIR}/imstb_truetype.h 
  ${IMGUI_BACKENDS_SOURCE_DIR}/imgui_impl_sdl3.h
  ${IMGUI_BACKENDS_SOURCE_DIR}/imgui_impl_vulkan.h)

install(TARGETS imgui
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
