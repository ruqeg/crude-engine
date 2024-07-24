target_sources(crude_engine
    PRIVATE
    transform.cpp
    scene.cpp
    camera.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_scene TYPE CXX_MODULES FILES
    component.ixx
    node.ixx
    transform.ixx
    mesh.ixx
    meshlet.ixx
    scene.ixx
    image.ixx
    camera.ixx)
