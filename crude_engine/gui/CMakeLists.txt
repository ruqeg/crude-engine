target_sources(crude_engine
  PRIVATE
  renderer_imgui_pass_system.cpp
  imgui_helper.cpp  
)

target_sources(crude_engine
  PUBLIC
  FILE_SET cxx_modules_gui TYPE CXX_MODULES FILES
  renderer_imgui_pass_system.ixx
  imgui_helper.ixx  
  imgui_input_system.ixx
  imgui_demo_layout_draw_system.ixx
)
