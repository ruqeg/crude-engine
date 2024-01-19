#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
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