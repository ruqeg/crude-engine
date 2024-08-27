target_sources(crude_editor
  PUBLIC
  imgui_editor_layout_draw_system.cpp
)
target_sources(crude_editor 
  PUBLIC
  FILE_SET cxx_modules_crude_editor_gui TYPE CXX_MODULES FILES
  imgui_editor_layout_draw_system.ixx
)
