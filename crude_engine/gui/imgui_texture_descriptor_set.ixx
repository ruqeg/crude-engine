module;

#include <vulkan/vulkan.h>

export module crude.gui.imgui_texture_descriptor_set;

export import crude.core.std_containers_heap;

export namespace crude::graphics
{
class Texture;
}

export namespace crude::gui
{

class ImGui_Texture_Descriptor_Set
{
public:
  explicit ImGui_Texture_Descriptor_Set(core::shared_ptr<graphics::Texture> texture);
  ~ImGui_Texture_Descriptor_Set();
  VkDescriptorSet getHandle();
private:
  VkDescriptorSet                            m_descriptorSetHandle;
  const core::shared_ptr<graphics::Texture>  m_texture;
};

}