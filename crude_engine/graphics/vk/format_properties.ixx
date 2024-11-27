module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.format_properties;

export namespace crude::gfx::vk
{

class Format_Properties
{
public:
  Format_Properties() = default;
  explicit Format_Properties(VkFormatProperties properties) noexcept : m_properties(properties) {}
public:
  bool checkFormatSupport(VkImageTiling tiling, VkFormatFeatureFlags features) const
  {
    if (tiling == VK_IMAGE_TILING_LINEAR && (m_properties.linearTilingFeatures & features) == features)
      return true;
    else if (tiling == VK_IMAGE_TILING_OPTIMAL && (m_properties.optimalTilingFeatures & features) == features)
      return true;
    return false;
  }
private:
  VkFormatProperties m_properties;
};

}