#include <vulkan/vulkan.hpp>

module crude.gfx.vk.physical_device;

import crude.core.logger;
import crude.gfx.vk.surface;
import crude.gfx.vk.instance;

namespace crude::gfx::vk
{
 
Physical_Device::Physical_Device(VkPhysicalDevice vkPhysicalDevice, core::shared_ptr<Instance> instance)
  : m_instance(instance)
{
  m_handle = vkPhysicalDevice;
}

bool Physical_Device::checkPresentSupport(core::shared_ptr<const Surface> surface, core::uint32 queueFamilyIndex) const
{
  VkBool32 presentSupport = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(
    m_handle,
    queueFamilyIndex,
    surface->getHandle(),
    &presentSupport);
  return presentSupport;
}
  
bool Physical_Device::checkSurfaceSupport(core::shared_ptr<const Surface> surface) const
{
  return !getSurfaceFormats(surface).empty() && !getSurfacePresentModes(surface).empty();
}

Surface_Capabilities_KHR Physical_Device::getSurfaceCapabilitis(core::shared_ptr<const Surface> surface) const
{
  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    m_handle, 
    surface->getHandle(),
    &capabilities);
  return Surface_Capabilities_KHR(capabilities);
}
   
core::vector<VkSurfaceFormatKHR> Physical_Device::getSurfaceFormats(core::shared_ptr<const Surface> surface) const
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

VkSurfaceFormatKHR Physical_Device::findSurfaceFormat(core::shared_ptr<const Surface> surface, VkFormat format, VkColorSpaceKHR colorSpace) const const
{
  core::vector<VkSurfaceFormatKHR> availableFormats = getSurfaceFormats(surface);
  if (availableFormats.size() < 1)
  {
    core::logError(core::Debug::Channel::Graphics, "Can't find surface format");
    return {};
  }

  for (const auto& availableFormat : availableFormats)
  {
    if (availableFormat.format == format && availableFormat.colorSpace == colorSpace)
      return availableFormat;
  }

  return availableFormats[0];
}

core::vector<VkPresentModeKHR> Physical_Device::getSurfacePresentModes(core::shared_ptr<const Surface> surface) const
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

VkPresentModeKHR Physical_Device::findSurfacePresentMode(core::shared_ptr<const Surface> surface, VkPresentModeKHR presentMode) const
{
  core::vector<VkPresentModeKHR> availablePresentModes = getSurfacePresentModes(surface);
  for (VkPresentModeKHR availablePresentMode : availablePresentModes)
  {
    if (availablePresentMode == presentMode)
      return availablePresentMode;
  }
  return VK_PRESENT_MODE_FIFO_KHR;
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

bool Physical_Device::checkExtensionSupport(core::span<const char* const> extensions) const
{
  core::vector<VkExtensionProperties> availableExtensions = getExtensionProperties();

  // !TODO
  core::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
  for (const auto& extension : availableExtensions)
  {
    requiredExtensions.erase(extension.extensionName);
  }
  return requiredExtensions.empty();
}
  
Format_Properties Physical_Device::getFormatProperties(Format format) const
{
  VkFormatProperties properties;
  vkGetPhysicalDeviceFormatProperties(m_handle, format, &properties);
  return Format_Properties(properties);
}
  
Physical_Device_Properties Physical_Device::getProperties() const
{
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(m_handle, &properties);
  return Physical_Device_Properties(properties);
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
