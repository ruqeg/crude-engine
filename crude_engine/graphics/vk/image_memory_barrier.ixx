module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.image_memory_barrier;

export import crude.core.std_containers_heap;
export import crude.gfx.vk.image_subresource_range;

export namespace crude::gfx::vk
{

class Image;

class Image_Memory_Barrier : public VkImageMemoryBarrier
{
public:
  explicit Image_Memory_Barrier(core::shared_ptr<Image>  image,
                                VkImageLayout            newLayout,
                                Image_Subresource_Range  subresourceRange,
                                core::uint32             srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                core::uint32             dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED);
private:
  core::shared_ptr<Image>  m_image;

public:
  friend class Command_Buffer;
};

}