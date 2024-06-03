module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.physical_device;

import crude.core.shared_ptr;
import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Surface;

class Physical_Device : public Vulkan_Object<VkPhysicalDevice>
{
public:
  explicit Physical_Device(VkPhysicalDevice vkPhysicalDevice);
  bool getSupportSurface(core::Shared_Ptr<const Surface> surface, core::uint32 queueFamilyIndex) const;
  VkSurfaceCapabilitiesKHR getSurfaceCapabilitis(core::Shared_Ptr<const Surface> surface) const;
  core::vector<VkSurfaceFormatKHR> getSurfaceFormats(core::Shared_Ptr<const Surface> surface) const;
  core::vector<VkPresentModeKHR> getSurfacePresentModes(core::Shared_Ptr<const Surface> surface) const;
  core::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;
  core::vector<VkExtensionProperties> getExtensionProperties() const;
  VkFormatProperties getFormatProperties(VkFormat format) const;
  VkPhysicalDeviceProperties getProperties() const;
  VkPhysicalDeviceFeatures getFeatures() const;
  VkPhysicalDeviceMemoryProperties getMemoryProperties() const;
private:
  VkPhysicalDeviceProperties m_properties;
};

}
