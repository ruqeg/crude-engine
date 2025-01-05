target_sources(crude_engine
    PRIVATE
    scene_loader_saver_system.cpp
    gltf_loader_system.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_resources TYPE CXX_MODULES FILES
    scene_loader_saver_system.ixx
    gltf_loader_system.ixx)
