#include <vulkan/vulkan.hpp>

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

Device::Device(core::shared_ptr<const Physical_Device>      physicalDevice,
               const core::span<Device_Queue_Create_Info>&  queueDescriptors,
               const VkPhysicalDeviceFeatures&              enabledFeatures,
               core::span<const char* const>                enabledExtensions,
               core::span<const char* const>                enabledLayers)
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
  
core::shared_ptr<const Physical_Device> Device::getPhysicalDevice() const
{
  return m_physicalDevice;
}
  
core::Optional<core::shared_ptr<Queue>> Device::getQueue(VkQueueFlagBits flags, core::uint32 queueIndex) const
{
  const Device_Queue_Create_Info queueDescription(m_physicalDevice, flags, {});
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
  for (auto flag : { VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT })
  {
    auto queue = getQueue(flag, queueIndex);
    if (queue.hasValue() && queue->getFamilyIndex() == queueFamilyIndex)
      return queue;
  }
  return core::nullopt;
}

void Device::updateDescriptorSets(const core::span<const Write_Descriptor_Set>&  descriptorWrites,
                                  const core::span<const VkCopyDescriptorSet>&   descriptorCopies)
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

}
