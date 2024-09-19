target_sources(crude_editor
  PUBLIC
  scene_file_io.cpp
)

target_sources(crude_editor 
  PUBLIC
  FILE_SET cxx_modules_crude_editor_resources TYPE CXX_MODULES FILES
  scene_file_io.ixx
)
