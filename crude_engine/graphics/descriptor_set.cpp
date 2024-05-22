#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.descriptor_set;

import crude.graphics.vulkan.vulkan_utils;
import crude.graphics.vulkan.descriptor_pool;
import crude.graphics.vulkan.descriptor_set_layout;
import crude.graphics.vulkan.device;
import crude.core.algorithms;

namespace crude::graphics
{

Descriptor_Set::Descriptor_Set(core::Shared_Ptr<const Device>                                device,
                               const core::vector<core::Shared_Ptr<Descriptor_Set_Layout>>&  setLayouts,
                               core::Shared_Ptr<Descriptor_Pool>                             pool)
  :
  m_device(device),
  m_pool(pool),
  m_setLayouts(setLayouts)
{
  core::vector<VkDescriptorSetLayout> setLayoutsHandles(m_setLayouts.size());
  core::Algorithms::copyc(m_setLayouts.begin(), m_setLayouts.end(), setLayoutsHandles.begin(), [](auto& s, auto& d) -> void {
    *d = (*s)->getHandle();
  });

  VkDescriptorSetAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  vkAllocateInfo.pNext               = nullptr;
  vkAllocateInfo.pSetLayouts         = setLayoutsHandles.data();
  vkAllocateInfo.descriptorSetCount  = static_cast<core::uint32>(setLayoutsHandles.size());
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
