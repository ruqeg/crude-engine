#pragma once

#include <core/shared_ptr.hpp>
#include <core/array_unsafe.hpp>
#include <core/std_containers.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Surface;

class Physical_Device : public TObject<VkPhysicalDevice>
{
public:
  explicit Physical_Device(VkPhysicalDevice vkPhysicalDevice);
  bool getSupportSurface(Shared_Ptr<const Surface> surface, uint32 queueFamilyIndex) const;
  VkSurfaceCapabilitiesKHR getSurfaceCapabilitis(Shared_Ptr<const Surface> surface) const;
  vector<VkSurfaceFormatKHR> getSurfaceFormats(Shared_Ptr<const Surface> surface);
  vector<VkPresentModeKHR> getSurfacePresentModes(Shared_Ptr<const Surface> surface);
  vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;
  vector<VkExtensionProperties> getExtensionProperties() const;
  VkFormatProperties getFormatProperties(VkFormat format) const;
  VkPhysicalDeviceProperties getProperties() const;
  VkPhysicalDeviceFeatures getFeatures() const;
  VkPhysicalDeviceMemoryProperties getMemoryProperties() const;
private:
  VkPhysicalDeviceProperties m_properties;
};

}
