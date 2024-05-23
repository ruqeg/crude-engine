#include <vulkan/vulkan.hpp>

module crude.graphics.command_pool;

import crude.graphics.vulkan_utils;
import crude.graphics.device;

namespace crude::graphics
{

Command_Pool::Command_Pool(core::Shared_Ptr<const Device>  device,
                           VkCommandPoolCreateFlags        flags,
                           core::uint32                    queueFamilyIndex)
  :
  m_device(device)
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

}
