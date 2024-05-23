module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image_subresource_range;

import crude.core.shared_ptr;

export namespace crude::graphics
{

class Image;

class Image_Subresource_Range : public VkImageSubresourceRange
{
public:
  explicit Image_Subresource_Range(core::Shared_Ptr<Image>  image,
                                   core::uint32             baseMipLevel   = 0u,
                                   core::uint32             levelCount     = VK_REMAINING_MIP_LEVELS,
                                   core::uint32             baseArrayLayer = 0u,
                                   core::uint32             layerCount     = VK_REMAINING_ARRAY_LAYERS);
};

}