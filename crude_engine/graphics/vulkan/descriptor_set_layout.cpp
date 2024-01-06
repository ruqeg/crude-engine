#include <graphics/vulkan/descriptor_set_layout.hpp>
#include <graphics/vulkan/device.hpp>

namespace crude_engine
{
  
Descriptor_Set_Layout::Descriptor_Set_Layout(Shared_Ptr<const Device>       device,
                                             VkDescriptorSetLayoutBinding*  pBindings,
                                             uint32                         bindingCount)
  :
  m_device(device)
{
  VkDescriptorSetLayoutCreateInfo vkCreateInfo;
  vkCreateInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  vkCreateInfo.pNext         = nullptr;
  vkCreateInfo.flags         = 0u;

  vkCreateInfo.pBindings     = pBindings;
  vkCreateInfo.bindingCount  = bindingCount;

  VkResult result = vkCreateDescriptorSetLayout(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, &getVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create descriptor set layout");
}
  
Descriptor_Set_Layout::~Descriptor_Set_Layout()
{
  vkDestroyDescriptorSetLayout(CRUDE_OBJECT_HANDLE(m_device), m_handle, &getVkAllocationCallbacks());
}

}
