#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

module crude.gui.imgui_texture_descriptor_set;

import crude.graphics.sampler;
import crude.graphics.image;
import crude.graphics.image_view;
import crude.graphics.texture;

namespace crude::gui
{

ImGui_Texture_Descriptor_Set::ImGui_Texture_Descriptor_Set(core::shared_ptr<Renderer_ImGui_Pass_System_Ctx> rendererImguiPassContext, core::shared_ptr<graphics::Texture> texture)
  : m_texture{ texture }
  , m_rendererImguiPassContext{ rendererImguiPassContext }
{
  m_descriptorSetHandle = ImGui_ImplVulkan_AddTexture(
    m_texture->getSampler()->getHandle(),
    m_texture->getImageView()->getHandle(),
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

ImGui_Texture_Descriptor_Set::~ImGui_Texture_Descriptor_Set()
{
  ImGui_ImplVulkan_RemoveTexture(m_descriptorSetHandle);
}

VkDescriptorSet ImGui_Texture_Descriptor_Set::getHandle()
{
  return m_descriptorSetHandle;
}

}