#include "device.hpp"
#include "core.hpp"
#include "device_queue_create_info.hpp"
#include "physical_device.hpp"
#include <vulkan/vulkan_core.h>

namespace crude_vulkan_01
{

DeviceCreateInfo::DeviceCreateInfo(std::shared_ptr<const Physical_Device>     physicalDevice,
                                   const std::vector<DeviceQueueCreateInfo>&  queueDescriptors,
                                   const VkPhysicalDeviceFeatures&            enabledFeatures,
                                   const std::vector<const char*>&            enabledExtensions,
                                   const std::vector<const char*>&            enabledLayers)
  :
  physicalDevice(physicalDevice),
  enabledFeatures(enabledFeatures),
  enabledExtensions(enabledExtensions),
  enabledLayers(enabledLayers),
  queueDescriptors(queueDescriptors)
{}

Device::Device(const DeviceCreateInfo& createinfo)
{
  VkDeviceCreateInfo vkDeviceCreateInfo{};
  vkDeviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  vkDeviceCreateInfo.queueCreateInfoCount    = static_cast<uint32>(createinfo.queueDescriptors.size());
  vkDeviceCreateInfo.pQueueCreateInfos       = createinfo.queueDescriptors.data();
  vkDeviceCreateInfo.pEnabledFeatures        = &createinfo.enabledFeatures;
  vkDeviceCreateInfo.enabledExtensionCount   = static_cast<uint32>(createinfo.enabledExtensions.size());
  vkDeviceCreateInfo.ppEnabledExtensionNames = createinfo.enabledExtensions.data();
  vkDeviceCreateInfo.enabledLayerCount       = static_cast<uint32>(createinfo.enabledLayers.size());
  vkDeviceCreateInfo.ppEnabledLayerNames     = createinfo.enabledLayers.data();
  vkDeviceCreateInfo.pNext                   = nullptr;
  vkDeviceCreateInfo.flags                   = 0u;

  const VkResult result = vkCreateDevice(
      CRUDE_VULKAN_01_HANDLE(createinfo.physicalDevice), 
      &vkDeviceCreateInfo, 
      nullptr, 
      &m_handle);

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

}
