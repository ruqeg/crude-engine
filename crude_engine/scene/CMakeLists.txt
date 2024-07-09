target_sources(crude_engine
    PRIVATE
    world.cpp
    camera.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_scene TYPE CXX_MODULES FILES
    mesh.ixx
    meshlet.ixx
    world.ixx
    image.ixx
    camera.ixx)
