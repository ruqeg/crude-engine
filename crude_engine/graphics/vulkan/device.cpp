#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.device;

import crude_engine.graphics.vulkan.physical_device;
import crude_engine.graphics.vulkan.queue;
import crude_engine.graphics.vulkan.vulkan_utils;
import crude_engine.core.std_containers_heap;
import crude_engine.core.algorithms;

namespace crude_engine
{

Device_Queue_Create_Info::Device_Queue_Create_Info(uint32                queueFamilyIndex,
                                                   const span<float32>&  queuePriorities)
{
  this->sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  this->pQueuePriorities  = queuePriorities.data();
  this->queueCount        = queuePriorities.size();
  this->pNext             = nullptr;
  this->flags             = 0u;
  this->queueFamilyIndex  = queueFamilyIndex;
}

Device::Device(Shared_Ptr<const Physical_Device>      physicalDevice,
               const span<Device_Queue_Create_Info>&  queueDescriptors,
               const VkPhysicalDeviceFeatures&        enabledFeatures,
               span<const char*>                      enabledExtensions,
               span<const char*>                      enabledLayers)
  :
  m_physicalDevice(physicalDevice)
{
  VkDeviceCreateInfo vkDeviceCreateInfo{};
  vkDeviceCreateInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  vkDeviceCreateInfo.pNext                    = nullptr;
  vkDeviceCreateInfo.flags                    = 0u;

  vkDeviceCreateInfo.queueCreateInfoCount     = queueDescriptors.size();
  vkDeviceCreateInfo.pQueueCreateInfos        = queueDescriptors.data();
  vkDeviceCreateInfo.pEnabledFeatures         = &enabledFeatures;
  vkDeviceCreateInfo.enabledExtensionCount    = enabledExtensions.size();
  vkDeviceCreateInfo.ppEnabledExtensionNames  = enabledExtensions.data();
  vkDeviceCreateInfo.enabledLayerCount        = enabledLayers.size();
  vkDeviceCreateInfo.ppEnabledLayerNames      = enabledLayers.data();

  const VkResult result = vkCreateDevice(m_physicalDevice->getHanle(), &vkDeviceCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create logic device!");
}

Device::~Device()
{
  vkDestroyDevice(m_handle, nullptr);
}
  
Shared_Ptr<const Physical_Device> Device::getPhysicalDevice() const
{
  return m_physicalDevice;
}
  
Shared_Ptr<Queue> Device::getQueue(uint32 queueFamilyIndex, uint32 queueIndex) const
{
  Shared_Ptr<Queue> queue = makeShared<Queue>(queueFamilyIndex, queueIndex);
  vkGetDeviceQueue(m_handle, queueFamilyIndex, queueIndex, &CRUDE_OBJECT_HANDLE(queue));
  return queue;
}

void Device::updateDescriptorSets(const span<Write_Descriptor_Set>&  descriptorWrites,
                                  const span<VkCopyDescriptorSet>&   descriptorCopies)
{
  vector<VkWriteDescriptorSet> vkDescriptorWrites(descriptorWrites.size());


  //!TODO WTF???
  auto first = descriptorWrites.begin();
  auto last = descriptorWrites.end();
  vector<VkWriteDescriptorSet>::iterator dFirst = vkDescriptorWrites.begin();
  //Algorithms::copy(first, last, dFirst);

  while (first != last)
  {
    *dFirst = *first;
    ++dFirst; ++first;
  }

  vkUpdateDescriptorSets(
    m_handle, 
    static_cast<uint32>(vkDescriptorWrites.size()), 
    vkDescriptorWrites.data(), 
    descriptorCopies.size(),
    descriptorCopies.data());
}

void Device::waitIdle()
{
  vkDeviceWaitIdle(m_handle);
}

bool Device::waitForFences(span<Fence> fences, bool waitAll, uint64 timeout) const
{
  vector<VkFence> fencesHandles(fences.size());
  Algorithms::copyc(fences.begin(), fences.end(), fencesHandles.begin(), [](auto& s, auto& d) -> void {
    *d = CRUDE_OBJECT_HANDLE(s);
  });

  const VkResult result = vkWaitForFences(m_handle, fencesHandles.size(), fencesHandles.data(), waitAll, timeout);
  vulkanHandleResult(result, "failed to wait for fences");
  return result != VK_TIMEOUT;
}

bool Device::resetForFences(span<Fence> fences) const
{
  vector<VkFence> fencesHandles(fences.size());
  Algorithms::copyc(fences.begin(), fences.end(), fencesHandles.begin(), [](auto& s, auto& d) -> void {
    *d = CRUDE_OBJECT_HANDLE(s);
  });

  const VkResult result = vkResetFences(m_handle, fencesHandles.size(), fencesHandles.data());
  vulkanHandleResult(result, "failed to reset fences");
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

}
