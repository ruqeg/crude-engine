#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <cstring>
#include <algorithm>
#include <DirectXMath.h>
#include <crude_shaders/fullscreen_pbr.frag.inl>
#include <crude_shaders/fullscreen.mesh.inl>
#include <functional>

module crude.gfx.renderer_fullscreen_pbr_pass_system;

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
import crude.gfx.vk.debug_utils_messenger;
import crude.gfx.vk.image_view;
import crude.gfx.vk.command_pool;
import crude.gfx.vk.image_attachment;
import crude.gfx.vk.shader_module;
import crude.gfx.vk.storage_buffer;
import crude.gfx.vk.attachment_description;
import crude.gfx.gbuffer;
import crude.gfx.vk.sampler_state;
import crude.gfx.vk.sampler;

namespace crude::gfx
{

const vk::Uniform_Buffer_Descriptor         cPerFrameUniformBufferDescriptor2{ 0u, VK_SHADER_STAGE_FRAGMENT_BIT };
const vk::Combined_Image_Sampler_Descriptor cAlbedoTextureDescriptor{ 1u, VK_SHADER_STAGE_FRAGMENT_BIT };
const vk::Combined_Image_Sampler_Descriptor cMetallicRoughnessTextureDescriptor{ 2u, VK_SHADER_STAGE_FRAGMENT_BIT };
const vk::Combined_Image_Sampler_Descriptor cNormalTextureDescriptor{ 3u, VK_SHADER_STAGE_FRAGMENT_BIT };
const vk::Combined_Image_Sampler_Descriptor cDepthTextureDescriptor{ 4u, VK_SHADER_STAGE_FRAGMENT_BIT };
const vk::Storage_Buffer_Descriptor         cPointLightsBufferDescriptor{ 5u, VK_SHADER_STAGE_FRAGMENT_BIT };
const vk::Uniform_Buffer_Descriptor         cPbrDebugBufferDescriptor{ 6u, VK_SHADER_STAGE_FRAGMENT_BIT };

core::array<vk::Descriptor_Set_Layout_Binding, 7u> cDescriptorLayoutBindings =
{
  cPerFrameUniformBufferDescriptor2,
  cAlbedoTextureDescriptor,
  cMetallicRoughnessTextureDescriptor,
  cNormalTextureDescriptor,
  cDepthTextureDescriptor,
  cPointLightsBufferDescriptor,
  cPbrDebugBufferDescriptor
};

core::array<vk::Descriptor_Pool_Size, 7u> cDescriptorPoolSizes
{
  vk::Uniform_Buffer_Pool_Size(cFramesCount),
  vk::Combined_Image_Sampler_Pool_Size(cFramesCount),
  vk::Combined_Image_Sampler_Pool_Size(cFramesCount),
  vk::Combined_Image_Sampler_Pool_Size(cFramesCount),
  vk::Combined_Image_Sampler_Pool_Size(cFramesCount),
  vk::Storage_Buffer_Pool_Size(1u),
  vk::Uniform_Buffer_Pool_Size(cFramesCount),
};

void rendererFullscreenPBRPassSystemProcess(flecs::iter& it)
{
  Renderer_Fullscreen_PBR_Pass_Ctx* fullscreenPbrCtx = it.ctx<Renderer_Fullscreen_PBR_Pass_Ctx>();
  core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx = fullscreenPbrCtx->frameCtx;
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  core::shared_ptr<Renderer_Light_Ctx> lightCtx = fullscreenPbrCtx->lightCtx;

  core::array<VkClearValue, 2u> clearValues;;
  clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
  clearValues[1].depthStencil = { 1.0f, 0 };

  VkRect2D renderArea;
  renderArea.extent = fullscreenPbrCtx->colorAttachment->getExtent2D();
  renderArea.offset = VkOffset2D{ 0, 0 };

  frameCtx->getFrameGraphicsCommandBuffer()->setViewport(vk::Viewport({
    .x = 0.0f, .y = 0.0f,
    .width = static_cast<core::float32>(fullscreenPbrCtx->colorAttachment->getExtent().width),
    .height = static_cast<core::float32>(fullscreenPbrCtx->colorAttachment->getExtent().height),
    .minDepth = 0.0f, .maxDepth = 1.0f }));

  frameCtx->getFrameGraphicsCommandBuffer()->setScissor(vk::Scissor({
    .offset = { 0, 0 },
    .extent = fullscreenPbrCtx->colorAttachment->getExtent2D() }));

  frameCtx->getFrameGraphicsCommandBuffer()->beginRenderPass(fullscreenPbrCtx->pipeline->getRenderPass(), fullscreenPbrCtx->framebuffers[frameCtx->swapchainImageIndex], clearValues, renderArea);
  frameCtx->getFrameGraphicsCommandBuffer()->bindPipeline(fullscreenPbrCtx->pipeline);

  PBRDebug* data = fullscreenPbrCtx->pbrDebugBuffers[frameCtx->currentFrame]->mapUnsafe();
  *data = fullscreenPbrCtx->pbrDebug;
  fullscreenPbrCtx->pbrDebugBuffers[frameCtx->currentFrame]->unmap();

  fullscreenPbrCtx->perFrameBufferDescriptors[frameCtx->currentFrame].update(frameCtx->getFramePerFrameUniformBuffer());
  fullscreenPbrCtx->albedoTextureDescriptors[frameCtx->currentFrame].update(fullscreenPbrCtx->gbuffer->getAlbedoAttachmentView(), fullscreenPbrCtx->sampler);
  fullscreenPbrCtx->metallicRoughnessTextureDescriptors[frameCtx->currentFrame].update(fullscreenPbrCtx->gbuffer->getMetallicRoughnessAttachmentView(), fullscreenPbrCtx->sampler);
  fullscreenPbrCtx->normalTextureDescriptors[frameCtx->currentFrame].update(fullscreenPbrCtx->gbuffer->getNormalAttachmentView(), fullscreenPbrCtx->sampler);
  fullscreenPbrCtx->depthTextureDescriptors[frameCtx->currentFrame].update(fullscreenPbrCtx->gbuffer->getDepthStencilAttachmentView(), fullscreenPbrCtx->sampler);
  if (fullscreenPbrCtx->lightCtx->pointLightsBuffer.has_value())
    fullscreenPbrCtx->pointLightsBufferDescriptor.update(fullscreenPbrCtx->lightCtx->pointLightsBuffer.value(), fullscreenPbrCtx->lightCtx->pointLightsBuffer.value()->getSize());
  else
    fullscreenPbrCtx->pointLightsBufferDescriptor.clear();
  fullscreenPbrCtx->pbrDebugBufferDescriptors[frameCtx->currentFrame].update(fullscreenPbrCtx->pbrDebugBuffers[frameCtx->currentFrame]);

  core::array<VkWriteDescriptorSet, 7u> descriptorWrites;
  fullscreenPbrCtx->perFrameBufferDescriptors[frameCtx->currentFrame].write(descriptorWrites[0]);
  fullscreenPbrCtx->albedoTextureDescriptors[frameCtx->currentFrame].write(descriptorWrites[1]);
  fullscreenPbrCtx->metallicRoughnessTextureDescriptors[frameCtx->currentFrame].write(descriptorWrites[2]);
  fullscreenPbrCtx->normalTextureDescriptors[frameCtx->currentFrame].write(descriptorWrites[3]);
  fullscreenPbrCtx->depthTextureDescriptors[frameCtx->currentFrame].write(descriptorWrites[4]);
  fullscreenPbrCtx->pointLightsBufferDescriptor.write(descriptorWrites[5]);
  fullscreenPbrCtx->pbrDebugBufferDescriptors[frameCtx->currentFrame].write(descriptorWrites[6]);

  frameCtx->getFrameGraphicsCommandBuffer()->pushDescriptorSet(fullscreenPbrCtx->pipeline, descriptorWrites);
  frameCtx->getFrameGraphicsCommandBuffer()->drawMeshTasks(1u);

  frameCtx->getFrameGraphicsCommandBuffer()->endRenderPass();
}

Renderer_Fullscreen_PBR_Pass_Ctx::Renderer_Fullscreen_PBR_Pass_Ctx(core::shared_ptr<Renderer_Frame_System_Ctx> frameCtx, core::shared_ptr<Renderer_Light_Ctx> lightCtx, core::shared_ptr<GBuffer> gbuffer)
  : perFrameBufferDescriptors{ cPerFrameUniformBufferDescriptor2, cPerFrameUniformBufferDescriptor2 }
  , albedoTextureDescriptors{ cAlbedoTextureDescriptor, cAlbedoTextureDescriptor }
  , metallicRoughnessTextureDescriptors{ cMetallicRoughnessTextureDescriptor, cMetallicRoughnessTextureDescriptor }
  , normalTextureDescriptors{ cNormalTextureDescriptor, cNormalTextureDescriptor }
  , depthTextureDescriptors{ cDepthTextureDescriptor, cDepthTextureDescriptor }
  , pointLightsBufferDescriptor{ cPointLightsBufferDescriptor }
  , pbrDebugBufferDescriptors { cPbrDebugBufferDescriptor, cPbrDebugBufferDescriptor }
  , frameCtx{ frameCtx }
  , gbuffer{ gbuffer }
  , lightCtx{ lightCtx }
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  sampler = core::allocateShared<gfx::vk::Sampler>(coreCtx->device, gfx::vk::csamlper_state::gMagMinMipLinearRepeat);

