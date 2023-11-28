#include "device.hpp"
#include "core.hpp"
#include "physical_device.hpp"
#include "queue.hpp"
#include "fence.hpp"

namespace crude_vulkan_01
{

Device_Queue_Create_Info::Device_Queue_Create_Info(uint32                       queueFamilyIndex,
                                                   const std::vector<float32>&  queuePriorities)
{
  this->sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  this->pQueuePriorities  = queuePriorities.data();
  this->queueCount        = queuePriorities.size();
  this->pNext             = nullptr;
  this->flags             = 0u;
  this->queueFamilyIndex  = queueFamilyIndex;
}

Device_Create_Info::Device_Create_Info(std::shared_ptr<const Physical_Device>    physicalDevice,
                                   const std::vector<Device_Queue_Create_Info>&  queueDescriptors,
                                   const VkPhysicalDeviceFeatures&               enabledFeatures,
                                   const std::vector<const char*>&               enabledExtensions,
                                   const std::vector<const char*>&               enabledLayers)
  :
  physicalDevice(physicalDevice),
  enabledFeatures(enabledFeatures),
  enabledExtensions(enabledExtensions),
  enabledLayers(enabledLayers),
  queueDescriptors(queueDescriptors)
{}

Device::Device(const Device_Create_Info& createinfo)
{
  VkDeviceCreateInfo vkDeviceCreateInfo{};
  vkDeviceCreateInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  vkDeviceCreateInfo.pNext                    = nullptr;
  vkDeviceCreateInfo.flags                    = 0u;

  vkDeviceCreateInfo.queueCreateInfoCount     = static_cast<uint32>(createinfo.queueDescriptors.size());
  vkDeviceCreateInfo.pQueueCreateInfos        = createinfo.queueDescriptors.data();
  vkDeviceCreateInfo.pEnabledFeatures         = &createinfo.enabledFeatures;
  vkDeviceCreateInfo.enabledExtensionCount    = static_cast<uint32>(createinfo.enabledExtensions.size());
  vkDeviceCreateInfo.ppEnabledExtensionNames  = createinfo.enabledExtensions.data();
  vkDeviceCreateInfo.enabledLayerCount        = static_cast<uint32>(createinfo.enabledLayers.size());
  vkDeviceCreateInfo.ppEnabledLayerNames      = createinfo.enabledLayers.data();

  const VkResult result = vkCreateDevice(CRUDE_VULKAN_01_HANDLE(createinfo.physicalDevice), &vkDeviceCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create logic device!");

  m_physicalDevice = createinfo.physicalDevice;
}

Device::~Device()
{
  vkDestroyDevice(m_handle, nullptr);
}
  
std::shared_ptr<const Physical_Device> Device::getPhysicalDevice() const
{
  return m_physicalDevice;
}
  
std::shared_ptr<Queue> Device::getQueue(uint32 queueFamilyIndex, uint32 queueIndex) const
{
  std::shared_ptr<Queue> queue = std::make_shared<Queue>(Queue_Create_Info(queueFamilyIndex, queueIndex));
  vkGetDeviceQueue(m_handle, queueFamilyIndex, queueIndex, &CRUDE_VULKAN_01_HANDLE(queue));
  return queue;
}

void Device::updateDescriptorSets(const std::vector<Write_Descriptor_Set>& descriptorWrites, const std::vector<VkCopyDescriptorSet>& descriptorCopies)
{
  std::vector<VkWriteDescriptorSet> vkDescriptorWrites(descriptorWrites.size());
  for (uint32 i = 0u; i < vkDescriptorWrites.size(); ++i)
  {
    vkDescriptorWrites[i] = descriptorWrites[i];
  }

  vkUpdateDescriptorSets(m_handle, static_cast<uint32>(vkDescriptorWrites.size()), vkDescriptorWrites.data(), static_cast<uint32>(descriptorCopies.size()), descriptorCopies.data());
}

void Device::waitIdle()
{
  vkDeviceWaitIdle(m_handle);
}

bool Device::waitForFences(const std::vector<std::shared_ptr<Fence>>& fences, bool waitAll, uint64_t timeout) const
{
  std::vector<VkFence> fencesHandles(fences.size());
  for (uint32 i = 0; i < fences.size(); ++i)
  {
    fencesHandles[i] = CRUDE_VULKAN_01_HANDLE(fences[i]);
  }

  const VkResult result = vkWaitForFences(m_handle, fencesHandles.size(), fencesHandles.data(), waitAll, timeout);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to wait for fences");
  return result != VK_TIMEOUT;
}

bool Device::resetForFences(const std::vector<std::shared_ptr<Fence>>& fences) const
{
  std::vector<VkFence> fencesHandles(fences.size());
  for (uint32 i = 0; i < fences.size(); ++i)
  {
    fencesHandles[i] = CRUDE_VULKAN_01_HANDLE(fences[i]);
  }

  const VkResult result = vkResetFences(m_handle, fencesHandles.size(), fencesHandles.data());
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to reset fences");
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

}
