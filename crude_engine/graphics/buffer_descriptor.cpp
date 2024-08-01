#include <vulkan/vulkan.hpp>

module crude.graphics.buffer_descriptor;

import crude.graphics.storage_buffer;
import crude.graphics.uniform_buffer;
import crude.graphics.storage_buffer;

namespace crude::graphics
{

Buffer_Descriptor::Buffer_Descriptor(core::uint32 binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags)
  : Descriptor_Set_Layout_Binding(binding, descriptorType, 1u, stageFlags) 
{}

void Buffer_Descriptor::write(VkWriteDescriptorSet& writeDescriptorSet)
{
  writeEmpty(writeDescriptorSet);
  writeDescriptorSet.dstBinding      = this->binding;
  writeDescriptorSet.descriptorCount = this->descriptorCount;
  writeDescriptorSet.descriptorType  = this->descriptorType;
  writeDescriptorSet.pBufferInfo     = &m_descriptoBuferInfo;
}

void Buffer_Descriptor::updateBase(core::shared_ptr<const Buffer> buffer, VkDeviceSize range, VkDeviceSize offset)
{
  m_descriptoBuferInfo.buffer = buffer->getHandle();
  m_descriptoBuferInfo.offset = offset;
  m_descriptoBuferInfo.range = range;
}

Storage_Buffer_Descriptor::Storage_Buffer_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags)
  : Buffer_Descriptor(binding, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, stageFlags) 
{}

void Storage_Buffer_Descriptor::update(core::shared_ptr<Storage_Buffer> buffer, VkDeviceSize range, VkDeviceSize offset)
{
  updateBase(buffer, range, offset);
}

Uniform_Buffer_Descriptor::Uniform_Buffer_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags)
  : Buffer_Descriptor(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, stageFlags) 
{}

}