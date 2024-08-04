module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.renderer_frame_component;

export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Command_Buffer;

constexpr core::uint32 cFramesCount = 2u;

struct Renderer_Frame_Component
{
  core::shared_ptr<Command_Buffer>  commandBuffer;
  core::uint32                      imageIndex;
};

}