  colorAttachment = core::allocateShared<vk::Color_Attachment>(vk::Color_Attachment::Initialize{
    .device = coreCtx->device,
    .format = VK_FORMAT_B8G8R8A8_SRGB,
    .extent = gbuffer->getExtent(),
    .sampled = true,
    .explicitResolve = false,
    .mipLevelsCount = 1u,
    .samples = VK_SAMPLE_COUNT_1_BIT });

  initializeRenderPass();
  initalizeGraphicsPipeline();
  initializeFramebuffers();

  for (core::uint32 i = 0; i < cFramesCount; ++i)
  {
    pbrDebugBuffers[i] = core::allocateShared<vk::Uniform_Buffer<PBRDebug>>(coreCtx->device);
  }
}

core::shared_ptr<vk::Descriptor_Set_Layout> Renderer_Fullscreen_PBR_Pass_Ctx::createDescriptorSetLayout()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  auto descriptorPool = core::allocateShared<vk::Descriptor_Pool>(coreCtx->device, cDescriptorPoolSizes, 2);
  auto descriptorSetLayout = core::allocateShared<vk::Descriptor_Set_Layout>(coreCtx->device, cDescriptorLayoutBindings, true);
  return descriptorSetLayout;
}

void Renderer_Fullscreen_PBR_Pass_Ctx::initializeRenderPass()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  core::array<vk::Subpass_Dependency, 1u> subpassesDependencies =
  {
    vk::Subpass_Dependency({
      .srcSubpass      = VK_SUBPASS_EXTERNAL,
      .dstSubpass      = 0,
      .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .srcAccessMask   = 0,
      .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0})
  };
  renderPass = core::allocateShared<vk::Render_Pass>(coreCtx->device, getSubpassDescriptions(), subpassesDependencies, getAttachmentsDescriptions());
}

