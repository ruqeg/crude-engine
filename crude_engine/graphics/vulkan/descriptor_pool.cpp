#include <graphics/vulkan/descriptor_pool.hpp>
#include <graphics/vulkan/device.hpp>

namespace crude_engine
{

Descriptor_Pool::Descriptor_Pool(Shared_Ptr<const Device>                   device,
                                 const Array_Unsafe<VkDescriptorPoolSize>&  poolSizes,
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

  VkResult result = vkCreateDescriptorPool(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create descriptor pool");
}

Descriptor_Pool::~Descriptor_Pool()
{
  vkDestroyDescriptorPool(CRUDE_OBJECT_HANDLE(m_device), m_handle, getPVkAllocationCallbacks());
}

bool Descriptor_Pool::canFreeDescriptorSet() const
{
    return m_freeDescriptorSet;
}

}
