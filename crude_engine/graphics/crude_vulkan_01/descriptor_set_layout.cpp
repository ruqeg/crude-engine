#include "descriptor_set_layout.hpp"
#include "device.hpp"

namespace crude_vulkan_01
{
  
Descriptor_Set_Layout_Create_Info::Descriptor_Set_Layout_Create_Info(std::shared_ptr<const Device>                     device,
                                                                     const std::vector<VkDescriptorSetLayoutBinding>&  bindings)
  :
  device(device),
  bindings(bindings)
{}
  
Descriptor_Set_Layout::Descriptor_Set_Layout(const Descriptor_Set_Layout_Create_Info& createInfo)
{
  m_device = createInfo.device;

  VkDescriptorSetLayoutCreateInfo vkCreateInfo;
  vkCreateInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  vkCreateInfo.pNext         = nullptr;
  vkCreateInfo.flags         = 0u;

  vkCreateInfo.pBindings     = createInfo.bindings.data();
  vkCreateInfo.bindingCount  = static_cast<uint32>(createInfo.bindings.size());

  VkResult result = vkCreateDescriptorSetLayout(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create descriptor set layout");
}
  
Descriptor_Set_Layout::~Descriptor_Set_Layout()
{
  vkDestroyDescriptorSetLayout(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}
