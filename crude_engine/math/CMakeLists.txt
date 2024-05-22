target_sources(${PROJECT_NAME}
    PUBLIC
    matrix.cpp
    scalar.cpp
    vector.cpp)

target_sources(${PROJECT_NAME}
    PUBLIC
    FILE_SET cxx_modules_math TYPE CXX_MODULES FILES
    constants.ixx
    fuicont.ixx
    matrix.ixx
    scalar.ixx
    vector.ixx)
