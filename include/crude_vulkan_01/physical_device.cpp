#include "physical_device.hpp"
#include "surface.hpp"

namespace crude_vulkan_01
{

Physical_Device_Create_Info::Physical_Device_Create_Info(VkPhysicalDevice vkPhysicalDevice)
  :
  vkPhysicalDevice(vkPhysicalDevice)
{}
  
Physical_Device::Physical_Device(const Physical_Device_Create_Info& createInfo)
{
  m_handle = createInfo.vkPhysicalDevice;
}
  
bool Physical_Device::getSupportSurface(std::shared_ptr<const Surface> surface, uint32 queueFamilyIndex) const
{
  VkBool32 presentSupport = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(
    m_handle,
    queueFamilyIndex,
    CRUDE_VULKAN_01_HANDLE(surface),
    &presentSupport);
  return presentSupport;
}
  
VkSurfaceCapabilitiesKHR Physical_Device::getSurfaceCapabilitis(std::shared_ptr<const Surface> surface) const
{
  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    m_handle, 
    CRUDE_VULKAN_01_HANDLE(surface),
    &capabilities);
  return capabilities;
}
   
std::vector<VkSurfaceFormatKHR> Physical_Device::getSurfaceFormats(std::shared_ptr<const Surface> surface)
{
  uint32 formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(
    m_handle, 
    CRUDE_VULKAN_01_HANDLE(surface), 
    &formatCount, 
    nullptr);

  if (formatCount == 0u)
  {
    return {};
  }

  std::vector<VkSurfaceFormatKHR> formats(formatCount);
  vkGetPhysicalDeviceSurfaceFormatsKHR(
    m_handle, 
    CRUDE_VULKAN_01_HANDLE(surface),
    &formatCount, 
    formats.data());
  return formats;
}
  
std::vector<VkPresentModeKHR> Physical_Device::getSurfacePresentModes(std::shared_ptr<const Surface> surface)
{
  uint32 presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(
    m_handle, 
    CRUDE_VULKAN_01_HANDLE(surface),
    &presentModeCount, 
    nullptr);

  if (presentModeCount == 0u) 
  {
    return {};
  }

  std::vector<VkPresentModeKHR> presentModes(presentModeCount);
  vkGetPhysicalDeviceSurfacePresentModesKHR(
    m_handle, 
    CRUDE_VULKAN_01_HANDLE(surface), 
    &presentModeCount, 
    presentModes.data());

  return presentModes;
}

std::vector<VkQueueFamilyProperties> Physical_Device::getQueueFamilyProperties() const
{
  uint32 queueFamilyCount = 0u;
  vkGetPhysicalDeviceQueueFamilyProperties(
    m_handle, 
    &queueFamilyCount, 
    nullptr);
  
  if (queueFamilyCount == 0u)
  {
    return {};
  }

  std::vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
    m_handle, 
    &queueFamilyCount, 
    queueFamiliesProperties.data());

  return queueFamiliesProperties;
}
  
std::vector<VkExtensionProperties> Physical_Device::getExtensionProperties() const
{
  uint32 extensionCount = 0u;
  vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, nullptr);
  if (extensionCount == 0u)
  {
    return {};
  }
    
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, extensions.data());
  
  return extensions;
}
  
VkFormatProperties Physical_Device::getFormatProperties(VkFormat format) const
{
  VkFormatProperties properties;
  vkGetPhysicalDeviceFormatProperties(m_handle, format, &properties);
  return properties;
}
  
VkPhysicalDeviceProperties Physical_Device::getProperties() const
{
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(m_handle, &properties);
  return properties;
}

VkPhysicalDeviceFeatures Physical_Device::getFeatures() const
{
  VkPhysicalDeviceFeatures features;
  vkGetPhysicalDeviceFeatures(m_handle, &features);
  return features;
}

}
