#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <cstring>
#include <algorithm>
#include <DirectXMath.h>
#include <crude_shaders/fullscreen_pbr.frag.inl>
#include <crude_shaders/fullscreen.mesh.inl>
#include <functional>

module crude.graphics.renderer_fullscreen_pbr_pass_system;

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
import crude.graphics.sampler_state;
import crude.graphics.sampler;

namespace crude::graphics
{

const Combined_Image_Sampler_Descriptor cAlbedoTextureDescriptor(0u, VK_SHADER_STAGE_FRAGMENT_BIT);
const Combined_Image_Sampler_Descriptor cDepthTextureDescriptor(1u, VK_SHADER_STAGE_FRAGMENT_BIT);

core::array<Descriptor_Set_Layout_Binding, 2u> cDescriptorLayoutBindings =
{
  cAlbedoTextureDescriptor,
  cDepthTextureDescriptor,
};

core::array<Descriptor_Pool_Size, 2u> cDescriptorPoolSizes
{
  Combined_Image_Sampler_Pool_Size(cFramesCount),
  Combined_Image_Sampler_Pool_Size(cFramesCount),
};

Renderer_Fullscreen_PBR_Pass_Ctx::Renderer_Fullscreen_PBR_Pass_Ctx()
  : albedoTextureDescriptors{ cAlbedoTextureDescriptor, cAlbedoTextureDescriptor }
  , depthTextureDescriptors{ cDepthTextureDescriptor, cDepthTextureDescriptor }
{}

void rendererFullscreenPBRPassSystemProcess(flecs::iter& it)
{
  Renderer_Fullscreen_PBR_Pass_Ctx* fullscreenPbrCtx = it.ctx<Renderer_Fullscreen_PBR_Pass_Ctx>();
  Renderer_Frame_System_Ctx* frameCtx = fullscreenPbrCtx->frameCtx;
  Renderer_Core_System_Ctx* coreCtx = frameCtx->coreCtx;

  core::array<VkClearValue, 2u> clearValues;;
  clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
  clearValues[1].depthStencil = { 1.0f, 0 };

  VkRect2D renderArea;
  renderArea.extent = fullscreenPbrCtx->colorAttachment->getExtent2D();
  renderArea.offset = VkOffset2D{ 0, 0 };

  frameCtx->getFrameGraphicsCommandBuffer()->setViewport(Viewport({
    .x = 0.0f, .y = 0.0f,
    .width = static_cast<core::float32>(fullscreenPbrCtx->colorAttachment->getExtent().width),
    .height = static_cast<core::float32>(fullscreenPbrCtx->colorAttachment->getExtent().height),
    .minDepth = 0.0f, .maxDepth = 1.0f }));

  frameCtx->getFrameGraphicsCommandBuffer()->setScissor(Scissor({
    .offset = { 0, 0 },
    .extent = fullscreenPbrCtx->colorAttachment->getExtent2D() }));

  frameCtx->getFrameGraphicsCommandBuffer()->beginRenderPass(fullscreenPbrCtx->pipeline->getRenderPass(), fullscreenPbrCtx->framebuffers[frameCtx->swapchainImageIndex], clearValues, renderArea);
  frameCtx->getFrameGraphicsCommandBuffer()->bindPipeline(fullscreenPbrCtx->pipeline);

  fullscreenPbrCtx->albedoTextureDescriptors[frameCtx->currentFrame].update(fullscreenPbrCtx->gbuffer->getAlbedoAttachmentView(), fullscreenPbrCtx->sampler);
  fullscreenPbrCtx->depthTextureDescriptors[frameCtx->currentFrame].update(fullscreenPbrCtx->gbuffer->getDepthStencilAttachmentView(), fullscreenPbrCtx->sampler);

  core::array<VkWriteDescriptorSet, 2u> descriptorWrites;
  fullscreenPbrCtx->albedoTextureDescriptors[frameCtx->currentFrame].write(descriptorWrites[0]);
  fullscreenPbrCtx->depthTextureDescriptors[frameCtx->currentFrame].write(descriptorWrites[1]);

  frameCtx->getFrameGraphicsCommandBuffer()->pushDescriptorSet(fullscreenPbrCtx->pipeline, descriptorWrites);
  frameCtx->getFrameGraphicsCommandBuffer()->drawMeshTasks(1u);

  frameCtx->getFrameGraphicsCommandBuffer()->endRenderPass();
}

void rendererFullscreenPBRPassSystemInitialize(flecs::iter& it)
{
  Renderer_Fullscreen_PBR_Pass_Ctx* fullscreenPbrCtx = it.ctx<Renderer_Fullscreen_PBR_Pass_Ctx>();
  Renderer_Frame_System_Ctx* frameCtx = fullscreenPbrCtx->frameCtx;
  Renderer_Core_System_Ctx* coreCtx = frameCtx->coreCtx;

  fullscreenPbrCtx->sampler = core::allocateShared<graphics::Sampler>(coreCtx->device, graphics::csamlper_state::gMagMinMipLinearRepeat);

  fullscreenPbrCtx->colorAttachment = core::allocateShared<Color_Attachment>(Color_Attachment::Initialize{
    .device          = coreCtx->device,
    .format          = VK_FORMAT_B8G8R8A8_SRGB,
    .extent          = fullscreenPbrCtx->gbuffer->getExtent(),
    .sampled         = true,
    .explicitResolve = false,
    .mipLevelsCount  = 1u,
    .samples         = VK_SAMPLE_COUNT_1_BIT });

  initializeRenderPass(fullscreenPbrCtx);
  initalizeGraphicsPipeline(fullscreenPbrCtx);
  initializeFramebuffers(fullscreenPbrCtx);
}

core::shared_ptr<Descriptor_Set_Layout> createDescriptorSetLayout(Renderer_Fullscreen_PBR_Pass_Ctx* fullscreenPbrCtx)
{
  Renderer_Core_System_Ctx* coreCtx = fullscreenPbrCtx->frameCtx->coreCtx;
  auto descriptorPool = core::allocateShared<Descriptor_Pool>(coreCtx->device, cDescriptorPoolSizes, 2);
  auto descriptorSetLayout = core::allocateShared<Descriptor_Set_Layout>(coreCtx->device, cDescriptorLayoutBindings, true);
  return descriptorSetLayout;
}

void initializeRenderPass(Renderer_Fullscreen_PBR_Pass_Ctx* fullscreenPbrCtx)
{
  Renderer_Core_System_Ctx* coreCtx = fullscreenPbrCtx->frameCtx->coreCtx;
  core::array<Subpass_Dependency, 1u> subpassesDependencies =
  {
    Subpass_Dependency({
      .srcSubpass      = VK_SUBPASS_EXTERNAL,
      .dstSubpass      = 0,
      .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .srcAccessMask   = 0,
      .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0})
  };
  fullscreenPbrCtx->renderPass = core::allocateShared<Render_Pass>(coreCtx->device, getSubpassDescriptions(), subpassesDependencies, getAttachmentsDescriptions(fullscreenPbrCtx));
}

void initalizeGraphicsPipeline(Renderer_Fullscreen_PBR_Pass_Ctx* fullscreenPbrCtx)
{
  Renderer_Core_System_Ctx* coreCtx = fullscreenPbrCtx->frameCtx->coreCtx;

  core::shared_ptr<Shader_Module> meshShaderModule = core::allocateShared<Shader_Module>(coreCtx->device, crude::shaders::fullscreen::mesh);
  core::shared_ptr<Shader_Module> fragShaderModule = core::allocateShared<Shader_Module>(coreCtx->device, crude::shaders::fullscreen_pbr::frag);

  core::array<Shader_Stage_Create_Info, 2u> shaderStagesInfo =
  {
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
    coreCtx->device, createDescriptorSetLayout(fullscreenPbrCtx));

  fullscreenPbrCtx->pipeline = core::allocateShared<Pipeline>(
    coreCtx->device,
    fullscreenPbrCtx->renderPass,
    pipelineLayout,
    nullptr,
    shaderStagesInfo,
    vertexInputStateInfo,
    tesselationInfo,
    inputAssembly,
    viewportState,
    rasterizer,
    multisampling,
    core::nullopt,
    createColorBlendStateCreateInfo(),
    dynamicStateInfo,
    0u);
}

void initializeFramebuffers(Renderer_Fullscreen_PBR_Pass_Ctx* fullscreenPbrCtx)
{
  Renderer_Core_System_Ctx* coreCtx = fullscreenPbrCtx->frameCtx->coreCtx;
  fullscreenPbrCtx->framebuffers.reserve(coreCtx->swapchain->getSwapchainImages().size());
  for (core::size_t i = 0; i < coreCtx->swapchain->getSwapchainImages().size(); ++i)
  {
    auto colorAttachmentView = core::allocateShared<Image_View>(fullscreenPbrCtx->colorAttachment);
    fullscreenPbrCtx->framebuffers.push_back(core::allocateShared<Framebuffer>(
      coreCtx->device,
      fullscreenPbrCtx->pipeline->getRenderPass(),
      core::vector<core::shared_ptr<Image_View>>{ colorAttachmentView },
      fullscreenPbrCtx->colorAttachment->getExtent2D(),
      1u));
  }
}

core::array<Subpass_Description, 1> getSubpassDescriptions()
{
  return
  {
    Subpass_Description(Subpass_Description::Initialize_Color{
      .colorLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL})
  };
}

core::vector<Attachment_Description> getAttachmentsDescriptions(Renderer_Fullscreen_PBR_Pass_Ctx* fullscreenPbrCtx)
{
  return
  {
    Attachment_Description({
      .format        = fullscreenPbrCtx->colorAttachment->getFormat(),
      .samples       = VK_SAMPLE_COUNT_1_BIT,
      .colorOp       = attachment_op::gClearStore,
      .stenicilOp    = attachment_op::gDontCare,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL})
  };
}

Color_Blend_State_Create_Info createColorBlendStateCreateInfo()
{
  core::array<Pipeline_Color_Blend_Attachment_State, 1u> colorAttachments =
  {
    Pipeline_Color_Blend_Attachment_State({
      .blendEnable = VK_FALSE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD })
  };
  return Color_Blend_State_Create_Info({
    .attachments = colorAttachments,
    .blendConstants = { 0.f, 0.f, 0.f, 0.f },
    .logicOpEnable = false,
    .logicOp = VK_LOGIC_OP_COPY });
}

}