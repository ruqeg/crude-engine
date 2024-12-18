module;

#include <vulkan/vulkan.hpp>
#include <flecs.h>

export module crude.gfx.gbuffer_pass;

export import crude.core.std_containers_heap;

export namespace crude::gfx
{

class Render_Pass;
class Render_Graph;

}


export namespace crude::gfx
{

void initializeGbufferPass(core::shared_ptr<Render_Graph> graph, flecs::world world, flecs::entity cameraNode);

}