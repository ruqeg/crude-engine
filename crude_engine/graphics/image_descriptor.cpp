#include <vulkan/vulkan.hpp>

module crude.graphics.image_descriptor;

import crude.graphics.image;
import crude.graphics.image_view;
import crude.graphics.sampler;
import crude.core.logger;

namespace crude::graphics
{

Image_Descriptor::Image_Descriptor(VkDescriptorType descriptorType, core::uint32 binding, VkShaderStageFlags stageFlags) noexcept
  :
  Descriptor_Set_Layout_Binding(binding, descriptorType, 1u, stageFlags)
{}

void Image_Descriptor::write(VkWriteDescriptorSet& writeDescriptorSet)
{
  writeEmpty(writeDescriptorSet);
  writeDescriptorSet.dstBinding      = this->binding;
  writeDescriptorSet.descriptorCount = this->descriptorCount;
  writeDescriptorSet.descriptorType  = this->descriptorType;
  writeDescriptorSet.pImageInfo      = &m_descriptorImageInfo;
}

bool Image_Descriptor::update(core::shared_ptr<const Image_View> imageView, core::shared_ptr<const Sampler> sampler) noexcept
{
  Image_Subresource_Range range = imageView->getSubresourceRange();
  core::shared_ptr<const Image> image = imageView->getImage();
  VkImageLayout baseLayout = image->getMipLayout(range.baseMipLevel);
  for (core::uint32 mipLevel = range.baseMipLevel + 1; mipLevel < range.baseMipLevel + range.levelCount; ++mipLevel)
  {
    if (image->getMipLayout(mipLevel) != baseLayout)
    {
      core::logError(core::Debug::Channel::Graphics, "Mips have different image layout");
      return false;
    }
  }

  m_descriptorImageInfo.sampler = sampler->getHandle();
  m_descriptorImageInfo.imageView = imageView->getHandle();
  m_descriptorImageInfo.imageLayout = baseLayout;
  return true;
}


Combined_Image_Sampler_Descriptor::Combined_Image_Sampler_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags) noexcept
  : Image_Descriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding, stageFlags) 
{}

}