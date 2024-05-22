target_sources(crude_engine
    PRIVATE
    archetype.cpp
    component_register.cpp
    entity.cpp
    id_manager.cpp
    world.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_ecs TYPE CXX_MODULES FILES
    alias.ixx
    archetype.ixx
    component_container.ixx
    component_register.ixx
    entity.ixx
    id_manager.ixx
    query.ixx
    world.ixx)
