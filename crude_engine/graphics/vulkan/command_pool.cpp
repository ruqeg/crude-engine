#include "command_pool.hpp"
#include "device.hpp"

namespace crude_engine
{

Command_Pool_Create_Info::Command_Pool_Create_Info(std::shared_ptr<const Device>  device,
                                                   VkCommandPoolCreateFlags       flags,
                                                   uint32                         queueFamilyIndex)
  :
  device(device),
  flags(flags),
  queueFamilyIndex(queueFamilyIndex)
{}

Command_Pool::Command_Pool(const Command_Pool_Create_Info& createInfo)
{
  m_device = createInfo.device;

  VkCommandPoolCreateInfo vkCreateInfo;
  vkCreateInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  vkCreateInfo.pNext             = nullptr;

  vkCreateInfo.flags             = createInfo.flags;
  vkCreateInfo.queueFamilyIndex  = createInfo.queueFamilyIndex;

  VkResult result = vkCreateCommandPool(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create command pool");
}

Command_Pool::~Command_Pool()
{
  vkDestroyCommandPool(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}
