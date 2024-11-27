#include <vulkan/vulkan.hpp>
#include <algorithm>

module crude.gfx.vk.device;

import crude.gfx.vk.queue;
import crude.gfx.vk.fence;
import crude.gfx.vk.physical_device;
import crude.gfx.vk.vulkan_utils;
import crude.core.std_containers_heap;
import crude.core.algorithms;
import crude.core.logger;

namespace crude::gfx::vk
{

Device::Device(core::shared_ptr<const Physical_Device>     physicalDevice,
               core::span<const Device_Queue_Descriptor>   queueDescriptors,
               const VkPhysicalDeviceFeatures&             enabledFeatures,
               core::span<const char* const>               enabledExtensions,
               core::span<const char* const>               enabledLayers)
  :
  m_physicalDevice(physicalDevice),
  m_queueDescriptors(queueDescriptors.begin(), queueDescriptors.end())
{
  // !TODO
  m_queueDescriptors.erase(std::unique(m_queueDescriptors.begin(), m_queueDescriptors.end()), m_queueDescriptors.end());
  core::vector<VkDeviceQueueCreateInfo> queueDescriptorsData(m_queueDescriptors.begin(), m_queueDescriptors.end());

  VkPhysicalDeviceRobustness2FeaturesEXT deviceRobustness{};
  deviceRobustness.sType          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT;
  deviceRobustness.nullDescriptor = VK_TRUE;

  // !TODO
  VkPhysicalDevice8BitStorageFeatures deviceFeatures8BitStorage{};
  deviceFeatures8BitStorage.sType                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES;
  deviceFeatures8BitStorage.pNext                   = &deviceRobustness;
  deviceFeatures8BitStorage.storageBuffer8BitAccess = VK_TRUE;

  VkPhysicalDeviceMeshShaderFeaturesEXT deviceFeaturesMesh{};
  deviceFeaturesMesh.sType      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
  deviceFeaturesMesh.pNext      = &deviceFeatures8BitStorage;
  deviceFeaturesMesh.meshShader = VK_TRUE;
  deviceFeaturesMesh.taskShader = VK_TRUE;

  VkPhysicalDeviceFeatures2  deviceFeatures{};
  deviceFeatures.sType    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
  deviceFeatures.pNext    = &deviceFeaturesMesh;
  deviceFeatures.features = enabledFeatures;

  VkDeviceCreateInfo vkDeviceCreateInfo{};
  vkDeviceCreateInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  vkDeviceCreateInfo.pNext                    = &deviceFeatures;
  vkDeviceCreateInfo.flags                    = 0u;
  vkDeviceCreateInfo.queueCreateInfoCount     = queueDescriptorsData.size();
  vkDeviceCreateInfo.pQueueCreateInfos        = queueDescriptorsData.data();
  vkDeviceCreateInfo.pEnabledFeatures         = nullptr;
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
  
core::optional<core::shared_ptr<Queue>> Device::getQueueByFlag(VkQueueFlagBits flags, core::uint32 queueIndex) const
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

core::optional<core::shared_ptr<Queue>> Device::getQueueByFamily(core::uint32 queueFamilyIndex, core::uint32 queueIndex) const
{
  // !TODO
  for (auto flag : { VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT })
  {
    auto queue = getQueueByFlag(flag, queueIndex);
    if (queue.has_value() && queue.value()->getFamilyIndex() == queueFamilyIndex)
      return queue;
  }
  return core::nullopt;
}

void Device::updateDescriptorSets(const core::span<const VkWriteDescriptorSet>  descriptorWrites,
                                  const core::span<const VkCopyDescriptorSet>   descriptorCopies)
{
  vkUpdateDescriptorSets(
    m_handle, 
    static_cast<core::uint32>(descriptorWrites.size()),
    descriptorWrites.data(),
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
