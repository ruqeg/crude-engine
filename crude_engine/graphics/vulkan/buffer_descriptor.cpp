#include <vulkan/vulkan.hpp>

module crude.gfx.vk.buffer_descriptor;

import crude.gfx.vk.storage_buffer;
import crude.gfx.vk.uniform_buffer;
import crude.gfx.vk.storage_buffer;

namespace crude::gfx::vk
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
  m_descriptoBuferInfo.range  = range;
}

void Buffer_Descriptor::updateBase(core::shared_ptr<const Buffer> buffer)
{
  updateBase(buffer, buffer->getSize(), 0);
}

void Buffer_Descriptor::clear()
{
  m_descriptoBuferInfo.buffer = VK_NULL_HANDLE;
  m_descriptoBuferInfo.offset = 0u;
  m_descriptoBuferInfo.range  = VK_WHOLE_SIZE;
}

Storage_Buffer_Descriptor::Storage_Buffer_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags)
  : Buffer_Descriptor(binding, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, stageFlags) 
{}

void Storage_Buffer_Descriptor::update(core::shared_ptr<Storage_Buffer> buffer)
{
  update(buffer, buffer->getSize(), 0u);
}

void Storage_Buffer_Descriptor::update(core::shared_ptr<Storage_Buffer> buffer, VkDeviceSize range, VkDeviceSize offset)
{
  updateBase(buffer, range, offset);
}

Uniform_Buffer_Descriptor::Uniform_Buffer_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags)
  : Buffer_Descriptor(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, stageFlags) 
{}

}