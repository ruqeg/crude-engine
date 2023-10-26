#pragma once

#include "core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include <vector>

namespace crude_vulkan_01
{

class Surface;

struct PhysicalDeviceCreateInfo
{
  VkPhysicalDevice vkPhysicalDevice;
  explicit PhysicalDeviceCreateInfo(VkPhysicalDevice vkPhysicalDevice);
};

class Physical_Device : public TObject<VkPhysicalDevice>
{
public:
  explicit Physical_Device(const PhysicalDeviceCreateInfo& createInfo);
  bool getSupportSurface(std::shared_ptr<const Surface> surface, uint32 queueFamilyIndex) const;
  VkSurfaceCapabilitiesKHR getSurfaceCapabilitis(std::shared_ptr<const Surface> surface) const;
  std::vector<VkSurfaceFormatKHR> getSurfaceFormats(std::shared_ptr<const Surface> surface);
  std::vector<VkPresentModeKHR> getSurfacePresentModes(std::shared_ptr<const Surface> surface);
  std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;
  std::vector<VkExtensionProperties> getExtensionProperties() const;
  VkFormatProperties getFormatProperties(VkFormat format) const;
  VkPhysicalDeviceProperties getProperties() const;
  VkPhysicalDeviceFeatures getFeatures() const;
private:
  VkPhysicalDeviceProperties m_properties;
  VkPhysicalDeviceFeatures m_features;
  std::vector<VkExtensionProperties> m_extensionProperties;
};

}
