#include <vulkan/vulkan.hpp>
#include <algorithm>

module crude.graphics.device;

import crude.graphics.queue;
import crude.graphics.fence;
import crude.graphics.physical_device;
import crude.graphics.vulkan_utils;
import crude.core.std_containers_heap;
import crude.core.algorithms;
import crude.core.logger;

namespace crude::graphics
{

Device::Device(core::shared_ptr<const Physical_Device>     physicalDevice,
               core::span<const Device_Queue_Descriptor>   queueDescriptors,
               const VkPhysicalDeviceFeatures&             enabledFeatures,
               core::span<const char* const>               enabledExtensions,
               core::span<const char* const>               enabledLayers)
  :
  m_physicalDevice(physicalDevice)
{
  std::unique_copy(queueDescriptors.begin(), queueDescriptors.end(), m_queueDescriptors.begin());
  core::vector<VkDeviceQueueCreateInfo> queueDescriptorsData(m_queueDescriptors.begin(), m_queueDescriptors.end());

  VkDeviceCreateInfo vkDeviceCreateInfo{};
  vkDeviceCreateInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  vkDeviceCreateInfo.pNext                    = nullptr;
  vkDeviceCreateInfo.flags                    = 0u;
  vkDeviceCreateInfo.queueCreateInfoCount     = queueDescriptorsData.size();
  vkDeviceCreateInfo.pQueueCreateInfos        = queueDescriptorsData.data();
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
  
core::shared_ptr<const Physical_Device> Device::getPhysicalDevice() const
{
  return m_physicalDevice;
}
  
core::Optional<core::shared_ptr<Queue>> Device::getQueueByFlag(VkQueueFlagBits flags, core::uint32 queueIndex) const
{
  // !TODO
  const Device_Queue_Descriptor queueDescription(m_physicalDevice, flags, {});
  core::shared_ptr<Queue> queue = core::allocateShared<Queue>(flags, queueDescription.queueFamilyIndex, queueIndex);
  vkGetDeviceQueue(m_handle, queueDescription.queueFamilyIndex, queueIndex, &queue->getHandle());
  if (queue->getHandle() == VK_NULL_HANDLE)
  {
    core::log(core::Debug::Channel::Graphics, "Failed Device::getQueue");
    return core::nullopt;
  }
  return queue;
}

core::Optional<core::shared_ptr<Queue>> Device::getQueueByFamily(core::uint32 queueFamilyIndex, core::uint32 queueIndex) const
{
  // !TODO
  for (auto flag : { VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT })
  {
    auto queue = getQueueByFlag(flag, queueIndex);
    if (queue.hasValue() && queue->getFamilyIndex() == queueFamilyIndex)
      return queue;
  }
  return core::nullopt;
}

void Device::updateDescriptorSets(const core::span<const Write_Descriptor_Set>&  descriptorWrites,
                                  const core::span<const VkCopyDescriptorSet>&   descriptorCopies)
{
  core::vector<VkWriteDescriptorSet> vkDescriptorWrites(descriptorWrites.size());
  core::copy(descriptorWrites.begin(), descriptorWrites.end(), vkDescriptorWrites.begin());

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
  core::copyc(fences.begin(), fences.end(), fencesHandles.begin(), [](auto& s, auto& d) -> void {
    *d = (s)->getHandle();
  });

  const VkResult result = vkWaitForFences(m_handle, fencesHandles.size(), fencesHandles.data(), waitAll, timeout);
  vulkanHandleResult(result, "failed to wait for fences");
  return result != VK_TIMEOUT;
}

bool Device::resetForFences(core::span<Fence> fences) const
{
  core::vector<VkFence> fencesHandles(fences.size());
  core::copyc(fences.begin(), fences.end(), fencesHandles.begin(), [](auto& s, auto& d) -> void {
    *d = (s)->getHandle();
  });

  const VkResult result = vkResetFences(m_handle, fencesHandles.size(), fencesHandles.data());
  vulkanHandleResult(result, "failed to reset fences");
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

const core::vector<Device_Queue_Descriptor>& Device::getQueueDescriptors() const
{
  return m_queueDescriptors;
}

}
