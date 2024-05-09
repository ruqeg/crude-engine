#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.descriptor_pool;

import crude_engine.graphics.vulkan.device;
import crude_engine.graphics.vulkan.vulkan_utils;

namespace crude_engine
{

Descriptor_Pool::Descriptor_Pool(Shared_Ptr<const Device>                   device,
                                 const span<VkDescriptorPoolSize>&  poolSizes,
                                 uint32                                     maxSets,
                                 bool                                       freeDescriptorSet)
  :
  m_device(device),
  m_freeDescriptorSet(m_freeDescriptorSet)
{
  VkDescriptorPoolCreateInfo vkCreateInfo{};
  vkCreateInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  vkCreateInfo.pNext          = nullptr;
  vkCreateInfo.flags          = 0u;

  vkCreateInfo.pPoolSizes     = poolSizes.data();
  vkCreateInfo.poolSizeCount  = poolSizes.size();
  vkCreateInfo.maxSets        = maxSets;

  if (m_freeDescriptorSet)  vkCreateInfo.flags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

  VkResult result = vkCreateDescriptorPool(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create descriptor pool");
}

Descriptor_Pool::~Descriptor_Pool()
{
  vkDestroyDescriptorPool(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

bool Descriptor_Pool::canFreeDescriptorSet() const
{
    return m_freeDescriptorSet;
}

}
