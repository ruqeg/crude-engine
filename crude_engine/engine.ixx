module;

#include <flecs.h>

export module crude.engine;

export import crude.core.std_containers_heap;
export import crude.platform.input_system;
export import crude.graphics.renderer_core_system;
export import crude.graphics.renderer_frame_system;
export import crude.graphics.renderer_deferred_gbuffer_pass_system;
export import crude.graphics.renderer_fullscreen_pbr_pass_system;
export import crude.gui.renderer_imgui_pass_system;

export namespace crude::platform
{
class SDL_Window_Container;
}

export namespace crude
{

struct Renderer_System_Ctx
{
  Renderer_System_Ctx();
  graphics::Renderer_Core_System_Ctx                   coreCtx;
  graphics::Renderer_Frame_System_Ctx                  frameCtx;
  graphics::Renderer_Deferred_GBuffer_Pass_Systen_Ctx  deferredGBufferPassCtx;
  graphics::Renderer_Fullscreen_PBR_Pass_Ctx           fullscreenPbrPassCtx;
  gui::Renderer_ImGui_Pass_System_Ctx                  imguiPassCtx;
};

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
protected:
  void initialize(const Initialize& initialize);
  void deinitialize();
  void mainLoop();
private:
  void initializeWindow(const Initialize_Window& initialize);
  void initializeSystems(const Initialize_Systems& initialize);
  void initializeInputSystems(const Initialize_Systems& initialize);
  void initializeRendererSystems(const Initialize_Systems& initialize);
protected:
  flecs::world                                      m_world;
  flecs::system                                     m_inputSystem;
  flecs::system                                     m_rendererSystem;
  flecs::entity                                     m_sceneNode;
  platform::Input_System_Context                    m_inputSystemCtx;
  Renderer_System_Ctx                               m_rendererSystemCtx;
  core::shared_ptr<platform::SDL_Window_Container>  m_windowContainer;
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