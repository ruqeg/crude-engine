module;

#include <functional>
#include <flecs.h>
#include <vulkan/vulkan.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

export module crude.gui.imgui_renderer_pass_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gui.imgui_layout_component;

export namespace crude::graphics
{
class Render_Pass;
class Framebuffer;
class Device;
class Descriptor_Pool;
class Renderer_Core_Component;
}

export namespace crude::gui
{

struct ImGui_Renderer_Component
{
  core::shared_ptr<graphics::Render_Pass>               renderPass;
  core::shared_ptr<graphics::Descriptor_Pool>           descriptorPool;
  core::vector<core::shared_ptr<graphics::Framebuffer>> framebuffers;
};

void imguiRendererPassSystemInitialize(flecs::iter& it);
void imguiRendererPassSystemDeinitialize(flecs::iter& it);
void imguiRendererPassSystemProcess(flecs::iter& it);

}

namespace crude::gui
{
void imguiRendererPassSystemInitializeRenderPass(ImGui_Renderer_Component* imguiRendererComponent, graphics::Renderer_Core_Component* rendererCoreComponent);
void imguiRendererPassSystemInitializeDescriptorPool(ImGui_Renderer_Component* imguiRendererComponent, graphics::Renderer_Core_Component* rendererCoreComponent);
void imguiRendererPassSystemInitializeFramebuffers(ImGui_Renderer_Component* imguiRendererComponent, graphics::Renderer_Core_Component* rendererCoreComponent);

}