#include <vulkan/vulkan.h>

module crude.gfx.vk.format_helper;

import crude.gfx.vk.physical_device;
import crude.core.logger;

namespace crude::gfx::vk
{

Format findDepthFormatSupportedByDevice(core::shared_ptr<const Physical_Device> physicalDevice, core::span<const Format> candidates) noexcept
{
  return findFormatSupportedByDevice(
    physicalDevice,
    candidates,
    VK_IMAGE_TILING_OPTIMAL,
    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

Format findFormatSupportedByDevice(core::shared_ptr<const Physical_Device> physicalDevice, core::span<const Format> candidates, VkImageTiling tiling, VkFormatFeatureFlags features) noexcept
{
  for (Format format : candidates)
  {
    if (physicalDevice->getFormatProperties(format).checkFormatSupport(tiling, features))
    {
      return format;
    }
  }
  core::logError(core::Debug::Channel::Graphics, "Failed to find supported format!");
  return Format(VK_FORMAT_UNDEFINED);
}

}