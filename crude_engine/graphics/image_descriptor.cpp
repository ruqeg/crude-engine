#include <vulkan/vulkan.hpp>

module crude.graphics.image_descriptor;

import crude.graphics.image;
import crude.graphics.image_view;
import crude.graphics.sampler;

namespace crude::graphics
{

Image_Descriptor::Image_Descriptor(VkDescriptorType descriptorType, core::uint32 binding, VkShaderStageFlags stageFlags) noexcept
  :
  Descriptor_Set_Layout_Binding(binding, descriptorType, 1u, stageFlags)
{}

void Image_Descriptor::update(core::shared_ptr<const Image_View> imageView, core::shared_ptr<const Sampler> sampler) noexcept
{
  m_descriptorImageInfo.sampler = sampler->getHandle();
  m_descriptorImageInfo.imageView = imageView->getHandle();
  m_descriptorImageInfo.imageLayout = imageView->getImage()->getLayout();
}


Combined_Image_Sampler_Descriptor::Combined_Image_Sampler_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags) noexcept
  : Image_Descriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding, stageFlags) 
{}

}