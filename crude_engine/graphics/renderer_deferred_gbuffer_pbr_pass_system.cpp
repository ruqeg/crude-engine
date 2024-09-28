#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <cstring>
#include <algorithm>
#include <DirectXMath.h>
#include <crude_shaders/deferred_gbuffer.frag.inl>
#include <crude_shaders/deferred_gbuffer.mesh.inl>
#include <crude_shaders/deferred_gbuffer.task.inl>
#include <functional>

module crude.graphics.renderer_deferred_gbuffer_pbr_pass_system;

import crude.core.logger;
import crude.scene.image;
import crude.graphics.format_helper;
import crude.graphics.generate_mipmaps;
import crude.graphics.flush;
import crude.graphics.mesh_buffer;
import crude.scene.mesh;
import crude.graphics.render_pass;
import crude.graphics.command_buffer;
import crude.graphics.framebuffer;
import crude.graphics.pipeline;
import crude.graphics.swap_chain;
import crude.graphics.descriptor_pool;
import crude.graphics.descriptor_set_layout;
import crude.graphics.semaphore;
import crude.graphics.fence;
import crude.platform.sdl_window_container;
import crude.graphics.physical_device;
import crude.graphics.queue;
import crude.graphics.instance;
import crude.graphics.device;
import crude.graphics.surface;
import crude.graphics.swap_chain;
import crude.graphics.swap_chain_image;
import crude.graphics.image_view;
import crude.graphics.uniform_buffer;
import crude.graphics.material;
import crude.graphics.texture;
import crude.graphics.debug_utils_messenger;
import crude.graphics.command_pool;
import crude.graphics.image_attachment;
import crude.graphics.shader_module;
import crude.graphics.storage_buffer;
import crude.graphics.attachment_description;
import crude.graphics.gbuffer;

