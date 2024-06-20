target_sources(crude_engine
    PRIVATE
    camera.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_scene TYPE CXX_MODULES FILES
    image.ixx
    camera.ixx
    index.ixx
    vertex.ixx
    mesh.ixx
    model_geometry.ixx)
