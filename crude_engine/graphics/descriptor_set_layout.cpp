#include <vulkan/vulkan.hpp>

module crude.graphics.descriptor_set_layout;

import crude.graphics.device;
import crude.graphics.vulkan_utils;

namespace crude::graphics
{
  
Descriptor_Set_Layout::Descriptor_Set_Layout(core::Shared_Ptr<const Device>                    device,
                                             const core::span<Descriptor_Set_Layout_Binding>&  bindings)
  :
  m_device(device)
{
  VkDescriptorSetLayoutCreateInfo vkCreateInfo;
  vkCreateInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  vkCreateInfo.pNext         = nullptr;
  vkCreateInfo.flags         = 0u;

  vkCreateInfo.pBindings     = bindings.data();
  vkCreateInfo.bindingCount  = bindings.size();

  VkResult result = vkCreateDescriptorSetLayout(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create descriptor set layout");
}
  
Descriptor_Set_Layout::~Descriptor_Set_Layout()
{
  vkDestroyDescriptorSetLayout(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}
