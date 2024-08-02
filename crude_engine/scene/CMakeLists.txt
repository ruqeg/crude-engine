target_sources(crude_engine
    PRIVATE
    camera.cpp
    free_camera_script.cpp
    transform.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_scene TYPE CXX_MODULES FILES
    camera.ixx
    free_camera_script.ixx
    image.ixx
    mesh.ixx
    transform.ixx)
