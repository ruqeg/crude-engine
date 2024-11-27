#include <vulkan/vulkan.hpp>

module crude.gfx.vk.command_pool;

import crude.gfx.vk.vulkan_utils;
import crude.gfx.vk.device;

namespace crude::gfx::vk
{

Command_Pool::Command_Pool(core::shared_ptr<const Device>  device,
                           VkCommandPoolCreateFlags        flags,
                           core::uint32                    queueFamilyIndex)
  :
  m_device(device),
  m_queueFamilyIndex(queueFamilyIndex)
{
  VkCommandPoolCreateInfo vkCreateInfo;
  vkCreateInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  vkCreateInfo.pNext             = nullptr;

  vkCreateInfo.flags             = flags;
  vkCreateInfo.queueFamilyIndex  = queueFamilyIndex;

  VkResult result = vkCreateCommandPool(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create command pool");
}

Command_Pool::~Command_Pool()
{
  vkDestroyCommandPool(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

core::shared_ptr<const Device> const Command_Pool::getDevice()
{ 
  return m_device;
}

}
