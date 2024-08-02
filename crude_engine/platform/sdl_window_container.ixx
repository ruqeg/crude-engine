module;

#include <SDL3/SDL.h>
#include <memory>

export module crude.platform.sdl_window_container;

import crude.core.alias;

export namespace crude::platform
{

enum SDL_Window_Container_Flag
{
  SDL_WINDOW_CONTAINER_FLAG_VULKAN = SDL_WINDOW_VULKAN
};

class SDL_Window_Container
{
public:
  SDL_Window_Container(const char* title, core::int32 width, core::int32 height, core::uint32 flags);
  ~SDL_Window_Container() = default;

  bool isValid() const { return m_window.get(); }
  std::shared_ptr<SDL_Window> getWindow() { return m_window; }
  std::shared_ptr<const SDL_Window> getWindow() const { return m_window; }
  
  core::uint32 getWidth() const;
  core::uint32 getHeight() const;
  core::float32 getAspect() const;

private:
  std::shared_ptr<SDL_Window> m_window;
};

}
