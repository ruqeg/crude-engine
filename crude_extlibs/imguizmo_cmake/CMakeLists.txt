cmake_minimum_required(VERSION 3.25)

project(imguizmo CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(imguizmo)

find_package(Vulkan REQUIRED)

target_link_libraries(imguizmo PRIVATE imgui)

if(MSVC)
 target_compile_options(imguizmo PRIVATE "/MP")
endif()

set(IMGUIZMO_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo")

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${IMGUIZMO_SOURCE_DIR})

target_sources(imguizmo PRIVATE
  ${IMGUIZMO_SOURCE_DIR}/GraphEditor.cpp
  ${IMGUIZMO_SOURCE_DIR}/GraphEditor.h
  ${IMGUIZMO_SOURCE_DIR}/ImCurveEdit.cpp
  ${IMGUIZMO_SOURCE_DIR}/ImCurveEdit.h
  ${IMGUIZMO_SOURCE_DIR}/ImGradient.cpp
  ${IMGUIZMO_SOURCE_DIR}/ImGradient.h
  ${IMGUIZMO_SOURCE_DIR}/ImGuizmo.cpp
  ${IMGUIZMO_SOURCE_DIR}/ImGuizmo.h
  ${IMGUIZMO_SOURCE_DIR}/ImSequencer.cpp
  ${IMGUIZMO_SOURCE_DIR}/ImSequencer.h
  ${IMGUIZMO_SOURCE_DIR}/ImZoomSlider.h
)

install(TARGETS imguizmo
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
