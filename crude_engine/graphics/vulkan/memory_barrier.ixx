module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.memory_barrier;

export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Memory_Barrier : public VkMemoryBarrier
{
public:
  explicit Memory_Barrier(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask)
	: VkMemoryBarrier {
	  .sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER,
	  .pNext         = nullptr,
	  .srcAccessMask = srcAccessMask,
	  .dstAccessMask = dstAccessMask }
  {}
};

}