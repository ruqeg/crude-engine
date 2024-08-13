#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <cstring>
#include <algorithm>
#include <directxmath/DirectXMath.h>
#include <crude_shaders/fullscreen_pbr.frag.inl>
#include <crude_shaders/fullscreen.mesh.inl>
#include <functional>

module crude.graphics.fullscreen_pbr_pass_system;

import crude.core.logger;
import crude.scene.image;
import crude.graphics.format_helper;
import crude.graphics.generate_mipmaps;
import crude.graphics.flush;
import crude.graphics.mesh_buffer;
import crude.scene.mesh;
import crude.scene.free_camera_script;
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
import crude.graphics.renderer_core_component;
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

Fullscreen_PBR_Pass_Component::Fullscreen_PBR_Pass_Component()
  : albedoTextureDescriptors{ cAlbedoTextureDescriptor, cAlbedoTextureDescriptor }
  , depthTextureDescriptors{ cDepthTextureDescriptor, cDepthTextureDescriptor }
{}

Fullscreen_PBR_Pass_Component::Fullscreen_PBR_Pass_Component(core::shared_ptr<GBuffer> gbuffer, core::shared_ptr<Swap_Chain> swapchain)
  : Fullscreen_PBR_Pass_Component()
{
  this->gbuffer = gbuffer;

  this->colorAttachment = core::allocateShared<Color_Attachment>(
    swapchain->getDevice(), swapchain->getSurfaceFormat().format, swapchain->getExtent(), 1u,
    swapchain->getDevice()->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount());

  sampler = core::allocateShared<graphics::Sampler>(gbuffer->getDevice(), graphics::csamlper_state::gMagMinMipLinearRepeat);

  initializeRenderPass(swapchain);
  initalizeGraphicsPipeline();
  initializeFramebuffers(swapchain);
}

core::shared_ptr<Descriptor_Set_Layout> Fullscreen_PBR_Pass_Component::createDescriptorSetLayout()
{
  auto descriptorPool = core::allocateShared<Descriptor_Pool>(gbuffer->getDevice(), cDescriptorPoolSizes);
  auto descriptorSetLayout = core::allocateShared<Descriptor_Set_Layout>(gbuffer->getDevice(), cDescriptorLayoutBindings, true);
  return descriptorSetLayout;
}

void Fullscreen_PBR_Pass_Component::initializeRenderPass(core::shared_ptr<Swap_Chain> swapchain)
{
  core::array<Subpass_Dependency, 1u> subpassesDependencies =
  {
    Subpass_Dependency(
      VK_SUBPASS_EXTERNAL, 0u,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      0u,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      0u)
  };
  renderPass = core::allocateShared<Render_Pass>(gbuffer->getDevice(), getSubpassDescriptions(), subpassesDependencies, getAttachmentsDescriptions(swapchain));
}

