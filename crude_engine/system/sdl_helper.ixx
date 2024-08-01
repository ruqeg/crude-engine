module;

#include <SDL3/SDL.h>

export module crude.system.sdl_helper;

export namespace crude::system
{
 
bool initializeSDL();
bool loadSDLVulkan();
void unloadSDLVulkan();
void deinitializeSDL();
bool isSDLInitialized();

}
