#include "device_queue_create_info.hpp"
#include "core.hpp"
#include "physical_device.hpp"

namespace crude_vulkan_01 
{
  
DeviceQueueCreateInfo::DeviceQueueCreateInfo(uint32                       queueFamilyIndex,
                                             const std::vector<float32>&  queuePriorities)
{
  this->sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  this->pQueuePriorities = queuePriorities.data();
  this->queueCount       = queuePriorities.size();
  this->pNext            = nullptr;
  this->flags            = 0u;
  this->queueFamilyIndex = queueFamilyIndex;
}

}
