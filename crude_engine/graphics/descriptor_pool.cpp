#include <vulkan/vulkan.hpp>
#include <cmath>

module crude.graphics.descriptor_pool;

import crude.graphics.device;
import crude.graphics.vulkan_utils;

namespace crude::graphics
{

Descriptor_Pool::Descriptor_Pool(core::Shared_Ptr<const Device>           device,
                                 const core::span<Descriptor_Pool_Size>&  poolSizes,
                                 bool                                     freeDescriptorSet)
  :
  m_device(device),
  m_freeDescriptorSet(m_freeDescriptorSet)
{
  core::uint32 maxSets = 0u;
  for (auto& poolSize : poolSizes)
    maxSets = std::max(poolSize.descriptorCount, maxSets);

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