namespace crude::graphics
{

void rendererDeferredGBufferPbrPassSystemProcess(flecs::iter& it)
{
  Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx* deferredGBufferCtx = it.ctx<Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx>();
  core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx = deferredGBufferCtx->frameCtx;
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  
  core::array<VkClearValue, 4u> clearValues;
  clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
  clearValues[1].color = { {0.0f, 0.5f, 0.0f, 1.0f} };
  clearValues[2].color = { {0.0f, 1.0f, 0.0f, 1.0f} };
  clearValues[3].depthStencil = { 1.0f, 0 };

  VkRect2D renderArea;
  renderArea.extent = coreCtx->swapchain->getExtent();
  renderArea.offset = VkOffset2D { 0, 0 };

  frameCtx->getFrameGraphicsCommandBuffer()->setViewport(Viewport({
    .x = 0.0f, .y = 0.0f,
    .width = static_cast<core::float32>(coreCtx->swapchain->getExtent().width),
    .height = static_cast<core::float32>(coreCtx->swapchain->getExtent().height),
    .minDepth = 0.0f, .maxDepth = 1.0f }));

  frameCtx->getFrameGraphicsCommandBuffer()->setScissor(Scissor({
    .offset = { 0, 0 },
    .extent = coreCtx->swapchain->getExtent() }));

  frameCtx->getFrameGraphicsCommandBuffer()->beginRenderPass(deferredGBufferCtx->pipeline->getRenderPass(), deferredGBufferCtx->framebuffers[frameCtx->swapchainImageIndex], clearValues, renderArea);
  frameCtx->getFrameGraphicsCommandBuffer()->bindPipeline(deferredGBufferCtx->pipeline);

  deferredGBufferCtx->perFrameBufferDescriptors[frameCtx->currentFrame].update(frameCtx->getFramePerFrameUniformBuffer());

  while (it.next())
  {
    auto meshBuffers = it.field<core::shared_ptr<Mesh_Buffer>>(0);
    auto meshes = it.field<core::shared_ptr<scene::Mesh>>(1);

    for (auto i : it)
    {
      deferredGBufferCtx->submeshesDrawsBufferDescriptor.update(meshBuffers[i]->getSubmeshesDrawsBuffer(), meshBuffers[i]->getSubmeshesDrawsBuffer()->getSize());
      deferredGBufferCtx->vertexBufferDescriptor.update(meshBuffers[i]->getVerticesBuffer(), meshBuffers[i]->getVerticesBuffer()->getSize());
      deferredGBufferCtx->meshletBufferDescriptor.update(meshBuffers[i]->getMeshletsBuffer(), meshBuffers[i]->getMeshletsBuffer()->getSize());
      deferredGBufferCtx->primitiveIndicesBufferDescriptor.update(meshBuffers[i]->getPrimitiveIndicesBuffer(), meshBuffers[i]->getPrimitiveIndicesBuffer()->getSize());
      deferredGBufferCtx->vertexIndicesBufferDescriptor.update(meshBuffers[i]->getVertexIndicesBuffer(), meshBuffers[i]->getVertexIndicesBuffer()->getSize());

      Per_Mesh perMesh;
      if (it.entity(i).has<scene::Transform>())
      {
        auto transform = it.entity(i).get_ref<scene::Transform>();
        perMesh.modelToWorld = transform->getNodeToWorldFloat4x4();
      }

      for (core::uint32 submeshIndex = 0u; submeshIndex < meshes[i]->submeshes.size(); ++submeshIndex)
      {
        const scene::Sub_Mesh& submesh = meshes[i]->submeshes[submeshIndex];
        if (!submesh.material || !submesh.material->albedo || !submesh.material->metallicRoughness || !submesh.material->normal)
        {
          continue;
        }
        deferredGBufferCtx->submeshAlbedoDescriptors[frameCtx->currentFrame].update(submesh.material->albedo->getImageView(), submesh.material->albedo->getSampler());
        deferredGBufferCtx->submeshMetallicRoughnessDescriptors[frameCtx->currentFrame].update(submesh.material->metallicRoughness->getImageView(), submesh.material->metallicRoughness->getSampler());
        deferredGBufferCtx->submeshNormalDescriptors[frameCtx->currentFrame].update(submesh.material->normal->getImageView(), submesh.material->normal->getSampler());

        core::array<VkWriteDescriptorSet, 9u> descriptorWrites;
        deferredGBufferCtx->perFrameBufferDescriptors[frameCtx->currentFrame].write(descriptorWrites[0]);
        deferredGBufferCtx->submeshAlbedoDescriptors[frameCtx->currentFrame].write(descriptorWrites[1]);
        deferredGBufferCtx->submeshMetallicRoughnessDescriptors[frameCtx->currentFrame].write(descriptorWrites[2]);
        deferredGBufferCtx->submeshNormalDescriptors[frameCtx->currentFrame].write(descriptorWrites[3]);
        deferredGBufferCtx->submeshesDrawsBufferDescriptor.write(descriptorWrites[4]);
        deferredGBufferCtx->vertexBufferDescriptor.write(descriptorWrites[5]);
        deferredGBufferCtx->meshletBufferDescriptor.write(descriptorWrites[6]);
        deferredGBufferCtx->primitiveIndicesBufferDescriptor.write(descriptorWrites[7]);
        deferredGBufferCtx->vertexIndicesBufferDescriptor.write(descriptorWrites[8]);

        frameCtx->getFrameGraphicsCommandBuffer()->pushDescriptorSet(deferredGBufferCtx->pipeline, descriptorWrites);
        perMesh.submeshIndex = submeshIndex;
        frameCtx->getFrameGraphicsCommandBuffer()->pushConstant(deferredGBufferCtx->pipeline->getPipelineLayout(), perMesh);

        const core::size_t taskWorkGroupSize = 1 + submesh.lods[0].meshletCount / 128;
        frameCtx->getFrameGraphicsCommandBuffer()->drawMeshTasks(taskWorkGroupSize);
      }
    }
  }
  
  frameCtx->getFrameGraphicsCommandBuffer()->endRenderPass();
}

Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx::Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx)
  : perFrameBufferDescriptors{ cPerFrameUniformBufferDescriptor, cPerFrameUniformBufferDescriptor }
  , submeshAlbedoDescriptors{ cSubmeshAlbedoDescriptor, cSubmeshAlbedoDescriptor }
  , submeshMetallicRoughnessDescriptors{ cSubmeshMetallicRoughnessDescriptor, cSubmeshMetallicRoughnessDescriptor }
  , submeshNormalDescriptors{ cSubmeshNormalDescriptor, cSubmeshNormalDescriptor }
  , submeshesDrawsBufferDescriptor{ cSubmeshesDrawsBufferDescriptor }
  , vertexBufferDescriptor{ cVertexBufferDescriptor }
  , meshletBufferDescriptor{ cMeshletBufferDescriptor }
  , primitiveIndicesBufferDescriptor{ cPrimitiveIndicesBufferDescriptor }
  , vertexIndicesBufferDescriptor{ cVertexIndicesBufferDescriptor }
  , frameCtx{ frameCtx }
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  gbuffer = core::allocateShared<GBuffer>(coreCtx->device, coreCtx->swapchain->getExtent());

  initializeRenderPass();
  initalizeGraphicsPipeline();
  initializeFramebuffers();
}