void Fullscreen_PBR_Pass_Component::initalizeGraphicsPipeline()
{
  core::shared_ptr<Shader_Module> meshShaderModule = core::allocateShared<Shader_Module>(gbuffer->getDevice(), crude::shaders::fullscreen::mesh);
  core::shared_ptr<Shader_Module> fragShaderModule = core::allocateShared<Shader_Module>(gbuffer->getDevice(), crude::shaders::fullscreen_pbr::frag);

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
    .rasterizationSamples = gbuffer->getDevice()->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount(),
    .sampleShadingEnable = VK_TRUE,
    .minSampleShading = 0.2f,
    .alphaToCoverageEnable = false,
    .alphaToOneEnable = false });

  core::array<VkDynamicState, 2> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
  };
  Dynamic_State_Create_Info dynamicStateInfo(dynamicStates);

  core::shared_ptr<Pipeline_Layout> pipelineLayout = core::allocateShared<Pipeline_Layout>(
    gbuffer->getDevice(), createDescriptorSetLayout());

  pipeline = core::allocateShared<Pipeline>(
    gbuffer->getDevice(),
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

void Fullscreen_PBR_Pass_Component::initializeFramebuffers(core::shared_ptr<Swap_Chain> swapchain)
{
  framebuffers.reserve(swapchain->getSwapchainImages().size());
  for (auto& swapchainImage : swapchain->getSwapchainImages())
  {
    auto swapchainImageView = core::allocateShared<Image_View>(swapchainImage);
    framebuffers.push_back(core::allocateShared<Framebuffer>(
      gbuffer->getDevice(), 
      pipeline->getRenderPass(), 
      getFramebufferAttachments(swapchainImageView),
      gbuffer->getExtent(), 
      1u));
  }
}

core::array<Subpass_Description, 1> Fullscreen_PBR_Pass_Component::getSubpassDescriptions()
{
  return
  {
    Subpass_Description(Subpass_Description::Initialize_Color_Resolve{
      .colorLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      .resolveLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL })
  };
}

core::vector<Attachment_Description> Fullscreen_PBR_Pass_Component::getAttachmentsDescriptions(core::shared_ptr<Swap_Chain> swapchain)
{
  return
  {
    Color_Attachment_Description({.attachment = gbuffer->getAlbedoAttachment() }),
    Swapchain_Attachment_Description({
      .swapchain = swapchain,
      .colorOp = attachment_op::gDontCare})
  };
}

core::vector<core::shared_ptr<Image_View>> Fullscreen_PBR_Pass_Component::getFramebufferAttachments(core::shared_ptr<Image_View> swapchainImageView)
{
  return { core::allocateShared<Image_View>(colorAttachment), swapchainImageView };
}

Color_Blend_State_Create_Info Fullscreen_PBR_Pass_Component::createColorBlendStateCreateInfo()
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

void fullscreenPBRPassSystemProcess(flecs::iter& it)
{
  Fullscreen_PBR_Pass_Component* fullscreenPBRComponent = it.world().get_mut<Fullscreen_PBR_Pass_Component>();
  Renderer_Frame_Component* frameComponent = it.world().get_mut<Renderer_Frame_Component>();
  Renderer_Core_Component* coreComponent = it.world().get_mut<Renderer_Core_Component>();

  core::array<VkClearValue, 2u> clearValues;;
  clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
  clearValues[1].depthStencil = { 1.0f, 0 };

  VkRect2D renderArea;
  renderArea.extent = coreComponent->swapchain->getExtent();
  renderArea.offset = VkOffset2D{ 0, 0 };

  frameComponent->getFrameGraphicsCommandBuffer()->setViewport(Viewport({
    .x = 0.0f, .y = 0.0f,
    .width = static_cast<core::float32>(coreComponent->swapchain->getExtent().width),
    .height = static_cast<core::float32>(coreComponent->swapchain->getExtent().height),
    .minDepth = 0.0f, .maxDepth = 1.0f }));

  frameComponent->getFrameGraphicsCommandBuffer()->setScissor(Scissor({
    .offset = { 0, 0 },
    .extent = coreComponent->swapchain->getExtent() }));

  frameComponent->getFrameGraphicsCommandBuffer()->beginRenderPass(fullscreenPBRComponent->pipeline->getRenderPass(), fullscreenPBRComponent->framebuffers[frameComponent->swapchainImageIndex], clearValues, renderArea);
  frameComponent->getFrameGraphicsCommandBuffer()->bindPipeline(fullscreenPBRComponent->pipeline);

  fullscreenPBRComponent->albedoTextureDescriptors[frameComponent->currentFrame].update(fullscreenPBRComponent->gbuffer->getAlbedoAttachmentView(), fullscreenPBRComponent->sampler);
  fullscreenPBRComponent->depthTextureDescriptors[frameComponent->currentFrame].update(fullscreenPBRComponent->gbuffer->getDepthStencilAttachmentView(), fullscreenPBRComponent->sampler);

  core::array<VkWriteDescriptorSet, 2u> descriptorWrites;
  fullscreenPBRComponent->albedoTextureDescriptors[frameComponent->currentFrame].write(descriptorWrites[0]);
  fullscreenPBRComponent->depthTextureDescriptors[frameComponent->currentFrame].write(descriptorWrites[1]);

  frameComponent->getFrameGraphicsCommandBuffer()->pushDescriptorSet(fullscreenPBRComponent->pipeline, descriptorWrites);
  frameComponent->getFrameGraphicsCommandBuffer()->drawMeshTasks(1u);

  frameComponent->getFrameGraphicsCommandBuffer()->endRenderPass();
}

}