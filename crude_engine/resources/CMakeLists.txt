target_sources(crude_engine
    PRIVATE
    gltf_loader.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_resources TYPE CXX_MODULES FILES
    gltf_loader.ixx
    image_loader.ixx)
