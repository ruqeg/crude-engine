module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.format_helper;

export import crude.gfx.vk.format_properties;
export import crude.gfx.vk.format;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

namespace depth_formats
{
constexpr core::array<Format, 2> gDepthCandidates{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM };
constexpr core::array<Format, 2> gDepthStencilCandidates{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
}

class Physical_Device;

Format findDepthFormatSupportedByDevice(core::shared_ptr<const Physical_Device> physicalDevice, core::span<const Format> candidates = depth_formats::gDepthStencilCandidates) noexcept;
Format findFormatSupportedByDevice(core::shared_ptr<const Physical_Device> physicalDevice, core::span<const Format> candidates, VkImageTiling tiling, VkFormatFeatureFlags features) noexcept;

}