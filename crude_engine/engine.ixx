module;

#include <flecs.h>

export module crude.engine;

export import crude.core.std_containers_heap;
export import crude.graphics.renderer_deferred_gbuffer_color_pass_system;
export import crude.graphics.renderer_deferred_gbuffer_pbr_pass_system;
export import crude.graphics.renderer_fullscreen_pbr_pass_system;
export import crude.platform.input_system;
export import crude.gui.renderer_imgui_pass_system;
export import crude.resources.gltf_model_loader_system;
export import crude.resources.scene_loader_saver_system;
export import crude.graphics.renderer_point_shadow_pass_system;

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
  flecs::world                                                           m_world;
  flecs::system                                                          m_inputSystem;
  flecs::system                                                          m_rendererSystem;
  flecs::entity                                                          m_sceneNode;

  flecs::system                                                          m_lightUpdateSystem;

  flecs::system                                                          m_gltfModelLoaderSystem;
  flecs::system                                                          m_sceneSaverSystem;
  flecs::system                                                          m_sceneLoaderSystem;
  
  core::shared_ptr<resources::Scene_Loader_Context>                            m_sceneLoaderCtx;
  core::shared_ptr<resources::Scene_Saver_Context>                             m_sceneSaverCtx;
  core::shared_ptr<resources::GLTF_Model_Loader_Context>                       m_gltfModelLoaderCtx;
  core::shared_ptr<platform::Input_System_Context>                             m_inputSystemCtx;
  core::shared_ptr<graphics::Renderer_Core_System_Ctx>                         m_rendererCoreCtx;
  core::shared_ptr<graphics::Renderer_Frame_System_Ctx>                        m_rendererFrameCtx;
  core::shared_ptr<graphics::Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx>  m_rendererDeferredGBufferColorPassCtx;
  core::shared_ptr<graphics::Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx>    m_rendererDeferredGBufferPbrPassCtx;
  core::shared_ptr<graphics::Renderer_Light_Ctx>                               m_rendererLightCtx;
  core::shared_ptr<graphics::Renderer_Fullscreen_PBR_Pass_Ctx>                 m_rendererFullscreenPbrPassCtx;
  core::shared_ptr<graphics::Renderer_Point_Shadow_Pass_Systen_Ctx>            m_rendererPointShadowPassCtx;
  core::shared_ptr<gui::Renderer_ImGui_Pass_System_Ctx>                        m_rendererImguiPassCtx;
  core::shared_ptr<platform::SDL_Window_Container>                             m_windowContainer;
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