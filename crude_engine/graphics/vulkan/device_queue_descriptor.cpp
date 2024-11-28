#include <vulkan/vulkan.hpp>

module crude.gfx.vk.device_queue_descriptor;

import crude.gfx.vk.surface;
import crude.gfx.vk.physical_device;
import crude.core.logger;

namespace crude::gfx::vk
{

Device_Queue_Descriptor::Device_Queue_Descriptor(core::shared_ptr<const Physical_Device>  physicalDevice,
                                                 VkQueueFlagBits                          queueType,
                                                 core::span<const core::float32>          queuePriorities)
  :
  m_queueDenotation(queueFlagBitsToDenotation(queueType))
{
  this->sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  this->pNext            = nullptr;
  this->flags            = 0;
  this->queueFamilyIndex = chooseFamilyIndex(queueType, physicalDevice->getQueueFamilyProperties()),
  this->queueCount       = queuePriorities.size(),
  this->pQueuePriorities = queuePriorities.data();
}

Device_Queue_Descriptor::Device_Queue_Descriptor(core::shared_ptr<const Physical_Device>  physicalDevice,
                                                 core::shared_ptr<const Surface>          surface, 
                                                 core::span<const core::float32>          queuePriorities)
  :
  m_queueDenotation(DEVICE_QUEUE_DENOTATION_PRESENT_BITS)
{
  this->sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  this->pNext            = nullptr;
  this->flags            = 0;
  this->queueFamilyIndex = chooseFamilyIndex(physicalDevice, surface, physicalDevice->getQueueFamilyProperties()),
  this->queueCount       = queuePriorities.size(),
  this->pQueuePriorities = queuePriorities.data();
}

bool Device_Queue_Descriptor::operator==(const Device_Queue_Descriptor& other) const
{
  if (this->queueFamilyIndex != other.queueFamilyIndex || this->queueCount != other.queueCount)
    return false;

  for (core::uint32 i = 0; i < this->queueCount; ++i)
  {
    if (this->pQueuePriorities[i] != other.pQueuePriorities[i])
      return false;
  }
    
  return true;
}

bool Device_Queue_Descriptor::operator!=(const Device_Queue_Descriptor& other) const
{
  return !operator==(other);
}

core::uint32 Device_Queue_Descriptor::chooseFamilyIndex(VkQueueFlagBits queueType, const core::vector<VkQueueFamilyProperties>& queueFamilyProperties) const
{
  core::uint32 queueFamilyIndex = 0;
  if (queueType == VK_QUEUE_TRANSFER_BIT)
  {
    for (auto const& property : queueFamilyProperties)
    {
      if (property.queueFlags & queueType)
      {
        const bool hasGraphicsOrCompute = property.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
        if (!hasGraphicsOrCompute)
          return queueFamilyIndex;
      }
      ++queueFamilyIndex;
    }
  }
  
  queueFamilyIndex = 0;
  for (auto const& property : queueFamilyProperties)
  {
    if (property.queueFlags & queueType)
      return queueFamilyIndex;
    ++queueFamilyIndex;
  }
  return 0u;
}

core::uint32 Device_Queue_Descriptor::chooseFamilyIndex(core::shared_ptr<const Physical_Device>       physicalDevice,
                                                        core::shared_ptr<const Surface>               surface, 
                                                        const core::vector<VkQueueFamilyProperties>&  queueFamilyProperties) const
{
  core::uint32 queueFamilyIndex = 0;
  for (auto const& property : queueFamilyProperties)
  {
    if (physicalDevice->checkPresentSupport(surface, queueFamilyIndex))
    {
      return queueFamilyIndex;
    }
    ++queueFamilyIndex;
  }
  core::logError(core::Debug::Channel::Graphics, "Failed to choose family index of surface queue");
  return 0;
}

Device_Queue_Denotation_Flag_Bits queueFlagBitsToDenotation(VkQueueFlagBits flag)
{
  switch (flag)
  {
  case VK_QUEUE_GRAPHICS_BIT: return DEVICE_QUEUE_DENOTATION_GRAPHICS_BITS;
  case VK_QUEUE_TRANSFER_BIT: return DEVICE_QUEUE_DENOTATION_TRANSFER_BITS;
  default:
    core::logError(core::Debug::Channel::Graphics, "Unknown flag for Device_Queue_Info::queueFlagToDenotation!");
  }
  return DEVICE_QUEUE_DENOTATION_MAX_BITS;
}

}