#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"

namespace crude_vulkan_01
{

class Image;

class Image_Subresource_Range : public VkImageSubresourceRange
{
public:
  explicit Image_Subresource_Range(std::shared_ptr<Image>  image,
                                   uint32                  baseMipLevel   = 0u,
                                   uint32                  levelCount     = VK_REMAINING_MIP_LEVELS,
                                   uint32                  baseArrayLayer = 0u,
                                   uint32                  layerCount     = VK_REMAINING_ARRAY_LAYERS);
};

}