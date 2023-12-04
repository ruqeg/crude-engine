#include "descriptor_pool.hpp"
#include "device.hpp"

namespace crude_engine
{


Descriptor_Pool_Create_Info::Descriptor_Pool_Create_Info(std::shared_ptr<const Device>             device,
                                                         const std::vector<VkDescriptorPoolSize>&  poolSizes,
                                                         uint32                                    maxSets,
                                                         bool                                      freeDescriptorSet)
  :
  device(device),
  poolSizes(poolSizes),
  maxSets(maxSets),
  freeDescriptorSet(freeDescriptorSet)
{}

Descriptor_Pool::Descriptor_Pool(const Descriptor_Pool_Create_Info& createInfo)
{
  m_device = createInfo.device;
  m_freeDescriptorSet = createInfo.freeDescriptorSet;

  VkDescriptorPoolCreateInfo vkCreateInfo{};
  vkCreateInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  vkCreateInfo.pNext          = nullptr;
  vkCreateInfo.flags          = 0u;

  vkCreateInfo.pPoolSizes     = createInfo.poolSizes.data();
  vkCreateInfo.poolSizeCount  = static_cast<uint32>(createInfo.poolSizes.size());
  vkCreateInfo.maxSets        = createInfo.maxSets;

  if (m_freeDescriptorSet)  vkCreateInfo.flags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

  VkResult result = vkCreateDescriptorPool(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create descriptor pool");
}

Descriptor_Pool::~Descriptor_Pool()
{
  vkDestroyDescriptorPool(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

bool Descriptor_Pool::canFreeDescriptorSet() const
{
    return m_freeDescriptorSet;
}

}
