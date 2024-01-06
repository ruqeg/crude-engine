#include <graphics/vulkan/write_descriptor_set.hpp>
#include <graphics/vulkan/descriptor_set.hpp>

namespace crude_engine
{

Write_Descriptor_Set::Write_Descriptor_Set(std::shared_ptr<Descriptor_Set>               dscriptorSet,
                                           uint32                                        binding,
                                           uint32                                        arrayElement,
                                           uint32                                        descriptorCount,
                                           VkDescriptorType                              descriptorType,
                                           const std::optional<Descriptor_Image_Info>&   imageInfo,
                                           const std::optional<Descriptor_Buffer_Info>&  bufferInfo)
  :
  m_descriptorSet(dscriptorSet),
  m_imageInfo(imageInfo),
  m_bufferInfo(bufferInfo)
{
  this->sType             = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  this->pNext             = nullptr;
  this->dstSet            = CRUDE_VULKAN_01_HANDLE(m_descriptorSet);
  this->dstBinding        = binding;
  this->dstArrayElement   = arrayElement;
  this->descriptorCount   = descriptorCount;
  this->descriptorType    = descriptorType;
  this->pImageInfo        = CRUDE_VULKAN_01_OPTIONAL_REFVALUE_OR_NULL(m_imageInfo);
  this->pBufferInfo       = CRUDE_VULKAN_01_OPTIONAL_REFVALUE_OR_NULL(m_bufferInfo);
  this->pTexelBufferView  = nullptr;
}

}
