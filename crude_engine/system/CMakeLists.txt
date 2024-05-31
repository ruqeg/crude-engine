target_sources(crude_engine
    PRIVATE
    mouse_event_handler.cpp
    keyboard_event_handler.cpp
    window_event_handler.cpp
    sdl_io_manager.cpp
    sdl_window_container.cpp
    sdl_system.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_system TYPE CXX_MODULES FILES
    mouse_event_handler.ixx
    keyboard_event_handler.ixx
    window_event_handler.ixx
    sdl_io_manager.ixx
    window_event.ixx
    mouse_event.ixx
    keyboard_event.ixx
    sdl_window_container.ixx
    sdl_system.ixx)
