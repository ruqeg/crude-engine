module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.physical_device;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_heap;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Surface;

class Physical_Device : public Vulkan_Object<VkPhysicalDevice>
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
