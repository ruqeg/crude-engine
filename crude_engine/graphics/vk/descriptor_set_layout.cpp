#include <vulkan/vulkan.hpp>

module crude.gfx.vk.descriptor_set_layout;

import crude.gfx.vk.device;
import crude.gfx.vk.vulkan_utils;

namespace crude::gfx::vk
{
  
Descriptor_Set_Layout::Descriptor_Set_Layout(core::shared_ptr<const Device>                   device,
                                             core::span<const Descriptor_Set_Layout_Binding>  bindings,
                                             bool                                             pushDescriptor)
  :
  m_device(device)
{
  VkDescriptorSetLayoutCreateInfo vkCreateInfo;
  vkCreateInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  vkCreateInfo.pNext         = nullptr;
  vkCreateInfo.flags         = pushDescriptor ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR : 0;

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
