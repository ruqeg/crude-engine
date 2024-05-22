set(SDL2_INCLUDE_DIRS "${CRUDE_EXTLIBS_DIR}/SDL/include")
set(SDL2_LIBRARIES_DIRS "${CRUDE_EXTLIBS_DIR}/SDL/lib")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_LIBRARIES "${SDL2_LIBRARIES_DIRS}/x64/SDL2;${SDL2_LIBRARIES_DIRS}/x64/SDL2main")
else ()
  set(SDL2_LIBRARIES "${SDL2_LIBRARIES_DIRS}/x86/SDL2;${SDL2_LIBRARIES_DIRS}/x86/SDL2main")
endif ()

target_include_directories(crude_engine PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(crude_engine ${SDL2_LIBRARIES})

message("SDFSDFSDFDSS ")
target_sources(crude_engine
    PRIVATE
    window.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_system TYPE CXX_MODULES FILES
    window.ixx)
