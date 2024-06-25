module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.physical_device;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.format;
export import crude.graphics.format_properties;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Surface;

class Physical_Device : public Vulkan_Object<VkPhysicalDevice>
{
public:
  explicit Physical_Device(VkPhysicalDevice vkPhysicalDevice);
  bool checkPresentSupport(core::shared_ptr<const Surface> surface, core::uint32 queueFamilyIndex) const;
  bool checkSurfaceSupport(core::shared_ptr<const Surface> surface) const;
  VkSurfaceCapabilitiesKHR getSurfaceCapabilitis(core::shared_ptr<const Surface> surface) const;
  core::vector<VkSurfaceFormatKHR> getSurfaceFormats(core::shared_ptr<const Surface> surface) const;
  core::vector<VkPresentModeKHR> getSurfacePresentModes(core::shared_ptr<const Surface> surface) const;
  core::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;
  core::vector<VkExtensionProperties> getExtensionProperties() const;
  bool checkExtensionSupport(core::span<const char* const> extensions) const;
  Format_Properties getFormatProperties(Format format) const;
  VkPhysicalDeviceProperties getProperties() const;
  VkPhysicalDeviceFeatures getFeatures() const;
  VkPhysicalDeviceMemoryProperties getMemoryProperties() const;
private:
  VkPhysicalDeviceProperties m_properties;
};

}
