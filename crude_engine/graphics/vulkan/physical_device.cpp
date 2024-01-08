#include <graphics/vulkan/physical_device.hpp>
#include <graphics/vulkan/surface.hpp>

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
    CRUDE_OBJECT_HANDLE(surface),
    &presentSupport);
  return presentSupport;
}
  
VkSurfaceCapabilitiesKHR Physical_Device::getSurfaceCapabilitis(Shared_Ptr<const Surface> surface) const
{
  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    m_handle, 
    CRUDE_OBJECT_HANDLE(surface),
    &capabilities);
  return capabilities;
}
   
Array_Dynamic<VkSurfaceFormatKHR> Physical_Device::getSurfaceFormats(Shared_Ptr<const Surface> surface)
{
  uint32 formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(
    m_handle, 
    CRUDE_OBJECT_HANDLE(surface),
    &formatCount, 
    nullptr);

  if (formatCount == 0u)
  {
    return {};
  }

  Array_Dynamic<VkSurfaceFormatKHR> formats(formatCount);
  vkGetPhysicalDeviceSurfaceFormatsKHR(
    m_handle, 
    CRUDE_OBJECT_HANDLE(surface),
    &formatCount, 
    formats.data());
  return formats;
}

Array_Dynamic<VkPresentModeKHR> Physical_Device::getSurfacePresentModes(Shared_Ptr<const Surface> surface)
{
  uint32 presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(
    m_handle, 
    CRUDE_OBJECT_HANDLE(surface),
    &presentModeCount, 
    nullptr);

  if (presentModeCount == 0u) 
  {
    return {};
  }

  Array_Dynamic<VkPresentModeKHR> presentModes(presentModeCount);
  vkGetPhysicalDeviceSurfacePresentModesKHR(
    m_handle, 
    CRUDE_OBJECT_HANDLE(surface),
    &presentModeCount, 
    presentModes.data());

  return presentModes;
}

Array_Dynamic<VkQueueFamilyProperties> Physical_Device::getQueueFamilyProperties() const
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

  Array_Dynamic<VkQueueFamilyProperties> queueFamiliesProperties(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
    m_handle, 
    &queueFamilyCount, 
    queueFamiliesProperties.data());

  return queueFamiliesProperties;
}
  
Array_Dynamic<VkExtensionProperties> Physical_Device::getExtensionProperties() const
{
  uint32 extensionCount = 0u;
  vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, nullptr);
  if (extensionCount == 0u)
  {
    return {};
  }
    
  Array_Dynamic<VkExtensionProperties> extensions(extensionCount);
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
