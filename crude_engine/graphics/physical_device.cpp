#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.physical_device;

import crude.graphics.vulkan.surface;

namespace crude::graphics
{
 
Physical_Device::Physical_Device(VkPhysicalDevice vkPhysicalDevice)
{
  m_handle = vkPhysicalDevice;
}
  
bool Physical_Device::getSupportSurface(core::Shared_Ptr<const Surface> surface, core::uint32 queueFamilyIndex) const
{
  VkBool32 presentSupport = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(
    m_handle,
    queueFamilyIndex,
    surface->getHandle(),
    &presentSupport);
  return presentSupport;
}
  
VkSurfaceCapabilitiesKHR Physical_Device::getSurfaceCapabilitis(core::Shared_Ptr<const Surface> surface) const
{
  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    m_handle, 
    surface->getHandle(),
    &capabilities);
  return capabilities;
}
   
core::vector<VkSurfaceFormatKHR> Physical_Device::getSurfaceFormats(core::Shared_Ptr<const Surface> surface)
{
  core::uint32 formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(
    m_handle, 
    surface->getHandle(),
    &formatCount, 
    nullptr);

  if (formatCount == 0u)
  {
    return {};
  }

  core::vector<VkSurfaceFormatKHR> formats(formatCount);
  vkGetPhysicalDeviceSurfaceFormatsKHR(
    m_handle, 
    surface->getHandle(),
    &formatCount, 
    formats.data());
  return formats;
}

core::vector<VkPresentModeKHR> Physical_Device::getSurfacePresentModes(core::Shared_Ptr<const Surface> surface)
{
  core::uint32 presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(
    m_handle, 
    surface->getHandle(),
    &presentModeCount, 
    nullptr);

  if (presentModeCount == 0u) 
  {
    return {};
  }

  core::vector<VkPresentModeKHR> presentModes(presentModeCount);
  vkGetPhysicalDeviceSurfacePresentModesKHR(
    m_handle, 
    surface->getHandle(),
    &presentModeCount, 
    presentModes.data());

  return presentModes;
}

core::vector<VkQueueFamilyProperties> Physical_Device::getQueueFamilyProperties() const
{
  core::uint32 queueFamilyCount = 0u;
  vkGetPhysicalDeviceQueueFamilyProperties(
    m_handle, 
    &queueFamilyCount, 
    nullptr);
  
  if (queueFamilyCount == 0u)
  {
    return {};
  }

  core::vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
    m_handle, 
    &queueFamilyCount, 
    queueFamiliesProperties.data());

  return queueFamiliesProperties;
}
  
core::vector<VkExtensionProperties> Physical_Device::getExtensionProperties() const
{
  core::uint32 extensionCount = 0u;
  vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, nullptr);
  if (extensionCount == 0u)
  {
    return {};
  }
    
  core::vector<VkExtensionProperties> extensions(extensionCount);
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
