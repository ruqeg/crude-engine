#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.physical_device;

import crude_engine.graphics.vulkan.surface;

namespace crude_engine
{
 
Physical_Device::Physical_Device(VkPhysicalDevice vkPhysicalDevice)
{
  m_handle = vkPhysicalDevice;
}
  
bool Physical_Device::getSupportSurface(Shared_Ptr<const Surface> surface, uint32 queueFamilyIndex) const
{
  VkBool32 presentSupport = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(
    m_handle,
    queueFamilyIndex,
    surface->getHandle(),
    &presentSupport);
  return presentSupport;
}
  
VkSurfaceCapabilitiesKHR Physical_Device::getSurfaceCapabilitis(Shared_Ptr<const Surface> surface) const
{
  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    m_handle, 
    surface->getHandle(),
    &capabilities);
  return capabilities;
}
   
vector<VkSurfaceFormatKHR> Physical_Device::getSurfaceFormats(Shared_Ptr<const Surface> surface)
{
  uint32 formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(
    m_handle, 
    surface->getHandle(),
    &formatCount, 
    nullptr);

  if (formatCount == 0u)
  {
    return {};
  }

  vector<VkSurfaceFormatKHR> formats(formatCount);
  vkGetPhysicalDeviceSurfaceFormatsKHR(
    m_handle, 
    surface->getHandle(),
    &formatCount, 
    formats.data());
  return formats;
}

vector<VkPresentModeKHR> Physical_Device::getSurfacePresentModes(Shared_Ptr<const Surface> surface)
{
  uint32 presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(
    m_handle, 
    surface->getHandle(),
    &presentModeCount, 
    nullptr);

  if (presentModeCount == 0u) 
  {
    return {};
  }

  vector<VkPresentModeKHR> presentModes(presentModeCount);
  vkGetPhysicalDeviceSurfacePresentModesKHR(
    m_handle, 
    surface->getHandle(),
    &presentModeCount, 
    presentModes.data());

  return presentModes;
}

vector<VkQueueFamilyProperties> Physical_Device::getQueueFamilyProperties() const
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

  vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
    m_handle, 
    &queueFamilyCount, 
    queueFamiliesProperties.data());

  return queueFamiliesProperties;
}
  
vector<VkExtensionProperties> Physical_Device::getExtensionProperties() const
{
  uint32 extensionCount = 0u;
  vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, nullptr);
  if (extensionCount == 0u)
  {
    return {};
  }
    
  vector<VkExtensionProperties> extensions(extensionCount);
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

VkPhysicalDeviceMemoryProperties Physical_Device::getMemoryProperties() const
{
  VkPhysicalDeviceMemoryProperties properties;
  vkGetPhysicalDeviceMemoryProperties(m_handle, &properties);
  return properties;
}

}
