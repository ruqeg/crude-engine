add_subdirectory(vulkan)

target_sources(crude_engine
  PRIVATE
  gbuffer_pass.cpp
  gbuffer.cpp
  mesh_buffer.cpp
  pipeline_manager.cpp
  render_graph.cpp
  renderer_core.cpp
  renderer_frame.cpp
  texture.cpp
)

target_sources(crude_engine
  PUBLIC
  FILE_SET cxx_modules_graphics TYPE CXX_MODULES FILES
  gbuffer_pass.ixx
  camera_ubo.ixx
  gbuffer.ixx
  material.ixx
  mesh_buffer.ixx
  pipeline_manager.ixx
  render_graph.ixx
  renderer_core.ixx
  renderer_frame.ixx
  texture.ixx
)
