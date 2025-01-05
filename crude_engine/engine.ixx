module;

#include <flecs.h>

export module crude.engine;

export import crude.core.std_containers_heap;
export import crude.platform.input_system;
export import crude.resources.gltf_loader_system;
export import crude.resources.scene_loader_saver_system;
export import crude.gfx.renderer_frame;
export import crude.gfx.render_graph;

export namespace crude
{

class Engine
{
public:
  struct Initialize;
  struct Initialize_Window;
public:
  static void preinitialize(core::uint32 defaultFreeRBTCapacity);
  static void postdeinitialize();
protected:
  void initialize(const Initialize& initialize);
  void deinitialize();
  void mainLoop();
private:
  void initializeWindow(const Initialize_Window& initialize);
  void initializeSystems();
  void initializeInputSystems();
  void initializeRendererSystems();
protected:
  flecs::world                                       m_world;
  flecs::system                                      m_inputSystem;
  flecs::entity                                      m_sceneNode;
                                                     
  flecs::system                                      m_lightUpdateSystem;
                                                     
  flecs::system                                      m_gltfModelLoaderSystem;
  flecs::system                                      m_sceneSaverSystem;
  flecs::system                                      m_sceneLoaderSystem;
  
  core::shared_ptr<gfx::Render_Graph>                m_graph;
  core::shared_ptr<gfx::Renderer_Core>               m_rendererCore;
  core::shared_ptr<gfx::Renderer_Frame>              m_rendererFrame;

  core::shared_ptr<resources::Scene_Loader_Context>  m_sceneLoaderCtx;
  core::shared_ptr<resources::Scene_Saver_Context>   m_sceneSaverCtx;
  core::shared_ptr<resources::GLTF_Loader_Context>   m_gltfModelLoaderCtx;
  core::shared_ptr<platform::Input_System_Context>   m_inputSystemCtx;
  core::shared_ptr<platform::SDL_Window_Container>   m_windowContainer;
};

struct Engine::Initialize_Window
{
  const char*   title;
  core::uint32  width;
  core::uint32  height;
};

struct Engine::Initialize
{
  Initialize_Window  window;
};

}