#include "semaphore.hpp"
#include "device.hpp"

namespace crude_vulkan_01
{

class Device;

Semaphore_Create_Info::Semaphore_Create_Info(std::shared_ptr<const Device> device, VkSemaphoreCreateFlags flags)
  :
  device(device),
  flags(flags)
{}

Semaphore::Semaphore(const Semaphore_Create_Info& createInfo)
{
  m_device = createInfo.device;

  VkSemaphoreCreateInfo vkCreateInfo{};
  vkCreateInfo.sType  = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  vkCreateInfo.pNext  = nullptr;
  vkCreateInfo.flags  = createInfo.flags;

  VkResult result = vkCreateSemaphore(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create semaphore");
}
 
Semaphore::~Semaphore()
{
  vkDestroySemaphore(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}
