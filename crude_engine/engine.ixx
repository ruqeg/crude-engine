module;

#include <flecs.h>

export module crude.engine;

export import crude.core.timer;
export import crude.graphics.renderer;
export import crude.network.network_system;
export import crude.platform.input_system;

export namespace crude
{

struct Engine_Initialize
{
  core::uint32 defaultFreeRBTCapacity;
  core::uint32 width;
  core::uint32 height;
  const char*  title;
};

// !TODO
class Engine
{
public:
  void initialize(const Engine_Initialize& config);
  void deinitialize();
  void mainLoop();
private:
  void initializeSystems();
  void render();
private:
  core::shared_ptr<graphics::Renderer>  m_renderer;
  core::Timer                           m_timer;
  flecs::world                          m_world;
  flecs::system                         m_freeCameraUpdateSystem;
  flecs::system                         m_inputSystem;
};

}