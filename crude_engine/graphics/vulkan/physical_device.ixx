module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.physical_device;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.vk.format;
export import crude.gfx.vk.format_properties;
export import crude.gfx.vk.physical_device_properties;
export import crude.gfx.vk.surface_capabilities_khr;
import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{

class Surface;
class Instance;

class Physical_Device : public Vulkan_Object<VkPhysicalDevice>
{
public:
  explicit Physical_Device(VkPhysicalDevice vkPhysicalDevice, core::shared_ptr<Instance> instance);
  bool checkPresentSupport(core::shared_ptr<const Surface> surface, core::uint32 queueFamilyIndex) const;
  bool checkSurfaceSupport(core::shared_ptr<const Surface> surface) const;
  Surface_Capabilities_KHR getSurfaceCapabilitis(core::shared_ptr<const Surface> surface) const;
  core::vector<VkSurfaceFormatKHR> getSurfaceFormats(core::shared_ptr<const Surface> surface) const;
  VkSurfaceFormatKHR findSurfaceFormat(core::shared_ptr<const Surface> surface, VkFormat format, VkColorSpaceKHR colorSpace) const;
  core::vector<VkPresentModeKHR> getSurfacePresentModes(core::shared_ptr<const Surface> surface) const;
  VkPresentModeKHR findSurfacePresentMode(core::shared_ptr<const Surface> surface, VkPresentModeKHR presentMode) const;
  core::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;
  core::vector<VkExtensionProperties> getExtensionProperties() const;
  bool checkExtensionSupport(core::span<const char* const> extensions) const;
  Format_Properties getFormatProperties(Format format) const;
  Physical_Device_Properties getProperties() const;
  VkPhysicalDeviceFeatures getFeatures() const;
  VkPhysicalDeviceMemoryProperties getMemoryProperties() const;
private:
  core::shared_ptr<Instance>  m_instance;
  VkPhysicalDeviceProperties  m_properties;
};

}
