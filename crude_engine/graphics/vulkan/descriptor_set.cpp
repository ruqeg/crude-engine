#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.descriptor_set;

import crude_engine.graphics.vulkan.vulkan_utils;
import crude_engine.graphics.vulkan.descriptor_pool;
import crude_engine.graphics.vulkan.descriptor_set_layout;
import crude_engine.graphics.vulkan.device;
import crude_engine.core.algorithms;

namespace crude_engine
{

Descriptor_Set::Descriptor_Set(Shared_Ptr<const Device>                          device,
                               const vector<Shared_Ptr<Descriptor_Set_Layout>>&  setLayouts,
                               Shared_Ptr<Descriptor_Pool>                       pool)
  :
  m_device(device),
  m_pool(pool),
  m_setLayouts(setLayouts)
{
  vector<VkDescriptorSetLayout> setLayoutsHandles(m_setLayouts.size());
  Algorithms::copyc(m_setLayouts.begin(), m_setLayouts.end(), setLayoutsHandles.begin(), [](auto& s, auto& d) -> void {
    *d = (*s)->getHandle();
  });

  VkDescriptorSetAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  vkAllocateInfo.pNext               = nullptr;
  vkAllocateInfo.pSetLayouts         = setLayoutsHandles.data();
  vkAllocateInfo.descriptorSetCount  = static_cast<uint32>(setLayoutsHandles.size());
  vkAllocateInfo.descriptorPool      = m_pool->getHandle();

  VkResult result = vkAllocateDescriptorSets(m_device->getHandle(), &vkAllocateInfo, &m_handle);
  vulkanHandleResult(result, "failed to allocate descriptor set");
}

Descriptor_Set::~Descriptor_Set()
{
  if (m_pool->canFreeDescriptorSet())
    vkFreeDescriptorSets(m_device->getHandle(), m_pool->getHandle(), 1u, &m_handle);
}

}
