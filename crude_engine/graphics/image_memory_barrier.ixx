module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.image_memory_barrier;

import crude.core.shared_ptr;
import crude.graphics.vulkan.image_subresource_range;

export namespace crude::graphics
{

class Image;

class Image_Memory_Barrier : public VkImageMemoryBarrier
{
public:
  explicit Image_Memory_Barrier(core::Shared_Ptr<Image>  image,
                                VkImageLayout            newLayout,
                                Image_Subresource_Range  subresourceRange,
                                core::uint32             srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                core::uint32             dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED);
private:
  core::Shared_Ptr<Image>  m_image;

public:
  friend class Command_Buffer;
};

}