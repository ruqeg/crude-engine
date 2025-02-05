#include <vulkan/vulkan.h>
#include <flecs.h>
#include <DirectXMath.h>

#include <crude_shaders/miss.rmiss.inl>
#include <crude_shaders/trace.rgen.inl>
#include <crude_shaders/hit.rchit.inl>

module crude.gfx.raytracing_pass;

import crude.core.std_containers_stack;
import crude.gfx.render_graph;
import crude.gfx.mesh_buffer;
import crude.gfx.material;
import crude.gfx.texture;
import crude.gfx.vk.graphics_pipeline;
import crude.gfx.vk.pipeline_layout;
import crude.gfx.vk.device;
import crude.gfx.vk.format_helper;
import crude.gfx.vk.shader_module;
import crude.gfx.vk.buffer_descriptor;
import crude.gfx.vk.vertex_buffer;
import crude.gfx.vk.image_descriptor;
import crude.gfx.vk.descriptor_pool;
import crude.gfx.vk.acceleration_structure_input_buffer;
import crude.gfx.vk.acceleration_structure_geometry;
import crude.gfx.vk.bottom_level_acceleration_structure;
import crude.gfx.vk.acceleration_structure_instance_buffer;
import crude.gfx.vk.acceleration_structure_instance;
import crude.gfx.vk.top_level_acceleration_structure;
import crude.gfx.vk.storage_buffer;
import crude.gfx.vk.flush;
import crude.gfx.vk.descriptor_set_layout;
import crude.gfx.vk.descriptor_set;
import crude.gfx.vk.ray_tracing_pipeline;
import crude.scene.mesh;
import crude.gfx.vk.acceleration_structure_descriptor;

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
  vk::Acceleration_Structure_Geometry_Triangles geometry = vk::Acceleration_Structure_Geometry_Triangles(VK_FORMAT_R32G32B32_SFLOAT, 3 * sizeof(core::uint32), vertexBuffer);
  core::shared_ptr<vk::Bottom_Level_Acceleration_Structure> bottomLevel = core::allocateShared<vk::Bottom_Level_Acceleration_Structure>(
    device,
    core::array<vk::Acceleration_Structure_Geometry, 1>{ geometry },
    VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
    VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR);
  core::shared_ptr<vk::Acceleration_Structure_Instance_Buffer<vk::Acceleration_Structure_Instance>> instanceBuffer = core::allocateShared<vk::Acceleration_Structure_Instance_Buffer<vk::Acceleration_Structure_Instance>>(device, 1u);
  vk::Acceleration_Structure_Geometry_Instances geometryInstaces = vk::Acceleration_Structure_Geometry_Instances{ instanceBuffer };
  instanceBuffer->getInstance(0).accelerationStructureReference = bottomLevel->getReference();
  core::shared_ptr<vk::Top_Level_Acceleration_Structure> topLevel = core::allocateShared<vk::Top_Level_Acceleration_Structure>(
    device,
    core::array<vk::Acceleration_Structure_Geometry, 1>{ geometryInstaces },
    VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
    VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR);

  const VkDeviceSize size = std::max(bottomLevel->getBuildScratchSize(), topLevel->getBuildScratchSize());
  core::shared_ptr<vk::Storage_Buffer> scratchBuffer = core::allocateShared<vk::Storage_Buffer>(commandBuffer, size);
  commandBuffer->begin();
  commandBuffer->barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR, vk::Memory_Barrier{ VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR });
  commandBuffer->buildAccelerationStructures(bottomLevel, core::array<vk::Acceleration_Structure_Geometry, 1>{ geometry }, scratchBuffer);
  commandBuffer->barrier(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR, VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR, vk::Memory_Barrier{ VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR });
  commandBuffer->buildAccelerationStructures(topLevel, core::array<vk::Acceleration_Structure_Geometry, 1>{ geometryInstaces }, scratchBuffer);
  commandBuffer->end();
  vk::flush(commandBuffer);

  core::array<vk::Descriptor_Set_Layout_Binding, 1> bindings =
  {
    vk::Acceleration_Structure_Descriptor(0u, VK_SHADER_STAGE_RAYGEN_BIT_KHR)
  };
  core::shared_ptr<vk::Descriptor_Set_Layout> descriptorSetLayout = core::allocateShared<vk::Descriptor_Set_Layout>(device, bindings);
  core::shared_ptr<vk::Descriptor_Set> descriptorSet = core::allocateShared<vk::Descriptor_Set>(descriptorPool, descriptorSetLayout);
  core::shared_ptr<vk::Pipeline_Layout> pipelineLayout = core::allocateShared<vk::Pipeline_Layout>(device, descriptorSetLayout);
  core::vector<vk::Ray_Tracing_Shader_Group> shaderGroups =
  {
    vk::General_Ray_Tracing_Shader_Group(0u),
    vk::Triangles_Hit_Ray_Tracing_Shader_Group(1u),
    vk::General_Ray_Tracing_Shader_Group(2u),
  };
  core::vector<core::shared_ptr<vk::Shader_Module>> shaderModules =
  {
    core::allocateShared<vk::Shader_Module>(device, crude::shaders::miss::rmiss),
    core::allocateShared<vk::Shader_Module>(device, crude::shaders::hit::rchit),
    core::allocateShared<vk::Shader_Module>(device, crude::shaders::trace::rgen)
  };
  core::vector<vk::Shader_Stage_Create_Info> shaderStages =
  {
    vk::Miss_Shader_Stage_Create_Info(shaderModules[0], "main"),
    vk::Closest_Hit_Shader_Stage_Create_Info(shaderModules[1], "main"),
    vk::Raygen_Shader_Stage_Create_Info(shaderModules[2], "main"),
  };
  core::shared_ptr<vk::Ray_Tracing_Pipeline> pipeline = core::allocateShared<vk::Ray_Tracing_Pipeline>(pipelineLayout, shaderStages, shaderGroups);



}

}