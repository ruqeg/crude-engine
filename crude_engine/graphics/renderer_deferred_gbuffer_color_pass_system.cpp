#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <cstring>
#include <algorithm>
#include <DirectXMath.h>
#include <crude_shaders/deferred_gbuffer_color.frag.inl>
#include <crude_shaders/deferred_gbuffer_color.mesh.inl>
#include <crude_shaders/deferred_gbuffer.task.inl>
#include <functional>

module crude.gfx.renderer_deferred_gbuffer_color_pass_system;

import crude.core.logger;
import crude.scene.image;
import crude.gfx.vk.format_helper;
import crude.gfx.vk.generate_mipmaps;
import crude.gfx.vk.flush;
import crude.gfx.mesh_buffer;
import crude.scene.mesh;
import crude.gfx.vk.render_pass;
import crude.gfx.vk.command_buffer;
import crude.gfx.vk.framebuffer;
import crude.gfx.vk.pipeline;
import crude.gfx.vk.swap_chain;
import crude.gfx.vk.descriptor_pool;
import crude.gfx.vk.descriptor_set_layout;
import crude.gfx.vk.semaphore;
import crude.gfx.vk.fence;
import crude.platform.sdl_window_container;
import crude.gfx.vk.physical_device;
import crude.gfx.vk.queue;
import crude.gfx.vk.instance;
import crude.gfx.vk.device;
import crude.gfx.vk.surface;
import crude.gfx.vk.swap_chain;
import crude.gfx.vk.swap_chain_image;
import crude.gfx.vk.image_view;
import crude.gfx.vk.uniform_buffer;
import crude.gfx.texture;
import crude.gfx.material;
import crude.gfx.vk.debug_utils_messenger;
import crude.gfx.vk.command_pool;
import crude.gfx.vk.image_attachment;
import crude.gfx.vk.shader_module;
import crude.gfx.vk.storage_buffer;
import crude.gfx.vk.attachment_description;
import crude.gfx.gbuffer;

namespace crude::gfx
{

void rendererDeferredGBufferColorPassSystemProcess(flecs::iter& it)
{
  Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx* deferredGBufferCtx = it.ctx<Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx>();
  core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx = deferredGBufferCtx->frameCtx;
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  VkRect2D renderArea;
  renderArea.extent = coreCtx->swapchain->getExtent();
  renderArea.offset = VkOffset2D{ 0, 0 };

  frameCtx->getFrameGraphicsCommandBuffer()->setViewport(vk::Viewport({
    .x = 0.0f, .y = 0.0f,
    .width = static_cast<core::float32>(coreCtx->swapchain->getExtent().width),
    .height = static_cast<core::float32>(coreCtx->swapchain->getExtent().height),
    .minDepth = 0.0f, .maxDepth = 1.0f }));

  frameCtx->getFrameGraphicsCommandBuffer()->setScissor(vk::Scissor({
    .offset = { 0, 0 },
    .extent = coreCtx->swapchain->getExtent() }));

  frameCtx->getFrameGraphicsCommandBuffer()->beginRenderPass(deferredGBufferCtx->pipeline->getRenderPass(), deferredGBufferCtx->framebuffers[frameCtx->swapchainImageIndex], renderArea);
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

        deferredGBufferCtx->submeshAlbedoDescriptors[frameCtx->currentFrame].update(submesh.material->albedo->getImageView(), submesh.material->albedo->getSampler());

        core::array<VkWriteDescriptorSet, 7u> descriptorWrites;
        deferredGBufferCtx->perFrameBufferDescriptors[frameCtx->currentFrame].write(descriptorWrites[0]);
        deferredGBufferCtx->submeshAlbedoDescriptors[frameCtx->currentFrame].write(descriptorWrites[1]);
        deferredGBufferCtx->submeshesDrawsBufferDescriptor.write(descriptorWrites[2]);
        deferredGBufferCtx->vertexBufferDescriptor.write(descriptorWrites[3]);
        deferredGBufferCtx->meshletBufferDescriptor.write(descriptorWrites[4]);
        deferredGBufferCtx->primitiveIndicesBufferDescriptor.write(descriptorWrites[5]);
        deferredGBufferCtx->vertexIndicesBufferDescriptor.write(descriptorWrites[6]);

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

Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx)
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

Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx, core::shared_ptr<GBuffer> gbuffer)
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
  this->gbuffer = gbuffer;
  initializeRenderPass();
  initalizeGraphicsPipeline();
  initializeFramebuffers();
}


core::shared_ptr<vk::Descriptor_Set_Layout> Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::createDescriptorSetLayout()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  auto descriptorPool = core::allocateShared<vk::Descriptor_Pool>(coreCtx->device, getDescriptorPoolSizes(), 2);
  auto descriptorSetLayout = core::allocateShared<vk::Descriptor_Set_Layout>(coreCtx->device, getDescriptorLayoutBindings(), true);
  return descriptorSetLayout;
}

void Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::initializeRenderPass()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  core::array<vk::Subpass_Dependency, 1u> subpassesDependencies =
  {
    vk::Subpass_Dependency({
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .srcAccessMask = 0u,
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0})
  };
  renderPass = core::allocateShared<vk::Render_Pass>(coreCtx->device, getSubpassDescriptions(), subpassesDependencies, getAttachmentsDescriptions());
}

void Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::initalizeGraphicsPipeline()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  core::shared_ptr<vk::Shader_Module> taskShaderModule = core::allocateShared<vk::Shader_Module>(coreCtx->device, crude::shaders::deferred_gbuffer::task);
  core::shared_ptr<vk::Shader_Module> meshShaderModule = core::allocateShared<vk::Shader_Module>(coreCtx->device, crude::shaders::deferred_gbuffer_color::mesh);
  core::shared_ptr<vk::Shader_Module> fragShaderModule = core::allocateShared<vk::Shader_Module>(coreCtx->device, crude::shaders::deferred_gbuffer_color::frag);

  core::array<vk::Shader_Stage_Create_Info, 3u> shaderStagesInfo =
  {
    vk::Task_Shader_Stage_Create_Info(taskShaderModule, "main"),
    vk::Mesh_Shader_Stage_Create_Info(meshShaderModule, "main"),
    vk::Fragment_Shader_Stage_Create_Info(fragShaderModule, "main"),
  };

  vk::Vertex_Input_State_Create_Info vertexInputStateInfo({
    .bindings = {},
    .attributes = {} });

  vk::Tessellation_State_Create_Info tesselationInfo(3u);

  vk::Input_Assembly_State_Create_Info inputAssembly({
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    .primitiveRestartEnable = false });

  vk::Viewport_State_Create_Info viewportState(1u, 1u);

  vk::Rasterization_State_Create_Info rasterizer({
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

  vk::Multisample_State_Create_Info multisampling({
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable = VK_FALSE,
    .minSampleShading = 0.2f,
    .alphaToCoverageEnable = false,
    .alphaToOneEnable = false });

  core::array<VkDynamicState, 2> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
  };
  vk::Dynamic_State_Create_Info dynamicStateInfo(dynamicStates);

  core::shared_ptr<vk::Pipeline_Layout> pipelineLayout = core::allocateShared<vk::Pipeline_Layout>(
    coreCtx->device, createDescriptorSetLayout(),
    vk::Push_Constant_Range<Per_Mesh>(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT));

  pipeline = core::allocateShared<vk::Pipeline>(
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

void Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::initializeFramebuffers()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  framebuffers.reserve(coreCtx->swapchainImages.size());
  for (core::uint32 i = 0; i < coreCtx->swapchainImages.size(); ++i)
  {
    framebuffers.push_back(core::allocateShared<vk::Framebuffer>(
      coreCtx->device, renderPass, getFramebufferAttachments(), gbuffer->getExtent(), 1u));
  }
}

core::array<vk::Subpass_Description, 1> Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::getSubpassDescriptions()
{
  return
  {
    vk::Subpass_Description(vk::Subpass_Description::Initialize_Color_Depth{
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL })
  };
}

core::vector<vk::Attachment_Description> Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::getAttachmentsDescriptions()
{
  return
  {
    vk::Attachment_Description({
      .format        = gbuffer->getAlbedoAttachment()->getFormat(),
      .samples       = gbuffer->getAlbedoAttachment()->getSampleCount(),
      .colorOp       = vk::attachment_op::gDontCareStore,
      .stenicilOp    = vk::attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }),
    vk::Attachment_Description({
      .format        = gbuffer->getDepthStencilAttachment()->getFormat(),
      .samples       = gbuffer->getDepthStencilAttachment()->getSampleCount(),
      .colorOp       = vk::attachment_op::gDontCareStore,
      .stenicilOp    = vk::attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL })
  };
}

core::vector<core::shared_ptr<vk::Image_View>> Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::getFramebufferAttachments()
{
  return { gbuffer->getAlbedoAttachmentView(), gbuffer->getDepthStencilAttachmentView() };
}

vk::Color_Blend_State_Create_Info Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::createColorBlendStateCreateInfo()
{
  core::array<vk::Pipeline_Color_Blend_Attachment_State, 1u> colorAttachments =
  {
    vk::Pipeline_Color_Blend_Attachment_State({
      .blendEnable = VK_TRUE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD }),
  };
  return vk::Color_Blend_State_Create_Info({
    .attachments = colorAttachments,
    .blendConstants = { 0.f, 0.f, 0.f, 0.f },
    .logicOpEnable = false,
    .logicOp = VK_LOGIC_OP_COPY });
}

vk::Depth_Stencil_State_Create_Info Renderer_Deferred_GBuffer_Color_Pass_Systen_Ctx::createDepthStencilStateCreateInfo()
{
  return vk::Depth_Stencil_State_Create_Info({
    .depthTestEnable = true,
    .depthWriteEnable = true,
    .depthCompareOp = VK_COMPARE_OP_LESS,
    .depthBoundsTestEnable = false,
    .stencilTestEnable = false,
    .front = {},
    .back = {},
    .minDepthBounds = 0.0f,
    .maxDepthBounds = 1.0f });
}

}