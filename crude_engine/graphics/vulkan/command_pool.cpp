#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.command_pool;

import crude_engine.graphics.vulkan.vulkan_utils;
import crude_engine.graphics.vulkan.device;

namespace crude_engine
{

Command_Pool::Command_Pool(Shared_Ptr<const Device>  device,
                           VkCommandPoolCreateFlags  flags,
                           uint32                    queueFamilyIndex)
  :
  m_device(device)
{
  VkCommandPoolCreateInfo vkCreateInfo;
  vkCreateInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  vkCreateInfo.pNext             = nullptr;

  vkCreateInfo.flags             = flags;
  vkCreateInfo.queueFamilyIndex  = queueFamilyIndex;

  VkResult result = vkCreateCommandPool(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleError(result, "failed to create command pool");
}

Command_Pool::~Command_Pool()
{
  vkDestroyCommandPool(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}
