module;

#include <functional>
#include <flecs.h>
#include <vulkan/vulkan.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

export module crude.gui.renderer_imgui_pass_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.renderer_frame_system;

export namespace crude::gfx::vk
{

class Render_Pass;
class Framebuffer;
class Device;
class Descriptor_Pool;

}

export namespace crude::gfx
{

class Renderer_Frame_System_Ctx;

}

export namespace crude::gui
{

struct Renderer_ImGui_Pass_System_Ctx
{
public:
  explicit Renderer_ImGui_Pass_System_Ctx(core::shared_ptr<gfx::Renderer_Frame_System_Ctx> frameCtx);
  ~Renderer_ImGui_Pass_System_Ctx();
  core::shared_ptr<gfx::Renderer_Frame_System_Ctx>      frameCtx;
  core::vector<flecs::system>                           layoutsDrawSystems;
  core::shared_ptr<gfx::vk::Render_Pass>                renderPass;
  core::shared_ptr<gfx::vk::Descriptor_Pool>            descriptorPool;
  core::vector<core::shared_ptr<gfx::vk::Framebuffer>>  framebuffers;
private:
  void initializeRenderPass();
  void initializeDescriptorPool();
  void initializeFramebuffers();
};

void rendererImguiPassSystemProcess(flecs::iter& it);

}