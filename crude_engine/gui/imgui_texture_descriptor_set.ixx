module;

#include <vulkan/vulkan.h>

export module crude.gui.imgui_texture_descriptor_set;

export import crude.core.std_containers_heap;
export import crude.gui.renderer_imgui_pass_system;

export namespace crude::gfx
{
class Texture;
}

export namespace crude::gui
{

class ImGui_Texture_Descriptor_Set
{
public:
  explicit ImGui_Texture_Descriptor_Set(core::shared_ptr<Renderer_ImGui_Pass_System_Ctx> rendererImguiPassContext, core::shared_ptr<gfx::Texture> texture);
  ~ImGui_Texture_Descriptor_Set();
  VkDescriptorSet getHandle();
private:
  core::shared_ptr<Renderer_ImGui_Pass_System_Ctx>  m_rendererImguiPassContext;
  VkDescriptorSet                                   m_descriptorSetHandle;
  const core::shared_ptr<gfx::Texture>              m_texture;
};

}