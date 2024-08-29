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

export namespace crude::graphics
{
class Render_Pass;
class Framebuffer;
class Device;
class Descriptor_Pool;
class Renderer_Frame_System_Ctx;
}

export namespace crude::gui
{

struct Renderer_ImGui_Pass_System_Ctx
{
  graphics::Renderer_Frame_System_Ctx*                   frameCtx;
  core::vector<flecs::system>                            layoutsDrawSystems;
  core::shared_ptr<graphics::Render_Pass>                renderPass;
  core::shared_ptr<graphics::Descriptor_Pool>            descriptorPool;
  core::vector<core::shared_ptr<graphics::Framebuffer>>  framebuffers;
};

void rendererImguiPassSystemInitialize(flecs::iter& it);
void rendererImguiPassSystemDeinitialize(flecs::iter& it);
void rendererImguiPassSystemProcess(flecs::iter& it);

}

namespace crude::gui
{
void initializeRenderPass(Renderer_ImGui_Pass_System_Ctx* imguiPassCtx);
void initializeDescriptorPool(Renderer_ImGui_Pass_System_Ctx* imguiPassCtx);
void initializeFramebuffers(Renderer_ImGui_Pass_System_Ctx* imguiPassCtx);

}