#include <vulkan/vulkan.hpp>
#include <cmath>

module crude.gfx.vk.descriptor_pool;

import crude.gfx.vk.device;
import crude.gfx.vk.vulkan_utils;

namespace crude::gfx::vk
{

Descriptor_Pool::Descriptor_Pool(core::shared_ptr<const Device>          device,
                                 core::span<const Descriptor_Pool_Size>  poolSizes,
                                 core::uint32                            maxSets,
                                 bool                                    freeDescriptorSet)
  :
  m_device(device),
  m_freeDescriptorSet(freeDescriptorSet)
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

core::shared_ptr<const Device> Descriptor_Pool::getDevice() const
{
  return m_device;
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
