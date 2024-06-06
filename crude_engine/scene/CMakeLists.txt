target_sources(crude_engine
    PRIVATE
    camera.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_camera TYPE CXX_MODULES FILES
    camera.ixx)
