target_sources(crude_engine
    PRIVATE
    matrix.cpp
    scalar.cpp
    vector.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_math TYPE CXX_MODULES FILES
    constants.ixx
    fuicont.ixx
    matrix.ixx
    scalar.ixx
    vector.ixx)
