#include <graphics/vulkan/device.hpp>
#include <graphics/vulkan/physical_device.hpp>
#include <graphics/vulkan/queue.hpp>
#include <graphics/vulkan/fence.hpp>
#include <core/data_structures/array_dynamic.hpp>

namespace crude_engine
{

Device_Queue_Create_Info::Device_Queue_Create_Info(uint32          queueFamilyIndex,
                                                   const float32*  pQueuePriorities,
                                                   uint32          queuePrioritiCount)
{
  this->sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  this->pQueuePriorities  = pQueuePriorities;
  this->queueCount        = queuePrioritiCount;
  this->pNext             = nullptr;
  this->flags             = 0u;
  this->queueFamilyIndex  = queueFamilyIndex;
}

Device::Device(Shared_Ptr<const Physical_Device>  physicalDevice,
               Device_Queue_Create_Info*          pQueueDescriptors,
               uint32                             queueDescriptorCount,
               const VkPhysicalDeviceFeatures*    pEnabledFeatures,
               uint32                             enabledFeatureCount,
               const char**                       pEnabledExtensions,
               uint32                             enabledExtensionCount,
               const char**                       pEnabledLayers,
               uint32                             enabledLayerCount)
  :
  m_physicalDevice(physicalDevice)
{
  VkDeviceCreateInfo vkDeviceCreateInfo{};
  vkDeviceCreateInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  vkDeviceCreateInfo.pNext                    = nullptr;
  vkDeviceCreateInfo.flags                    = 0u;

  vkDeviceCreateInfo.queueCreateInfoCount     = queueDescriptorCount;
  vkDeviceCreateInfo.pQueueCreateInfos        = pQueueDescriptors;
  vkDeviceCreateInfo.pEnabledFeatures         = pEnabledFeatures;
  vkDeviceCreateInfo.enabledExtensionCount    = enabledExtensionCount;
  vkDeviceCreateInfo.ppEnabledExtensionNames  = pEnabledExtensions;
  vkDeviceCreateInfo.enabledLayerCount        = enabledLayerCount;
  vkDeviceCreateInfo.ppEnabledLayerNames      = pEnabledLayers;

  const VkResult result = vkCreateDevice(CRUDE_OBJECT_HANDLE(m_physicalDevice), &vkDeviceCreateInfo, &getVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create logic device!");
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
  Shared_Ptr<Queue> queue = Shared_Ptr<Queue>::makeShared(Queue_Create_Info(queueFamilyIndex, queueIndex));
  vkGetDeviceQueue(m_handle, queueFamilyIndex, queueIndex, &CRUDE_OBJECT_HANDLE(queue));
  return queue;
}

void Device::updateDescriptorSets(const Write_Descriptor_Set*  pDescriptorWrites,
                                  const uint32                 descriptorWriteCount,
                                  const VkCopyDescriptorSet*   pDescriptorCopies,
                                  const uint32                 descriptorCopieCount)
{
  CRUDE_ASSERT(pDescriptorWrites);
  CRUDE_ASSERT(pDescriptorCopies);

  Array_Dynamic<VkWriteDescriptorSet> vkDescriptorWrites(descriptorWriteCount);
  Algorithms::copy(pDescriptorWrites, pDescriptorWrites + descriptorCopieCount, vkDescriptorWrites.begin());

  vkUpdateDescriptorSets(
    m_handle, 
    static_cast<uint32>(vkDescriptorWrites.size()), 
    vkDescriptorWrites.data(), 
    descriptorCopieCount,
    pDescriptorCopies);
}

void Device::waitIdle()
{
  vkDeviceWaitIdle(m_handle);
}

bool Device::waitForFences(Fence* pFences, uint32 fenceCount, bool waitAll, uint64 timeout) const
{
  CRUDE_ASSERT(pFences);

  Array_Dynamic<VkFence> fencesHandles(fenceCount);
  Algorithms::copyc(pFences, pFences + fenceCount, fencesHandles.begin(), [](auto& s, auto& d) -> void {
    *d = CRUDE_OBJECT_HANDLE(s);
  });

  const VkResult result = vkWaitForFences(m_handle, fencesHandles.size(), fencesHandles.data(), waitAll, timeout);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to wait for fences");
  return result != VK_TIMEOUT;
}

bool Device::resetForFences(Fence* pFences, uint32 fenceCount) const
{
  CRUDE_ASSERT(pFences);

  Array_Dynamic<VkFence> fencesHandles(fenceCount);
  Algorithms::copyc(pFences, pFences + fenceCount, fencesHandles.begin(), [](auto& s, auto& d) -> void {
    *d = CRUDE_OBJECT_HANDLE(s);
  });

  const VkResult result = vkResetFences(m_handle, fencesHandles.size(), fencesHandles.data());
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to reset fences");
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

}
