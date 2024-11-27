module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.image_subresource_range;

import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Image;

class Image_Subresource_Range : public VkImageSubresourceRange
{
public:
  explicit Image_Subresource_Range(core::shared_ptr<const Image>  image);
  explicit Image_Subresource_Range(core::shared_ptr<const Image>  image,
                                   core::uint32                   baseMipLevel,
                                   core::uint32                   levelCount,
                                   core::uint32                   baseArrayLayer,
                                   core::uint32                   layerCount);
};

}