core::shared_ptr<Descriptor_Set_Layout> Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx::createDescriptorSetLayout()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  auto descriptorPool = core::allocateShared<Descriptor_Pool>(coreCtx->device, getDescriptorPoolSizes(), 2);
  auto descriptorSetLayout = core::allocateShared<Descriptor_Set_Layout>(coreCtx->device, getDescriptorLayoutBindings(), true);
  return descriptorSetLayout;
}

void Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx::initializeRenderPass()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  core::array<Subpass_Dependency, 1u> subpassesDependencies =
  {
    Subpass_Dependency({
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .srcAccessMask = 0u,
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0})
  };
  renderPass = core::allocateShared<Render_Pass>(coreCtx->device, getSubpassDescriptions(), subpassesDependencies, getAttachmentsDescriptions());
}

void Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx::initalizeGraphicsPipeline()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  core::shared_ptr<Shader_Module> taskShaderModule = core::allocateShared<Shader_Module>(coreCtx->device, crude::shaders::deferred_gbuffer::task);
  core::shared_ptr<Shader_Module> meshShaderModule = core::allocateShared<Shader_Module>(coreCtx->device, crude::shaders::deferred_gbuffer::mesh);
  core::shared_ptr<Shader_Module> fragShaderModule = core::allocateShared<Shader_Module>(coreCtx->device, crude::shaders::deferred_gbuffer::frag);

  core::array<Shader_Stage_Create_Info, 3u> shaderStagesInfo =
  {
    Task_Shader_Stage_Create_Info(taskShaderModule, "main"),
    Mesh_Shader_Stage_Create_Info(meshShaderModule, "main"),
    Fragment_Shader_Stage_Create_Info(fragShaderModule, "main"),
  };

  Vertex_Input_State_Create_Info vertexInputStateInfo({
    .bindings = {},
    .attributes = {} });

  Tessellation_State_Create_Info tesselationInfo(3u);

  Input_Assembly_State_Create_Info inputAssembly({
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    .primitiveRestartEnable = false });

  Viewport_State_Create_Info viewportState(1u, 1u);

  Rasterization_State_Create_Info rasterizer({
    .depthClampEnable = false,
    .rasterizerDiscardEnable = false,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .cullMode = VK_CULL_MODE_BACK_BIT,
    .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
    .depthBiasEnable = false,
    .depthBiasConstantFactor = 0.0f,
    .depthBiasClamp = 0.0f,
    .depthBiasSlopeFactor = 0.0f,
    .lineWidth = 1.f });

  Multisample_State_Create_Info multisampling({
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable = VK_FALSE,
    .minSampleShading = 0.2f,
    .alphaToCoverageEnable = false,
    .alphaToOneEnable = false });

  core::array<VkDynamicState, 2> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
  };
  Dynamic_State_Create_Info dynamicStateInfo(dynamicStates);

  core::shared_ptr<Pipeline_Layout> pipelineLayout = core::allocateShared<Pipeline_Layout>(
    coreCtx->device, createDescriptorSetLayout(),
    Push_Constant_Range<Per_Mesh>(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT));

  pipeline = core::allocateShared<Pipeline>(
    coreCtx->device,
    renderPass,
    pipelineLayout,
    nullptr,
    shaderStagesInfo,
    vertexInputStateInfo,
    tesselationInfo,
    inputAssembly,
    viewportState,
    rasterizer,
    multisampling,
    createDepthStencilStateCreateInfo(),
    createColorBlendStateCreateInfo(),
    dynamicStateInfo,
    0u);
}

void Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx::initializeFramebuffers()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  framebuffers.reserve(coreCtx->swapchainImages.size());
  for (core::uint32 i = 0; i < coreCtx->swapchainImages.size(); ++i)
  {
    framebuffers.push_back(core::allocateShared<Framebuffer>(
      coreCtx->device, renderPass, getFramebufferAttachments(), gbuffer->getExtent(), 1u));
  }
}

core::array<Subpass_Description, 1> Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx::getSubpassDescriptions()
{
  return
  {
    Subpass_Description(Subpass_Description::Initialize_Color_Array_Depth{
      core::array<VkImageLayout,3>
      {
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      },
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL })
  };
}

core::vector<Attachment_Description> Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx::getAttachmentsDescriptions()
{
  return
  {
    Attachment_Description({
      .format        = gbuffer->getAlbedoAttachment()->getFormat(),
      .samples       = gbuffer->getAlbedoAttachment()->getSampleCount(),
      .colorOp       = attachment_op::gClearStore,
      .stenicilOp    = attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }),
    Attachment_Description({
      .format        = gbuffer->getMetallicRoughnessAttachment()->getFormat(),
      .samples       = gbuffer->getMetallicRoughnessAttachment()->getSampleCount(),
      .colorOp       = attachment_op::gClearStore,
      .stenicilOp    = attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }),
    Attachment_Description({
      .format        = gbuffer->getNormalAttachment()->getFormat(),
      .samples       = gbuffer->getNormalAttachment()->getSampleCount(),
      .colorOp       = attachment_op::gClearStore,
      .stenicilOp    = attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }),
    Attachment_Description({
      .format        = gbuffer->getDepthStencilAttachment()->getFormat(),
      .samples       = gbuffer->getDepthStencilAttachment()->getSampleCount(),
      .colorOp       = attachment_op::gClearStore,
      .stenicilOp    = attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL })
  };
}

core::vector<core::shared_ptr<Image_View>> Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx::getFramebufferAttachments()
{
  return { gbuffer->getAlbedoAttachmentView(), gbuffer->getMetallicRoughnessAttachmentView(), gbuffer->getNormalAttachmentView(), gbuffer->getDepthStencilAttachmentView() };
}

Color_Blend_State_Create_Info Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx::createColorBlendStateCreateInfo()
{
  core::array<Pipeline_Color_Blend_Attachment_State, 3u> colorAttachments =
  {
    Pipeline_Color_Blend_Attachment_State({
      .blendEnable         = VK_TRUE,
      .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .colorBlendOp        = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp        = VK_BLEND_OP_ADD }),
    Pipeline_Color_Blend_Attachment_State({
      .blendEnable         = VK_FALSE,
      .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .colorBlendOp        = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp        = VK_BLEND_OP_ADD }),
    Pipeline_Color_Blend_Attachment_State({
      .blendEnable         = VK_FALSE,
      .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .colorBlendOp        = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp        = VK_BLEND_OP_ADD })
  };
  return Color_Blend_State_Create_Info({
    .attachments = colorAttachments,
    .blendConstants = { 0.f, 0.f, 0.f, 0.f },
    .logicOpEnable = false,
    .logicOp = VK_LOGIC_OP_COPY });
}

Depth_Stencil_State_Create_Info Renderer_Deferred_GBuffer_PBR_Pass_Systen_Ctx::createDepthStencilStateCreateInfo()
{
  return Depth_Stencil_State_Create_Info({
    .depthTestEnable       = true,
    .depthWriteEnable      = true,
    .depthCompareOp        = VK_COMPARE_OP_LESS,
    .depthBoundsTestEnable = false,
    .stencilTestEnable     = false,
    .front                 = {},
    .back                  = {},
    .minDepthBounds        = 0.0f,
    .maxDepthBounds        = 1.0f });
}

}