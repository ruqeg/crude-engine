target_sources(crude_editor
  PUBLIC
  main.cpp
  application.cpp)

target_sources(crude_editor 
  PUBLIC
  FILE_SET cxx_modules_crude_editor TYPE CXX_MODULES FILES
  application.ixx)

add_subdirectory(gui)

#source_group(gui REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/gui/*")
