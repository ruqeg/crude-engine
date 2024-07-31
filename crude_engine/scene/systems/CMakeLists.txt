target_sources(crude_engine
    PRIVATE
    free_camera.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_scene TYPE CXX_MODULES FILES
    free_camera.ixx)
