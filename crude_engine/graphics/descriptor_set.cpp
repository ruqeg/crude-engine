#include <vulkan/vulkan.hpp>

module crude.graphics.descriptor_set;

import crude.graphics.vulkan_utils;
import crude.graphics.descriptor_pool;
import crude.graphics.descriptor_set_layout;
import crude.graphics.device;
import crude.core.algorithms;

namespace crude::graphics
{

Descriptor_Set::Descriptor_Set(core::shared_ptr<Descriptor_Pool> pool)
  : Descriptor_Set(pool, {})
{}

Descriptor_Set::Descriptor_Set(core::shared_ptr<Descriptor_Pool> pool, const core::vector<core::shared_ptr<Descriptor_Set_Layout>>& setLayouts)
  : m_pool(pool)
  , m_setLayouts(setLayouts)
{
  core::vector<VkDescriptorSetLayout> setLayoutHandles(setLayouts.begin(), setLayouts.end());
  VkDescriptorSetAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  vkAllocateInfo.pNext               = nullptr;
  vkAllocateInfo.pSetLayouts         = setLayoutHandles.data();
  vkAllocateInfo.descriptorSetCount  = m_setLayouts.size();
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

const core::vector<core::shared_ptr<Descriptor_Set_Layout>>& Descriptor_Set::getSetLayouts() noexcept
{ 
  return m_setLayouts;
}

core::shared_ptr<Descriptor_Pool> Descriptor_Set::getPool() noexcept 
{ 
  return m_pool; 
}

}
