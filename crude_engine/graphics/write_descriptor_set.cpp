#include <vulkan/vulkan.hpp>

module crude.graphics.write_descriptor_set;

import crude.graphics.descriptor_set;

namespace crude::graphics
{

// !TODO

Write_Push_Descriptor_Set::Write_Push_Descriptor_Set()
{
  this->sType             = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  this->pNext             = nullptr;
  this->dstSet            = nullptr;
  this->dstBinding        = 0u;
  this->dstArrayElement   = 0u;
  this->descriptorCount   = 0u;
  this->descriptorType    = VK_DESCRIPTOR_TYPE_MAX_ENUM;
  this->pImageInfo        = nullptr;
  this->pBufferInfo       = nullptr;
  this->pTexelBufferView  = nullptr;
}

Write_Buffer_Push_Descriptor_Set::Write_Buffer_Push_Descriptor_Set(const Buffer_Descriptor& bufferDescriptor)
  : m_bufferInfo(bufferDescriptor.m_descriptoBuferInfo)
{
  this->dstBinding        = bufferDescriptor.binding;
  this->descriptorCount   = bufferDescriptor.descriptorCount;
  this->descriptorType    = bufferDescriptor.descriptorType;
  this->pBufferInfo       = &m_bufferInfo;
}

Write_Image_Push_Descriptor_Set::Write_Image_Push_Descriptor_Set(const Image_Descriptor& imageDescriptor)
  : m_imageInfo(imageDescriptor.m_descriptorImageInfo)
{
  this->dstBinding        = imageDescriptor.binding;
  this->descriptorCount   = imageDescriptor.descriptorCount;
  this->descriptorType    = imageDescriptor.descriptorType;
  this->pImageInfo        = &m_imageInfo;
}

Write_Descriptor_Set::Write_Descriptor_Set(core::shared_ptr<Descriptor_Set> descriptorSet)
  : m_descriptorSet(descriptorSet)
{
  this->sType             = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  this->pNext             = nullptr;
  this->dstSet            = m_descriptorSet->getHandle();
  this->dstBinding        = 0u;
  this->dstArrayElement   = 0u;
  this->descriptorCount   = 0u;
  this->descriptorType    = VK_DESCRIPTOR_TYPE_MAX_ENUM;
  this->pImageInfo        = nullptr;
  this->pBufferInfo       = nullptr;
  this->pTexelBufferView  = nullptr;
}

Write_Buffer_Descriptor_Set::Write_Buffer_Descriptor_Set(core::shared_ptr<Descriptor_Set> descriptorSet, const Buffer_Descriptor& bufferDescriptor)
  : Write_Descriptor_Set(descriptorSet), m_bufferInfo(bufferDescriptor.m_descriptoBuferInfo)
{
  this->dstBinding        = bufferDescriptor.binding;
  this->descriptorCount   = bufferDescriptor.descriptorCount;
  this->descriptorType    = bufferDescriptor.descriptorType;
  this->pBufferInfo       = &m_bufferInfo;
}

Write_Image_Descriptor_Set::Write_Image_Descriptor_Set(core::shared_ptr<Descriptor_Set> descriptorSet, const Image_Descriptor& imageDescriptor)
  : Write_Descriptor_Set(descriptorSet), m_imageInfo(imageDescriptor.m_descriptorImageInfo)
{
  this->dstBinding        = imageDescriptor.binding;
  this->descriptorCount   = imageDescriptor.descriptorCount;
  this->descriptorType    = imageDescriptor.descriptorType;
  this->pImageInfo        = &m_imageInfo;
}

}