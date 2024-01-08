#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <core/data_structures/array_unsafe.hpp>
#include <core/data_structures/array_dynamic.hpp>
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
  Array_Dynamic<VkSurfaceFormatKHR> getSurfaceFormats(Shared_Ptr<const Surface> surface);
  Array_Dynamic<VkPresentModeKHR> getSurfacePresentModes(Shared_Ptr<const Surface> surface);
  Array_Dynamic<VkQueueFamilyProperties> getQueueFamilyProperties() const;
  Array_Dynamic<VkExtensionProperties> getExtensionProperties() const;
  VkFormatProperties getFormatProperties(VkFormat format) const;
  VkPhysicalDeviceProperties getProperties() const;
  VkPhysicalDeviceFeatures getFeatures() const;
  VkPhysicalDeviceMemoryProperties getMemoryProperties() const;
private:
  VkPhysicalDeviceProperties m_properties;
};

}
