#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

module crude.system.sdl_system;

import crude.core.logger;

namespace crude::system
{

constexpr SDL_InitFlags cSDLInitFlags = SDL_INIT_VIDEO;
  
SDL_System& SDL_System::getInstance()
{
  static SDL_System instance;
  return instance;
}
  
SDL_System::~SDL_System()
{
  deinitialize();
  deinitializeVulkan();
}
 
bool SDL_System::initialize()
{
  int res = SDL_Init(cSDLInitFlags);
  if (res != 0)
  {
    core::logError(
      core::Debug::Channel::System, 
      "SDL_System::initialize error: %s", 
      SDL_GetError());
  }
  return (res == 0);
}
  
bool SDL_System::initializeVulkan()
{
  int res = SDL_Vulkan_LoadLibrary(nullptr);
  if (res != 0)
  {
    core::logError(
      core::Debug::Channel::System, 
      "SDL_System::initializeVulkan error: %s", 
      SDL_GetError());
  }
  return (res == 0);
}

void SDL_System::deinitializeVulkan()
{
  SDL_Vulkan_UnloadLibrary();
}

void SDL_System::deinitialize()
{
  if (isInitialized())
  {
    SDL_Quit();
  }
}
  
bool SDL_System::isInitialized() const
{
  return SDL_WasInit(cSDLInitFlags); 
}

}
