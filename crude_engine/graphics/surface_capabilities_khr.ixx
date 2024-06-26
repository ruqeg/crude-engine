module;

#include <vulkan/vulkan.h>
#include <limits>
#include <algorithm>

export module crude.graphics.surface_capabilities_khr;

export import crude.core.alias;

export namespace crude::graphics
{

class Surface_Capabilities_KHR
{
public:
  Surface_Capabilities_KHR() = default;
  explicit Surface_Capabilities_KHR(VkSurfaceCapabilitiesKHR capabilities) noexcept : m_capabilities(capabilities) {}
public:
  core::uint32 getMinImageCount() const { return  m_capabilities.minImageCount; }
  core::uint32 getMaxImageCount() const { return  m_capabilities.maxImageCount; }
  const VkSurfaceTransformFlagBitsKHR& getCurrentTransform() const { return m_capabilities.currentTransform; }
  VkExtent2D calculateSurfaceExtentInPixels(VkExtent2D actualExtentInPixels)
  {
    if (m_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
      return m_capabilities.currentExtent;

    actualExtentInPixels.width = std::clamp(actualExtentInPixels.width, m_capabilities.minImageExtent.width, m_capabilities.maxImageExtent.width);
    actualExtentInPixels.height = std::clamp(actualExtentInPixels.height, m_capabilities.minImageExtent.height, m_capabilities.maxImageExtent.height);
    return actualExtentInPixels;
  }
private:
  VkSurfaceCapabilitiesKHR m_capabilities;
};

}