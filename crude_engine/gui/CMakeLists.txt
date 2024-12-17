target_sources(crude_engine
  PRIVATE
  imgui_helper.cpp  
)

target_sources(crude_engine
  PUBLIC
  FILE_SET cxx_modules_gui TYPE CXX_MODULES FILES
  imgui_helper.ixx  
  imgui_input_system.ixx
  imgui_demo_layout_draw_system.ixx
)
