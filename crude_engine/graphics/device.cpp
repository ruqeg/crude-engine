#include <vulkan/vulkan.hpp>

module crude.graphics.device;

import crude.graphics.queue;
import crude.graphics.fence;
import crude.graphics.physical_device;
import crude.graphics.vulkan_utils;
import crude.core.std_containers_heap;
import crude.core.algorithms;

namespace crude::graphics
{

Device_Queue_Create_Info::Device_Queue_Create_Info(core::uint32                      queueFamilyIndex,
                                                   const core::span<core::float32>&  queuePriorities)
{
  this->sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  this->pQueuePriorities  = queuePriorities.data();
  this->queueCount        = queuePriorities.size();
  this->pNext             = nullptr;
  this->flags             = 0u;
  this->queueFamilyIndex  = queueFamilyIndex;
}

Device::Device(core::Shared_Ptr<const Physical_Device>      physicalDevice,
               const core::span<Device_Queue_Create_Info>&  queueDescriptors,
               const VkPhysicalDeviceFeatures&              enabledFeatures,
               core::span<const char*>                      enabledExtensions,
               core::span<const char*>                      enabledLayers)
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

  const VkResult result = vkCreateDevice(m_physicalDevice->getHandle(), &vkDeviceCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create logic device!");
}

Device::~Device()
{
  vkDestroyDevice(m_handle, nullptr);
}
  
core::Shared_Ptr<const Physical_Device> Device::getPhysicalDevice() const
{
  return m_physicalDevice;
}
  
core::Shared_Ptr<Queue> Device::getQueue(core::uint32 queueFamilyIndex, core::uint32 queueIndex) const
{
  core::Shared_Ptr<Queue> queue = core::makeShared<Queue>(queueFamilyIndex, queueIndex);
  vkGetDeviceQueue(m_handle, queueFamilyIndex, queueIndex, &queue->getHandle());
  return queue;
}

void Device::updateDescriptorSets(const core::span<Write_Descriptor_Set>&  descriptorWrites,
                                  const core::span<VkCopyDescriptorSet>&   descriptorCopies)
{
  core::vector<VkWriteDescriptorSet> vkDescriptorWrites(descriptorWrites.size());


  //!TODO WTF???
  auto first = descriptorWrites.begin();
  auto last = descriptorWrites.end();
  core::vector<VkWriteDescriptorSet>::iterator dFirst = vkDescriptorWrites.begin();
  //Algorithms::copy(first, last, dFirst);

  while (first != last)
  {
    *dFirst = *first;
    ++dFirst; ++first;
  }

  vkUpdateDescriptorSets(
    m_handle, 
    static_cast<core::uint32>(vkDescriptorWrites.size()),
    vkDescriptorWrites.data(), 
    descriptorCopies.size(),
    descriptorCopies.data());
}

void Device::waitIdle()
{
  vkDeviceWaitIdle(m_handle);
}

bool Device::waitForFences(core::span<Fence> fences, bool waitAll, core::uint64 timeout) const
{
  core::vector<VkFence> fencesHandles(fences.size());
  core::Algorithms::copyc(fences.begin(), fences.end(), fencesHandles.begin(), [](auto& s, auto& d) -> void {
    *d = (s)->getHandle();
  });

  const VkResult result = vkWaitForFences(m_handle, fencesHandles.size(), fencesHandles.data(), waitAll, timeout);
  vulkanHandleResult(result, "failed to wait for fences");
  return result != VK_TIMEOUT;
}

bool Device::resetForFences(core::span<Fence> fences) const
{
  core::vector<VkFence> fencesHandles(fences.size());
  core::Algorithms::copyc(fences.begin(), fences.end(), fencesHandles.begin(), [](auto& s, auto& d) -> void {
    *d = (s)->getHandle();
  });

  const VkResult result = vkResetFences(m_handle, fencesHandles.size(), fencesHandles.data());
  vulkanHandleResult(result, "failed to reset fences");
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

}
