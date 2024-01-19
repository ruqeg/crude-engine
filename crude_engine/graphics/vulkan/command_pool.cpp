#include <graphics/vulkan/command_pool.hpp>
#include <graphics/vulkan/device.hpp>

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

  VkResult result = vkCreateCommandPool(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create command pool");
}

Command_Pool::~Command_Pool()
{
  vkDestroyCommandPool(CRUDE_OBJECT_HANDLE(m_device), m_handle, getPVkAllocationCallbacks());
}

}
