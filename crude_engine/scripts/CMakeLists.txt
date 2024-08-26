target_sources(crude_engine
  PRIVATE
  free_camera_script.cpp
)

target_sources(crude_engine
  PUBLIC
  FILE_SET cxx_modules_scripts TYPE CXX_MODULES FILES
  window_script.ixx
  free_camera_script.ixx
)
