#include "descriptor_set.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_set_layout.hpp"
#include "device.hpp"

namespace crude_vulkan_01
{

Descriptor_Set_Create_Info::Descriptor_Set_Create_Info(std::shared_ptr<const Device>                               device,
                                                       const std::vector<std::shared_ptr<Descriptor_Set_Layout>>&  setLayouts,
                                                       std::shared_ptr<Descriptor_Pool>                            pool)
  :
  device(device),
  setLayouts(setLayouts),
  pool(pool)
{}

Descriptor_Set::Descriptor_Set(const Descriptor_Set_Create_Info& createInfo)
{
  m_device      = createInfo.device;
  m_pool        = createInfo.pool;
  m_setLayouts  = createInfo.setLayouts;

  std::vector<VkDescriptorSetLayout> setLayoutsHandles(m_setLayouts.size());
  for (uint32 i = 0; i < setLayoutsHandles.size(); ++i)
  {
    setLayoutsHandles[i] = CRUDE_VULKAN_01_HANDLE(m_setLayouts[i]);
  }

  VkDescriptorSetAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  vkAllocateInfo.pNext               = nullptr;
  vkAllocateInfo.pSetLayouts         = setLayoutsHandles.data();
  vkAllocateInfo.descriptorSetCount  = static_cast<uint32>(setLayoutsHandles.size());
  vkAllocateInfo.descriptorPool      = CRUDE_VULKAN_01_HANDLE(m_pool);

  VkResult result = vkAllocateDescriptorSets(CRUDE_VULKAN_01_HANDLE(m_device), &vkAllocateInfo, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to allocate descriptor set");
}

Descriptor_Set::~Descriptor_Set()
{
  if (m_pool->canFreeDescriptorSet())
    vkFreeDescriptorSets(CRUDE_VULKAN_01_HANDLE(m_device), CRUDE_VULKAN_01_HANDLE(m_pool), 1u, &m_handle);
}

}
