#include <graphics/vulkan/write_descriptor_set.hpp>
#include <graphics/vulkan/descriptor_set.hpp>

namespace crude_engine
{

Write_Descriptor_Set::Write_Descriptor_Set(Shared_Ptr<Descriptor_Set>              descriptorSet,
                                          uint32                                   binding,
                                          uint32                                   arrayElement,
                                          uint32                                   descriptorCount,
                                          VkDescriptorType                         descriptorType,
                                          const Optional<Descriptor_Image_Info>&   imageInfo,
                                          const Optional<Descriptor_Buffer_Info>&  bufferInfo)
  :
  m_descriptorSet(descriptorSet),
  m_imageInfo(imageInfo),
  m_bufferInfo(bufferInfo)
{
  this->sType             = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  this->pNext             = nullptr;
  this->dstSet            = CRUDE_OBJECT_HANDLE(m_descriptorSet);
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
