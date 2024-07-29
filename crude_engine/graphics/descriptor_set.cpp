#include <vulkan/vulkan.hpp>

module crude.graphics.descriptor_set;

import crude.graphics.vulkan_utils;
import crude.graphics.descriptor_pool;
import crude.graphics.descriptor_set_layout;
import crude.graphics.device;
import crude.core.algorithms;

namespace crude::graphics
{

Descriptor_Set::Descriptor_Set(core::shared_ptr<Descriptor_Pool> pool, core::shared_ptr<Descriptor_Set_Layout> setLayout)
  : m_pool(pool)
  , m_setLayout(setLayout)
{
  VkDescriptorSetAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  vkAllocateInfo.pNext               = nullptr;
  vkAllocateInfo.pSetLayouts         = &m_setLayout->getHandle();
  vkAllocateInfo.descriptorSetCount  = 1u;
  vkAllocateInfo.descriptorPool      = m_pool->getHandle();

  VkResult result = vkAllocateDescriptorSets(getDevice()->getHandle(), &vkAllocateInfo, &m_handle);
  vulkanHandleResult(result, "failed to allocate descriptor set");
}

Descriptor_Set::~Descriptor_Set()
{
  if (m_pool->canFreeDescriptorSet())
    vkFreeDescriptorSets(getDevice()->getHandle(), m_pool->getHandle(), 1u, &m_handle);
}

core::shared_ptr<const Device> Descriptor_Set::getDevice() noexcept 
{ 
  return m_pool->getDevice();
}

core::shared_ptr<Descriptor_Set_Layout> Descriptor_Set::getSetLayout() noexcept
{ 
  return m_setLayout;
}

core::shared_ptr<Descriptor_Pool> Descriptor_Set::getPool() noexcept 
{ 
  return m_pool; 
}

}
