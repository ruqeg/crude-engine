#include <vulkan/vulkan.hpp>

module crude.graphics.write_descriptor_set;

import crude.graphics.descriptor_set;

namespace crude::graphics
{

Write_Descriptor_Set::Write_Descriptor_Set(core::shared_ptr<Descriptor_Set>  descriptorSet, 
                                           Uniform_Buffer_Descriptor         uniformbufferDesc)
  :
  m_descriptorSet(descriptorSet),
  m_bufferInfo(uniformbufferDesc.m_descriptoBuferInfo)
{
  this->sType             = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  this->pNext             = nullptr;
  this->dstSet            = m_descriptorSet->getHandle();
  this->dstBinding        = uniformbufferDesc.binding;
  this->dstArrayElement   = 0u;
  this->descriptorCount   = uniformbufferDesc.descriptorCount;
  this->descriptorType    = uniformbufferDesc.descriptorType;
  this->pImageInfo        = nullptr;
  this->pBufferInfo       = &m_bufferInfo;
  this->pTexelBufferView  = nullptr;
}



}