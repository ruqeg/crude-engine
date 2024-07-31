target_sources(crude_engine
    PRIVATE
    transform.cpp
    camera.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_scene TYPE CXX_MODULES FILES
    transform.ixx
    mesh.ixx
    camera.ixx)
