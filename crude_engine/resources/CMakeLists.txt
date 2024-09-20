target_sources(crude_engine
    PRIVATE
    scene_loadsave_system.cpp
    gltf_model_loader_system.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_resources TYPE CXX_MODULES FILES
    scene_loadsave_system.ixx
    gltf_model_loader_system.ixx)
