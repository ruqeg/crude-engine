#pragma once

#include "image_subresource_range.hpp"

namespace crude_engine
{

class Image;

class Image_Memory_Barrier : public VkImageMemoryBarrier
{
public:
  explicit Image_Memory_Barrier(std::shared_ptr<Image>   image,
                                VkImageLayout            newLayout,
                                Image_Subresource_Range  subresourceRange,
                                uint32                   srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                uint32                   dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED);
private:
  std::shared_ptr<Image>  m_image;
  friend class Command_Buffer;
};

}