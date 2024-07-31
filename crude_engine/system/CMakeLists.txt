target_sources(crude_engine
    PRIVATE
    sdl_window_container.cpp
    sdl_system.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_system TYPE CXX_MODULES FILES
    sdl_window_container.ixx
    sdl_system.ixx)
