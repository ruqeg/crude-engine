#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

module crude.system.sdl_helper;

import crude.core.logger;

namespace crude::system
{

constexpr SDL_InitFlags cSDLInitFlags = SDL_INIT_VIDEO;
 
bool initializeSDL()
{
  int res = SDL_Init(cSDLInitFlags);
  if (res != 0)
  {
    core::logError(
      core::Debug::Channel::System, 
      "SDL_Manager::initialize error: %s", 
      SDL_GetError());
  }
  return (res == 0);
}
  
bool loadSDLVulkan()
{
  int res = SDL_Vulkan_LoadLibrary(nullptr);
  if (res != 0)
  {
    core::logError(
      core::Debug::Channel::System, 
      "SDL_Manager::initializeVulkan error: %s", 
      SDL_GetError());
  }
  return (res == 0);
}

void unloadSDLVulkan()
{
  SDL_Vulkan_UnloadLibrary();
}

void deinitializeSDL()
{
  if (isSDLInitialized())
  {
    SDL_Quit();
  }
}
  
bool isSDLInitialized()
{
  return SDL_WasInit(cSDLInitFlags); 
}

}
