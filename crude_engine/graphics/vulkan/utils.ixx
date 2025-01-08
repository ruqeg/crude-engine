module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.utils;

export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{
class Buffer;
}

export namespace crude::gfx::vk
{

VkDeviceOrHostAddressConstKHR getAddress(const core::shared_ptr<const Buffer> buffer);

}
