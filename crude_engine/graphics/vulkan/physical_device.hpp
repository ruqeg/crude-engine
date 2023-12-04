#pragma once

#include "../../core/core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include <vector>

namespace crude_engine
{

class Surface;

struct Physical_Device_Create_Info
{
  VkPhysicalDevice vkPhysicalDevice;
  explicit Physical_Device_Create_Info(VkPhysicalDevice vkPhysicalDevice);
};

class Physical_Device : public TObject<VkPhysicalDevice>
{
public:
  explicit Physical_Device(const Physical_Device_Create_Info& createInfo);
  bool getSupportSurface(std::shared_ptr<const Surface> surface, uint32 queueFamilyIndex) const;
  VkSurfaceCapabilitiesKHR getSurfaceCapabilitis(std::shared_ptr<const Surface> surface) const;
  std::vector<VkSurfaceFormatKHR> getSurfaceFormats(std::shared_ptr<const Surface> surface);
  std::vector<VkPresentModeKHR> getSurfacePresentModes(std::shared_ptr<const Surface> surface);
  std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;
  std::vector<VkExtensionProperties> getExtensionProperties() const;
  VkFormatProperties getFormatProperties(VkFormat format) const;
  VkPhysicalDeviceProperties getProperties() const;
  VkPhysicalDeviceFeatures getFeatures() const;
  VkPhysicalDeviceMemoryProperties getMemoryProperties() const;
private:
  VkPhysicalDeviceProperties m_properties;
};

}
