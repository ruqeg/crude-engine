module;

#include <flecs.h>

export module crude.engine;

export import crude.core.timer;
export import crude.core.std_containers_heap;

export namespace crude
{

// !TODO
class Engine
{
public:
  struct Initialize;
  struct Initialize_Window;
  struct Initialize_Systems;
public:
  static void preinitialize(core::uint32 defaultFreeRBTCapacity);
  static void postdeinitialize();
public:
  void initialize(const Initialize& initialize);
  void deinitialize();
  void mainLoop();
private:
  void initializeWindow(const Initialize_Window& initialize);
  void initializeInputSystem(const Initialize_Systems& initialize);
  void initializeRendererSystem(const Initialize_Systems& initialize);
  void initializeRendererComponents();
protected:
  flecs::world   m_world;
  flecs::system  m_inputSystem;
  flecs::system  m_rendererSystem;
  flecs::entity  m_sceneNode;
};

struct Engine::Initialize_Window
{
  const char*   title;
  core::uint32  width;
  core::uint32  height;
};

struct Engine::Initialize_Systems
{
  core::vector<flecs::system> inputSystems;
  core::vector<flecs::system> imguiLayoutSystems;
};

struct Engine::Initialize
{
  Initialize_Window  window;
  Initialize_Systems systems;
};

}