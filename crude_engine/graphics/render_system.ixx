module;

#include <vulkan/vulkan.hpp>
#include <flecs.h>

export module crude.graphics.renderer_system;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.descriptor_set_layout_binding;
export import crude.graphics.buffer_descriptor;
export import crude.graphics.image_descriptor;

export namespace crude::scene
{

struct Mesh;

}

export namespace crude::graphics
{

class Mesh_Buffer;
class Pipeline;
class Command_Buffer;

struct Render_Component
{
  core::shared_ptr<Pipeline>            pipeline;
  core::shared_ptr<Command_Buffer>      commandBuffer;
  Uniform_Buffer_Descriptor*             perFrameUniformBufferDescriptor;
  Combined_Image_Sampler_Descriptor*     textureSamplerDescriptor;
  graphics::Storage_Buffer_Descriptor*   submeshesDrawsBufferDescriptor;
  graphics::Storage_Buffer_Descriptor*   vertexBufferDescriptor;
  graphics::Storage_Buffer_Descriptor*   meshletBufferDescriptor;
  graphics::Storage_Buffer_Descriptor*   primitiveIndicesBufferDescriptor;
  graphics::Storage_Buffer_Descriptor*   vertexIndicesBufferDescriptor;
};

void rendereSystemEach(flecs::iter& it, size_t index, core::shared_ptr<scene::Mesh> mesh, core::shared_ptr<graphics::Mesh_Buffer> meshBuffer);

}