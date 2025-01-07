#include <vulkan/vulkan.h>
#include <flecs.h>
#include <DirectXMath.h>


module crude.gfx.raytracing_pass;

import crude.core.std_containers_stack;
import crude.gfx.render_graph;
import crude.gfx.mesh_buffer;
import crude.gfx.material;
import crude.gfx.texture;
import crude.gfx.vk.pipeline;
import crude.gfx.vk.pipeline_layout;
import crude.gfx.vk.device;
import crude.gfx.vk.format_helper;
import crude.gfx.vk.shader_module;
import crude.gfx.vk.buffer_descriptor;
import crude.gfx.vk.vertex_buffer;
import crude.gfx.vk.image_descriptor;
import crude.gfx.vk.descriptor_pool;
import crude.gfx.vk.acceleration_structure_input_buffer;
import crude.scene.mesh;

namespace crude::gfx
{

void initializeRaytracingPass(core::shared_ptr<Render_Graph> graph, flecs::world world, flecs::entity cameraNode)
{
  core::shared_ptr<vk::Device> device = graph->getRendererFrame()->getCore()->getDevice();
  core::vector<vk::Descriptor_Pool_Size> poolSizes =
  {
    vk::Uniform_Buffer_Pool_Size{ 4 },
    vk::Uniform_Buffer_Pool_Size{ 4 },
    vk::Storage_Buffer_Pool_Size{ 10 },
    vk::Combined_Image_Sampler_Pool_Size{ 4 },
    vk::Acceleration_Structure_Pool_Size{ 10 },
  };

  core::shared_ptr<vk::Descriptor_Pool> descriptorPool = core::allocateShared<vk::Descriptor_Pool>(device, poolSizes, 10u);

  core::vector<DirectX::XMFLOAT3> vertices =
  {
      { 0.0f,-0.3f, 0.f},
      {-0.6f, 0.3f, 0.f},
      { 0.6f, 0.3f, 0.f}
  };

  auto commandBuffer = core::allocateShared<gfx::vk::Command_Buffer>(graph->getRendererFrame()->getCore()->getTransferCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  core::shared_ptr<vk::Acceleration_Structure_Input_Buffer> vertexBuffer = core::allocateShared<vk::Acceleration_Structure_Input_Buffer>(commandBuffer, vertices);
  geometry = magma::AccelerationStructureGeometryTriangles(VK_FORMAT_R32G32B32_SFLOAT, vertexBuffer.get());
}

}