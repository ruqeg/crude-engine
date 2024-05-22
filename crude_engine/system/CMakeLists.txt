set(EXTLIBS_DIR "../../extlibs/")

set(SDL2_INCLUDE_DIRS "${EXTLIBS_DIR}/SDL/include")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_LIBRARIES "${EXTLIBS_DIR}/SDL/lib/x64/SDL2.lib;${EXTLIBS_DIR}/SDL/lib/x64/SDL2main.lib")
else ()
  set(SDL2_LIBRARIES "${EXTLIBS_DIR}/SDL/lib/x86/SDL2.lib;${EXTLIBS_DIR}/SDL/lib/x86/SDL2main.lib")
endif ()

target_include_directories(crude_engine PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(crude_engine ${SDL2_LIBRARIES})

target_sources(crude_engine
    PUBLIC
    window.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_system TYPE CXX_MODULES FILES
    window.ixx)
