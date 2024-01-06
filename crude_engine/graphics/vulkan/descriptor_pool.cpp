#include <graphics/vulkan/descriptor_pool.hpp>
#include <graphics/vulkan/device.hpp>

namespace crude_engine
{

Descriptor_Pool::Descriptor_Pool(Shared_Ptr<const Device>  device,
                                 VkDescriptorPoolSize*     pPoolSizes,
                                 uint32                    poolSizeCount,
                                 uint32                    maxSets,
                                 bool                      freeDescriptorSet)
  :
  m_device(device),
  m_freeDescriptorSet(m_freeDescriptorSet)
{
  VkDescriptorPoolCreateInfo vkCreateInfo{};
  vkCreateInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  vkCreateInfo.pNext          = nullptr;
  vkCreateInfo.flags          = 0u;

  vkCreateInfo.pPoolSizes     = pPoolSizes;
  vkCreateInfo.poolSizeCount  = poolSizeCount;
  vkCreateInfo.maxSets        = maxSets;

  if (m_freeDescriptorSet)  vkCreateInfo.flags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

  VkResult result = vkCreateDescriptorPool(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, &getVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create descriptor pool");
}

Descriptor_Pool::~Descriptor_Pool()
{
  vkDestroyDescriptorPool(CRUDE_OBJECT_HANDLE(m_device), m_handle, &getVkAllocationCallbacks());
}

bool Descriptor_Pool::canFreeDescriptorSet() const
{
    return m_freeDescriptorSet;
}

}
