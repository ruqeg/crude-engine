#target_sources(crude_engine
#    PRIVATE
#    model_loader.ixx)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_resources TYPE CXX_MODULES FILES
    model_loader.ixx
    image_loader.ixx)
