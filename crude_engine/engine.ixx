module;

#include <flecs.h>

export module crude.engine;

export import crude.system.sdl_system;
export import crude.core.memory_manager;
export import crude.core.timer;
export import crude.graphics.renderer;
export import crude.network.network_system;

export namespace crude
{

struct Engine_Config
{
  core::uint32 defaultFreeRBTCapacity;
};

// !TODO
class Engine
{
public:
  Engine(core::shared_ptr<system::SDL_Window_Container> windowContainer);
public:
  void mainLoop();
  void updateEvent();
  void update(core::float64 elapsed);
  void render();
public:
  static void initialize(const Engine_Config& config);
private:
  static void initializeMemory(core::uint32 defaultFreeRBTCapacity);
  static void initalizeSystem();
  static void initalizeNetwork();
protected:
  core::shared_ptr<graphics::Renderer>  m_renderer;
  core::Timer                           m_timer;
  flecs::world                          m_world;
  bool                                  m_quit = false;
};

}