target_sources(crude_engine
    PRIVATE
    model_loader.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_utils TYPE CXX_MODULES FILES
    model_loader.ixx)
