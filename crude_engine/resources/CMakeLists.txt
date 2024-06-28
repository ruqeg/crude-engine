#target_sources(crude_engine
#    PRIVATE
#    world_loader.cpp
#    image_loader.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_resources TYPE CXX_MODULES FILES
    world_loader.ixx
    image_loader.ixx)
