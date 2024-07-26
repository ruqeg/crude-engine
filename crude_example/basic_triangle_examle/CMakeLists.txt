target_link_libraries(crude_example PUBLIC crude_engine)

target_sources(crude_example 
  PUBLIC
  main.cpp
  application.cpp)

target_sources(crude_example 
  PUBLIC
  FILE_SET cxx_modules_example TYPE CXX_MODULES FILES
  application.ixx)
