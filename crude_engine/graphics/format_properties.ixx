module;

#include <vulkan/vulkan.h>

export module crude.graphics.format_properties;

export namespace crude::graphics
{

class Format_Properties : private VkFormatProperties
{
public:
  Format_Properties() = default;
private:
  Format_Properties(VkFormatProperties vkProperties) { *this = vkProperties; }
public:
  bool checkFormatSupport(VkImageTiling tiling, VkFormatFeatureFlags features) const
  {
    if (tiling == VK_IMAGE_TILING_LINEAR && (this->linearTilingFeatures & features) == features)
      return true;
    else if (tiling == VK_IMAGE_TILING_OPTIMAL && (this->optimalTilingFeatures & features) == features)
      return true;
    return false;
  }
private:
  friend class Physical_Device;
};

}