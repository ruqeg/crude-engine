#include <vulkan/vulkan.h>
#include <flecs.h>
#include <DirectXMath.h>

#include <crude_shaders/deferred_gbuffer.frag.inl>
#include <crude_shaders/deferred_gbuffer.mesh.inl>
#include <crude_shaders/deferred_gbuffer.task.inl>

module crude.gfx.gbuffer_pass;

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
import crude.gfx.vk.image_descriptor;
import crude.scene.mesh;

namespace crude::gfx
{

struct Per_Mesh
{
  DirectX::XMFLOAT4X4  modelToWorld;
  core::uint32         submeshIndex;
};

struct Per_Frame
{
  Camera_UBO camera;
};

void initializeGbufferPass(core::shared_ptr<Render_Graph> graph, flecs::world world, flecs::entity cameraNode)
{
  core::shared_ptr<vk::Device> device = graph->getRendererFrame()->getCore()->getDevice();

  Attachment_Info albedo;;
  albedo.format = VK_FORMAT_B8G8R8A8_SRGB;
  albedo.blendAttachmentState = vk::Pipeline_Color_Blend_Attachment_State({
    .blendEnable = VK_TRUE,
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    .colorBlendOp = VK_BLEND_OP_ADD,
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    .alphaBlendOp = VK_BLEND_OP_ADD });

  Attachment_Info normal;
  normal.format = VK_FORMAT_R16G16B16A16_SNORM;
  normal.blendAttachmentState = vk::Pipeline_Color_Blend_Attachment_State({
    .blendEnable = VK_FALSE,
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT,
    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    .colorBlendOp = VK_BLEND_OP_ADD,
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    .alphaBlendOp = VK_BLEND_OP_ADD });

  Attachment_Info metallicRoughness;
  metallicRoughness.format = VK_FORMAT_R8G8_UNORM;
  metallicRoughness.blendAttachmentState = vk::Pipeline_Color_Blend_Attachment_State({
    .blendEnable = VK_FALSE,
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    .colorBlendOp = VK_BLEND_OP_ADD,
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    .alphaBlendOp = VK_BLEND_OP_ADD });
  
  Attachment_Info depth;
  depth.format = vk::findDepthFormatSupportedByDevice(device->getPhysicalDevice(), vk::depth_formats::gDepthCandidates);


  core::shared_ptr<Render_Pass> gbuffer = graph->addPass(VkExtent2D{ 800, 800 }, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT);
  gbuffer->addColorOutput(albedo);
  gbuffer->addColorOutput(normal);
  gbuffer->addColorOutput(metallicRoughness);
  gbuffer->setDepthStencilOutput(depth);
  gbuffer->setPushConstantRange(vk::Push_Constant_Range<Per_Mesh>(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT));
  gbuffer->setShaderStagesInfo({
    vk::Task_Shader_Stage_Create_Info(core::allocateShared<vk::Shader_Module>(device, crude::shaders::deferred_gbuffer::task), "main"),
    vk::Mesh_Shader_Stage_Create_Info(core::allocateShared<vk::Shader_Module>(device, crude::shaders::deferred_gbuffer::mesh), "main"),
    vk::Fragment_Shader_Stage_Create_Info(core::allocateShared<vk::Shader_Module>(device, crude::shaders::deferred_gbuffer::frag), "main"),
    });

  auto perFramesDesc = gbuffer->addUniformInput(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_FRAGMENT_BIT);
  auto submeshesDrawsDesc = gbuffer->addStorageInput(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT);
  auto verticesDesc = gbuffer->addStorageInput(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT);
  auto meshletsDesc = gbuffer->addStorageInput(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT);
  auto primitiveIndicesDesc = gbuffer->addStorageInput(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT);
  auto vertexIndicesDesc = gbuffer->addStorageInput(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT);
  auto albedoDesc = gbuffer->addTextureInput(VK_SHADER_STAGE_FRAGMENT_BIT);
  auto metallicRoughnessDesc = gbuffer->addTextureInput(VK_SHADER_STAGE_FRAGMENT_BIT);
  auto normalDesc = gbuffer->addTextureInput(VK_SHADER_STAGE_FRAGMENT_BIT);

  core::vector<core::shared_ptr<vk::Uniform_Buffer<Per_Frame>>> perFrameUniformBuffers =
  {
    core::allocateShared<vk::Uniform_Buffer<Per_Frame>>(device),
    core::allocateShared<vk::Uniform_Buffer<Per_Frame>>(device),
    core::allocateShared<vk::Uniform_Buffer<Per_Frame>>(device)
  };

  for (auto perFrameUniformBuffer : perFrameUniformBuffers)
  {
    Per_Frame* data = perFrameUniformBuffer->mapUnsafe();
    data->camera = Camera_UBO(cameraNode);
    perFrameUniformBuffer->unmap();
  }

  gbuffer->build(world
    .system<core::shared_ptr<gfx::Mesh_Buffer>, core::shared_ptr<scene::Mesh>>("gbuffer_pbr_pass_system")
    .kind(0)
    .run([rendererFrame = graph->getRendererFrame(), pipeline = gbuffer->getPipeline(), perFrameUniformBuffers, perFramesDesc, submeshesDrawsDesc, verticesDesc, meshletsDesc, primitiveIndicesDesc, vertexIndicesDesc, albedoDesc, metallicRoughnessDesc, normalDesc](flecs::iter& it) {
      const core::uint32 currentFrame = rendererFrame->getCurrentFrame();
      perFramesDesc[currentFrame].update(perFrameUniformBuffers[currentFrame]);

      while (it.next())
      {
        auto meshBuffers = it.field<core::shared_ptr<gfx::Mesh_Buffer>>(0);
        auto meshes = it.field<core::shared_ptr<scene::Mesh>>(1);

        for (auto i : it)
        {
          submeshesDrawsDesc[currentFrame].update(meshBuffers[i]->getSubmeshesDrawsBuffer());
          verticesDesc[currentFrame].update(meshBuffers[i]->getVerticesBuffer());
          meshletsDesc[currentFrame].update(meshBuffers[i]->getMeshletsBuffer());
          primitiveIndicesDesc[currentFrame].update(meshBuffers[i]->getPrimitiveIndicesBuffer());
          vertexIndicesDesc[currentFrame].update(meshBuffers[i]->getVertexIndicesBuffer());

          auto transform = it.entity(i).get_ref<scene::Transform>();
          if (!transform)
            continue;

          Per_Mesh perMesh;
          perMesh.modelToWorld = transform->getNodeToWorldFloat4x4();

          for (core::uint32 submeshIndex = 0u; submeshIndex < meshes[i]->submeshes.size(); ++submeshIndex)
          {
            const scene::Sub_Mesh& submesh = meshes[i]->submeshes[submeshIndex];
            
            const bool isMaterialValid = submesh.material && submesh.material->isValid();
            if (!isMaterialValid)
              continue;

            albedoDesc[currentFrame].update(submesh.material->albedo->getImageView(), submesh.material->albedo->getSampler());
            metallicRoughnessDesc[currentFrame].update(submesh.material->metallicRoughness->getImageView(), submesh.material->metallicRoughness->getSampler());
            normalDesc[currentFrame].update(submesh.material->normal->getImageView(), submesh.material->normal->getSampler());

            core::array<VkWriteDescriptorSet, 9> descriptorWrites;
            perFramesDesc[currentFrame].write(descriptorWrites[0]);
            submeshesDrawsDesc[currentFrame].write(descriptorWrites[1]);
            verticesDesc[currentFrame].write(descriptorWrites[2]);
            meshletsDesc[currentFrame].write(descriptorWrites[3]);
            primitiveIndicesDesc[currentFrame].write(descriptorWrites[4]);
            vertexIndicesDesc[currentFrame].write(descriptorWrites[5]);
            albedoDesc[currentFrame].write(descriptorWrites[6]);
            metallicRoughnessDesc[currentFrame].write(descriptorWrites[7]);
            normalDesc[currentFrame].write(descriptorWrites[8]);

            rendererFrame->getGraphicsCommandBuffer()->pushConstant(pipeline->getPipelineLayout(), perMesh);
            perMesh.submeshIndex = submeshIndex;
            rendererFrame->getGraphicsCommandBuffer()->pushDescriptorSet(pipeline, descriptorWrites);
            rendererFrame->getGraphicsCommandBuffer()->drawMeshTasks(1);
          }
        }
      }
      }));
}

}