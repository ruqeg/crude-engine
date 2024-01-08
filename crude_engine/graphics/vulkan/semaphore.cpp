#include <graphics/vulkan/semaphore.hpp>
#include <graphics/vulkan/device.hpp>

namespace crude_engine
{

Semaphore::Semaphore(Shared_Ptr<const Device> device, VkSemaphoreCreateFlags flags)
  :
  m_device(device)
{
  VkSemaphoreCreateInfo vkCreateInfo{};
  vkCreateInfo.sType  = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  vkCreateInfo.pNext  = nullptr;
  vkCreateInfo.flags  = flags;

  VkResult result = vkCreateSemaphore(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, &getVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create semaphore");
}
 
Semaphore::~Semaphore()
{
  vkDestroySemaphore(CRUDE_OBJECT_HANDLE(m_device), m_handle, &getVkAllocationCallbacks());
}

}
