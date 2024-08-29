#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <cstring>
#include <algorithm>
#include <DirectXMath.h>
#include <crude_shaders/deferred_gbuffer.frag.inl>
#include <crude_shaders/deferred_gbuffer.mesh.inl>
#include <crude_shaders/deferred_gbuffer.task.inl>
#include <functional>

module crude.graphics.renderer_deferred_gbuffer_pass_system;

import crude.core.logger;
import crude.scene.image;
import crude.graphics.format_helper;
import crude.graphics.generate_mipmaps;
import crude.graphics.flush;
import crude.graphics.mesh_buffer;
import crude.scene.mesh;
import crude.resources.gltf_loader;
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
import crude.graphics.debug_utils_messenger;
import crude.graphics.image_view;
import crude.graphics.command_pool;
import crude.graphics.image_attachment;
import crude.graphics.shader_module;
import crude.graphics.renderer_core_system;
import crude.graphics.renderer_frame_system;
import crude.graphics.storage_buffer;
import crude.graphics.attachment_description;
import crude.graphics.gbuffer;

namespace crude::graphics
{

const Uniform_Buffer_Descriptor          cPerFrameUniformBufferDescriptor(0u, VK_SHADER_STAGE_MESH_BIT_EXT);
const Combined_Image_Sampler_Descriptor  cSubmeshTextureDescriptor(6u, VK_SHADER_STAGE_FRAGMENT_BIT);
const Storage_Buffer_Descriptor          cSubmeshesDrawsBufferDescriptor{ 1u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const Storage_Buffer_Descriptor          cVertexBufferDescriptor{ 3u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const Storage_Buffer_Descriptor          cMeshletBufferDescriptor{ 2u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const Storage_Buffer_Descriptor          cPrimitiveIndicesBufferDescriptor{ 4u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };
const Storage_Buffer_Descriptor          cVertexIndicesBufferDescriptor{ 5u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT };

core::array<Descriptor_Set_Layout_Binding, 7u> cDescriptorLayoutBindings =
{
  cPerFrameUniformBufferDescriptor,
  cSubmeshTextureDescriptor,
  cSubmeshesDrawsBufferDescriptor,
  cVertexBufferDescriptor,
  cMeshletBufferDescriptor,
  cPrimitiveIndicesBufferDescriptor,
  cVertexIndicesBufferDescriptor
};

core::array<Descriptor_Pool_Size, 7u> cDescriptorPoolSizes
{
  Uniform_Buffer_Pool_Size(cFramesCount),
  Combined_Image_Sampler_Pool_Size(cFramesCount),
  Storage_Buffer_Pool_Size(1u),
  Storage_Buffer_Pool_Size(1u),
  Storage_Buffer_Pool_Size(1u),
  Storage_Buffer_Pool_Size(1u),
  Storage_Buffer_Pool_Size(1u)
};

struct Per_Mesh
{
  // !TODO Move
  DirectX::XMFLOAT4X4  modelToWorld;
  core::uint32         submeshIndex;
};

Renderer_Deferred_GBuffer_Pass_Systen_Ctx::Renderer_Deferred_GBuffer_Pass_Systen_Ctx()
  : perFrameBufferDescriptors{cPerFrameUniformBufferDescriptor, cPerFrameUniformBufferDescriptor}
  , submeshTextureDescriptors{ cSubmeshTextureDescriptor, cSubmeshTextureDescriptor }
  , submeshesDrawsBufferDescriptor{ cSubmeshesDrawsBufferDescriptor }
  , vertexBufferDescriptor{ cVertexBufferDescriptor }
  , meshletBufferDescriptor{ cMeshletBufferDescriptor }
  , primitiveIndicesBufferDescriptor{ cPrimitiveIndicesBufferDescriptor }
  , vertexIndicesBufferDescriptor{ cVertexIndicesBufferDescriptor }
{}

void rendererDeferredGBufferPassSystemProcess(flecs::iter& it)
{
  Renderer_Deferred_GBuffer_Pass_Systen_Ctx* deferredGBufferCtx = it.ctx<Renderer_Deferred_GBuffer_Pass_Systen_Ctx>();
  Renderer_Frame_System_Ctx* frameCtx = deferredGBufferCtx->frameCtx;
  Renderer_Core_System_Ctx* coreCtx = frameCtx->coreCtx;
  
  core::array<VkClearValue, 2u> clearValues;;
  clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
  clearValues[1].depthStencil = { 1.0f, 0 };

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
        deferredGBufferCtx->submeshTextureDescriptors[frameCtx->currentFrame].update(submesh.texture->getImageView(), submesh.texture->getSampler());

        core::array<VkWriteDescriptorSet, 7u> descriptorWrites;
        deferredGBufferCtx->perFrameBufferDescriptors[frameCtx->currentFrame].write(descriptorWrites[0]);
        deferredGBufferCtx->submeshTextureDescriptors[frameCtx->currentFrame].write(descriptorWrites[1]);
        deferredGBufferCtx->submeshesDrawsBufferDescriptor.write(descriptorWrites[2]);
        deferredGBufferCtx->vertexBufferDescriptor.write(descriptorWrites[3]);
        deferredGBufferCtx->meshletBufferDescriptor.write(descriptorWrites[4]);
        deferredGBufferCtx->primitiveIndicesBufferDescriptor.write(descriptorWrites[5]);
        deferredGBufferCtx->vertexIndicesBufferDescriptor.write(descriptorWrites[6]);

        frameCtx->getFrameGraphicsCommandBuffer()->pushDescriptorSet(deferredGBufferCtx->pipeline, descriptorWrites);
        perMesh.submeshIndex = submeshIndex;
        frameCtx->getFrameGraphicsCommandBuffer()->pushConstant(deferredGBufferCtx->pipeline->getPipelineLayout(), perMesh);
        frameCtx->getFrameGraphicsCommandBuffer()->drawMeshTasks(1u);
      }
    }
  }
  
  frameCtx->getFrameGraphicsCommandBuffer()->endRenderPass();
}

void rendererDeferredGBufferPassSystemInitialize(flecs::iter& it)
{
  Renderer_Deferred_GBuffer_Pass_Systen_Ctx* deferredGBufferCtx = it.ctx<Renderer_Deferred_GBuffer_Pass_Systen_Ctx>();
  Renderer_Frame_System_Ctx* frameCtx = deferredGBufferCtx->frameCtx;
  Renderer_Core_System_Ctx* coreCtx = frameCtx->coreCtx;

  deferredGBufferCtx->gbuffer = core::allocateShared<GBuffer>(coreCtx->device, coreCtx->swapchain->getExtent());

  initializeRenderPass(deferredGBufferCtx);
  initalizeGraphicsPipeline(deferredGBufferCtx);
  initializeFramebuffers(deferredGBufferCtx);
}

core::shared_ptr<Descriptor_Set_Layout> createDescriptorSetLayout(Renderer_Deferred_GBuffer_Pass_Systen_Ctx* deferredGBufferCtx)
{
  Renderer_Core_System_Ctx* coreCtx = deferredGBufferCtx->frameCtx->coreCtx;

  auto descriptorPool = core::allocateShared<Descriptor_Pool>(coreCtx->device, cDescriptorPoolSizes);
  auto descriptorSetLayout = core::allocateShared<Descriptor_Set_Layout>(coreCtx->device, cDescriptorLayoutBindings, true);
  return descriptorSetLayout;
}

void initializeRenderPass(Renderer_Deferred_GBuffer_Pass_Systen_Ctx* deferredGBufferCtx)
{
  Renderer_Core_System_Ctx* coreCtx = deferredGBufferCtx->frameCtx->coreCtx;
  
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
  deferredGBufferCtx->renderPass = core::allocateShared<Render_Pass>(coreCtx->device, getSubpassDescriptions(), subpassesDependencies, getAttachmentsDescriptions(deferredGBufferCtx));
}

void initalizeGraphicsPipeline(Renderer_Deferred_GBuffer_Pass_Systen_Ctx* deferredGBufferCtx)
{
  Renderer_Core_System_Ctx* coreCtx = deferredGBufferCtx->frameCtx->coreCtx;

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
    coreCtx->device, createDescriptorSetLayout(deferredGBufferCtx),
    Push_Constant_Range<Per_Mesh>(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT));

