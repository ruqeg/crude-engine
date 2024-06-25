module;

#include <vulkan/vulkan.h>

export module crude.graphics.format_helper;

export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
export import crude.graphics.format;

export namespace crude::graphics
{

namespace depth_formats
{
constexpr core::array<Format, 2> gDepthCandidates{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM };
constexpr core::array<Format, 2> gDepthStencilCandidates{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
}

class Physical_Device;

Format findDepthFormat(core::shared_ptr<const Physical_Device> physicalDevice, core::span<const Format> candidates = depth_formats::gDepthStencilCandidates) noexcept;

Format findSupportedFormat(core::shared_ptr<const Physical_Device>  physicalDevice,
                           core::span<const Format>                 candidates, 
                           VkImageTiling                            tiling, 
                           VkFormatFeatureFlags                     features) noexcept;

}
