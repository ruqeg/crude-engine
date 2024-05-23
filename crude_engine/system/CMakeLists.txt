target_sources(crude_engine
    PRIVATE
    window.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_system TYPE CXX_MODULES FILES
    window.ixx)
