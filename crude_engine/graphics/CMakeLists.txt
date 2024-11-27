add_subdirectory(vk)

target_sources(crude_engine
  PRIVATE
  gbuffer.cpp
  mesh_buffer.cpp
  render_graph.cpp
  renderer_core_system.cpp
  renderer_deferred_gbuffer_color_pass_system.cpp
  renderer_deferred_gbuffer_pbr_pass_system.cpp
  renderer_frame_system.cpp
  renderer_fullscreen_pbr_pass_system.cpp
  renderer_light_system.cpp
  renderer_point_shadow_pass_system.cpp
  texture.cpp
)

target_sources(crude_engine
  PUBLIC
  FILE_SET cxx_modules_graphics TYPE CXX_MODULES FILES
  camera_ubo.ixx
  gbuffer.ixx
  material.ixx
  mesh_buffer.ixx
  render_graph.ixx
  renderer_core_system.ixx
  renderer_deferred_gbuffer_color_pass_system.ixx
  renderer_deferred_gbuffer_pbr_pass_system.ixx
  renderer_frame_system.ixx
  renderer_fullscreen_pbr_pass_system.ixx
  renderer_light_system.ixx
  renderer_point_shadow_pass_system.ixx
  texture.ixx
)
