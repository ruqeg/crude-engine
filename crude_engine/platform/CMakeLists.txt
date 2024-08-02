target_sources(crude_engine
    PRIVATE
    input_system.cpp
    sdl_window_container.cpp
    sdl_helper.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_platform TYPE CXX_MODULES FILES
    input_system.ixx
    sdl_window_container.ixx
    sdl_helper.ixx)
