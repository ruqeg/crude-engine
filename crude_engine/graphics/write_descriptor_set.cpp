#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.write_descriptor_set;

import crude.graphics.vulkan.descriptor_set;

namespace crude::graphics
{

Write_Descriptor_Set::Write_Descriptor_Set(core::Shared_Ptr<Descriptor_Set>               descriptorSet,
                                           core::uint32                                   binding,
                                           core::uint32                                   arrayElement,
                                           core::uint32                                   descriptorCount,
                                           VkDescriptorType                               descriptorType,
                                           const core::Optional<Descriptor_Image_Info>&   imageInfo,
                                           const core::Optional<Descriptor_Buffer_Info>&  bufferInfo)
  :
  m_descriptorSet(descriptorSet),
  m_imageInfo(imageInfo),
  m_bufferInfo(bufferInfo)
{
  this->sType             = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  this->pNext             = nullptr;
  this->dstSet            = m_descriptorSet->getHandle();
  this->dstBinding        = binding;
  this->dstArrayElement   = arrayElement;
  this->descriptorCount   = descriptorCount;
  this->descriptorType    = descriptorType;
  //!===============
  //! TODO fix unsafe
  this->pImageInfo        = m_imageInfo.hasValue() ? (&m_imageInfo.value()) : nullptr;
  this->pBufferInfo       = m_bufferInfo.hasValue() ? (&m_bufferInfo.value()) : nullptr;
  //!===============
  this->pTexelBufferView  = nullptr;
}

}