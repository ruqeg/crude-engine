target_sources(crude_engine
    PRIVATE
    node.cpp
    transform.cpp
    scene.cpp
    camera.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_scene TYPE CXX_MODULES FILES
    node.ixx
    transform.ixx
    mesh.ixx
    scene.ixx
    image.ixx
    camera.ixx)
