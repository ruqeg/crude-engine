target_sources(crude_engine
    PRIVATE
    image_loader.cpp
    model_loader.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_utils TYPE CXX_MODULES FILES
    image_loader.ixx
    model_loader.ixx)
