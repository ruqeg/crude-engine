module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.image_memory_barrier;

import crude_engine.core.shared_ptr;
import crude_engine.graphics.vulkan.image_subresource_range;

export namespace crude_engine
{

class Image;

class Image_Memory_Barrier : public VkImageMemoryBarrier
{
public:
  explicit Image_Memory_Barrier(Shared_Ptr<Image>        image,
                                VkImageLayout            newLayout,
                                Image_Subresource_Range  subresourceRange,
                                uint32                   srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                uint32                   dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED);
private:
  Shared_Ptr<Image>  m_image;

public:
  friend class Command_Buffer;
};

}