void Renderer_Fullscreen_PBR_Pass_Ctx::initalizeGraphicsPipeline()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  core::shared_ptr<vk::Shader_Module> meshShaderModule = core::allocateShared<vk::Shader_Module>(coreCtx->device, crude::shaders::fullscreen::mesh);
  core::shared_ptr<vk::Shader_Module> fragShaderModule = core::allocateShared<vk::Shader_Module>(coreCtx->device, crude::shaders::fullscreen_pbr::frag);

  core::array<vk::Shader_Stage_Create_Info, 2u> shaderStagesInfo =
  {
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
    coreCtx->device, createDescriptorSetLayout());

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
    core::nullopt,
    createColorBlendStateCreateInfo(),
    dynamicStateInfo,
    0u);
}

void Renderer_Fullscreen_PBR_Pass_Ctx::initializeFramebuffers()
{
  core::shared_ptr<Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;
  framebuffers.reserve(coreCtx->swapchain->getSwapchainImages().size());
  for (core::size_t i = 0; i < coreCtx->swapchain->getSwapchainImages().size(); ++i)
  {
    auto colorAttachmentView = core::allocateShared<vk::Image_View>(colorAttachment);
    framebuffers.push_back(core::allocateShared<vk::Framebuffer>(
      coreCtx->device,
      pipeline->getRenderPass(),
      core::vector<core::shared_ptr<vk::Image_View>>{ colorAttachmentView },
      colorAttachment->getExtent2D(),
      1u));
  }
}

core::array<vk::Subpass_Description, 1> Renderer_Fullscreen_PBR_Pass_Ctx::getSubpassDescriptions()
{
  return
  {
    vk::Subpass_Description(vk::Subpass_Description::Initialize_Color{
      .colorLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL})
  };
}

core::vector<vk::Attachment_Description> Renderer_Fullscreen_PBR_Pass_Ctx::getAttachmentsDescriptions()
{
  return
  {
    vk::Attachment_Description({
      .format        = colorAttachment->getFormat(),
      .samples       = VK_SAMPLE_COUNT_1_BIT,
      .colorOp       = vk::attachment_op::gClearStore,
      .stenicilOp    = vk::attachment_op::gDontCare,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL})
  };
}

vk::Color_Blend_State_Create_Info Renderer_Fullscreen_PBR_Pass_Ctx::createColorBlendStateCreateInfo()
{
  core::array<vk::Pipeline_Color_Blend_Attachment_State, 1u> colorAttachments =
  {
    vk::Pipeline_Color_Blend_Attachment_State({
      .blendEnable = VK_FALSE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD })
  };
  return vk::Color_Blend_State_Create_Info({
    .attachments = colorAttachments,
    .blendConstants = { 0.f, 0.f, 0.f, 0.f },
    .logicOpEnable = false,
    .logicOp = VK_LOGIC_OP_COPY });
}

}