  deferredGBufferCtx->pipeline = core::allocateShared<Pipeline>(
    coreCtx->device,
    deferredGBufferCtx->renderPass,
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

void initializeFramebuffers(Renderer_Deferred_GBuffer_Pass_Systen_Ctx* deferredGBufferCtx)
{
  Renderer_Core_System_Ctx* coreCtx = deferredGBufferCtx->frameCtx->coreCtx;

  deferredGBufferCtx->framebuffers.reserve(coreCtx->swapchainImages.size());
  for (core::uint32 i = 0; i < coreCtx->swapchainImages.size(); ++i)
  {
    deferredGBufferCtx->framebuffers.push_back(core::allocateShared<Framebuffer>(
      coreCtx->device, deferredGBufferCtx->renderPass, getFramebufferAttachments(deferredGBufferCtx), deferredGBufferCtx->gbuffer->getExtent(), 1u));
  }
}

core::array<Subpass_Description, 1> getSubpassDescriptions()
{
  return
  {
    Subpass_Description(Subpass_Description::Initialize_Color_Array_Depth{
      core::array
      {
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
      },
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL })
  };
}

core::vector<Attachment_Description> getAttachmentsDescriptions(Renderer_Deferred_GBuffer_Pass_Systen_Ctx* deferredGBufferCtx)
{
  return
  {
    Attachment_Description({
      .format        = deferredGBufferCtx->gbuffer->getAlbedoAttachment()->getFormat(),
      .samples       = deferredGBufferCtx->gbuffer->getAlbedoAttachment()->getSampleCount(),
      .colorOp       = attachment_op::gClearStore,
      .stenicilOp    = attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }),
    Attachment_Description({
      .format        = deferredGBufferCtx->gbuffer->getDepthStencilAttachment()->getFormat(),
      .samples       = deferredGBufferCtx->gbuffer->getDepthStencilAttachment()->getSampleCount(),
      .colorOp       = attachment_op::gClearStore,
      .stenicilOp    = attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL })
  };
}

core::vector<core::shared_ptr<Image_View>> getFramebufferAttachments(Renderer_Deferred_GBuffer_Pass_Systen_Ctx* deferredGBufferCtx)
{
  return { deferredGBufferCtx->gbuffer->getAlbedoAttachmentView(), deferredGBufferCtx->gbuffer->getDepthStencilAttachmentView() };
}

Color_Blend_State_Create_Info createColorBlendStateCreateInfo()
{
  core::array<Pipeline_Color_Blend_Attachment_State, 1u> colorAttachments =
  {
    Pipeline_Color_Blend_Attachment_State({
      .blendEnable         = VK_FALSE,
      .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
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

Depth_Stencil_State_Create_Info createDepthStencilStateCreateInfo()
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