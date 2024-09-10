target_sources(crude_engine
  PRIVATE
  camera.cpp
  transform.cpp
)

target_sources(crude_engine
  PUBLIC
  FILE_SET cxx_modules_scene TYPE CXX_MODULES FILES
  camera.ixx
  image.ixx
  mesh.ixx
  transform.ixx
  light.ixx 
)
