module;

#include <flecs.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

export module crude.engine;

export import crude.core.timer;


// !TODO
import crude.core.std_containers_heap;
export namespace crude::graphics
{
class Render_Pass;
class Descriptor_Pool;
class Framebuffer;
class Command_Buffer;
}

export namespace crude
{

// !TODO
class Engine
{
public:
  static void preinitialize(core::uint32 defaultFreeRBTCapacity);
  static void postdeinitialize();
public:
  void initialize(const char* title, core::uint32 width, core::uint32 height);
  void mainLoop();
private:
  void initializeSystems();
private:
  core::Timer    m_timer;
  flecs::world   m_world;
  flecs::system  m_freeCameraUpdateSystem;
  flecs::system  m_inputSystem;
  flecs::system  m_deferredGBufferPassSystem;
  flecs::system  m_fullscreenPBRPassSystem;
  flecs::system  m_rendererFrameStartSystem;
  flecs::system  m_rendererFrameSubmitSystem;
  flecs::entity  m_sceneNode;

  core::shared_ptr<graphics::Render_Pass> m_renderPass;
  core::shared_ptr<graphics::Descriptor_Pool> m_descriptorPool;
  core::vector<core::shared_ptr<graphics::Framebuffer>> m_framebuffers;

  ImGui_ImplVulkanH_Window m_imguiWD;
};

}