module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.flush;

export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Command_Buffer;

void flush(core::shared_ptr<Command_Buffer> commandBuffer);

}