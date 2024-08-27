module;

#include <flecs.h>

export module crude.engine;

export import crude.core.std_containers_heap;

export namespace crude::platform
{
class Input_System_Context;
}

export namespace crude
{

// !TODO
class Engine
{
public:
  struct Initialize_Window;
  struct Initialize_Systems;
public:
  static void preinitialize(core::uint32 defaultFreeRBTCapacity);
  static void postdeinitialize();
protected:
  void initialize();
  void initializeWindow(const Initialize_Window& initialize);
  void initializeSystems(const Initialize_Systems& initialize);
  void deinitialize();
  void mainLoop();
private:
  void initializeInputSystems(const Initialize_Systems& initialize);
  void initializeRendererSystems(const Initialize_Systems& initialize);
protected:
  flecs::world                                             m_world;
  flecs::system                                            m_inputSystem;
  flecs::system                                            m_rendererSystem;
  flecs::entity                                            m_sceneNode;
  core::shared_ptr<platform::Input_System_Context>         m_inputSystemCtx;
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

}