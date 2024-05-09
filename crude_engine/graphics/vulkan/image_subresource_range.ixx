module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.image_subresource_range;

import crude_engine.core.shared_ptr;

export namespace crude_engine
{

class Image;

class Image_Subresource_Range : public VkImageSubresourceRange
{
public:
  explicit Image_Subresource_Range(Shared_Ptr<Image>  image,
                                   uint32             baseMipLevel   = 0u,
                                   uint32             levelCount     = VK_REMAINING_MIP_LEVELS,
                                   uint32             baseArrayLayer = 0u,
                                   uint32             layerCount     = VK_REMAINING_ARRAY_LAYERS);
};

}