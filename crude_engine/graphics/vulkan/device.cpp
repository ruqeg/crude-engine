#include <graphics/vulkan/device.hpp>
#include <graphics/vulkan/physical_device.hpp>
#include <graphics/vulkan/queue.hpp>
#include <graphics/vulkan/fence.hpp>
#include <core/data_structures/array_dynamic.hpp>

namespace crude_engine
{

Device_Queue_Create_Info::Device_Queue_Create_Info(uint32                        queueFamilyIndex,
                                                   const Array_Unsafe<float32>&  queuePriorities)
{
  this->sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  this->pQueuePriorities  = queuePriorities.data();
  this->queueCount        = queuePriorities.size();
  this->pNext             = nullptr;
  this->flags             = 0u;
  this->queueFamilyIndex  = queueFamilyIndex;
}

Device::Device(Shared_Ptr<const Physical_Device>              physicalDevice,
               const Array_Unsafe<Device_Queue_Create_Info>&  queueDescriptors,
               const VkPhysicalDeviceFeatures&                enabledFeatures,
               Array_Unsafe<const char*>                      enabledExtensions,
               Array_Unsafe<const char*>                      enabledLayers)
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

  const VkResult result = vkCreateDevice(CRUDE_OBJECT_HANDLE(m_physicalDevice), &vkDeviceCreateInfo, getPVkAllocationCallbacks(), &m_handle);
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
  Shared_Ptr<Queue> queue = makeShared<Queue>(queueFamilyIndex, queueIndex);
  vkGetDeviceQueue(m_handle, queueFamilyIndex, queueIndex, &CRUDE_OBJECT_HANDLE(queue));
  return queue;
}

void Device::updateDescriptorSets(const Array_Unsafe<Write_Descriptor_Set>&  descriptorWrites,
                                  const Array_Unsafe<VkCopyDescriptorSet>&   descriptorCopies)
{
  Array_Dynamic<VkWriteDescriptorSet> vkDescriptorWrites(descriptorWrites.size());


  //!TODO WTF???
  Array_Unsafe<Write_Descriptor_Set>::Const_Iterator first = descriptorWrites.begin();
  Array_Unsafe<Write_Descriptor_Set>::Const_Iterator last = descriptorWrites.end();
  Array_Dynamic<VkWriteDescriptorSet>::Iterator dFirst = vkDescriptorWrites.begin();
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

bool Device::waitForFences(Array_Unsafe<Fence> fences, bool waitAll, uint64 timeout) const
{
  Array_Dynamic<VkFence> fencesHandles(fences.size());
  Algorithms::copyc(fences.begin(), fences.end(), fencesHandles.begin(), [](auto& s, auto& d) -> void {
    *d = CRUDE_OBJECT_HANDLE(s);
  });

  const VkResult result = vkWaitForFences(m_handle, fencesHandles.size(), fencesHandles.data(), waitAll, timeout);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to wait for fences");
  return result != VK_TIMEOUT;
}

bool Device::resetForFences(Array_Unsafe<Fence> fences) const
{
  Array_Dynamic<VkFence> fencesHandles(fences.size());
  Algorithms::copyc(fences.begin(), fences.end(), fencesHandles.begin(), [](auto& s, auto& d) -> void {
    *d = CRUDE_OBJECT_HANDLE(s);
  });

  const VkResult result = vkResetFences(m_handle, fencesHandles.size(), fencesHandles.data());
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to reset fences");
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

}
