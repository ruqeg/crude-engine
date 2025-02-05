#include <vulkan/vulkan.hpp>
#include <flecs.h>

module crude.gfx.render_graph;

import crude.gfx.vk.render_pass;
import crude.gfx.vk.device;
import crude.gfx.vk.image_view;
import crude.gfx.vk.attachment_description;
import crude.gfx.vk.image_attachment;
import crude.gfx.vk.framebuffer;
import crude.gfx.vk.graphics_pipeline;
import crude.gfx.vk.buffer_descriptor;
import crude.gfx.vk.image_descriptor;
import crude.gfx.vk.descriptor_set_layout;
import crude.gfx.vk.command_buffer;
import crude.gfx.vk.pipeline_layout;

namespace crude::gfx
{

Render_Pass::Render_Pass(core::shared_ptr<Render_Graph> graph, const VkExtent2D& framebufferExtent, VkPipelineStageFlags queue)
  : m_graph{ graph }
  , m_queue{ queue }
  , m_framebufferExtent{ framebufferExtent }
{}

void Render_Pass::addColorOutput(const Attachment_Info& colorOutput)
{
  m_colorOutputs.push_back(colorOutput);
}

void Render_Pass::setDepthStencilOutput(const Attachment_Info& depthStencilOutput)
{
  m_depthStencilOutput = depthStencilOutput;
}

core::shared_ptr<vk::Uniform_Buffer_Descriptor[]> Render_Pass::addUniformInput(VkShaderStageFlags stageFlags)
{
  const core::uint32 binding = m_descriptorLayoutBindings.size();
  vk::Uniform_Buffer_Descriptor descriptor{ binding, stageFlags };
  m_descriptorLayoutBindings.push_back(descriptor);
  return core::allocateShared<vk::Uniform_Buffer_Descriptor[]>(m_graph->m_rendererFrame->getFramesCount(), descriptor);
}

core::shared_ptr<vk::Storage_Buffer_Descriptor[]> Render_Pass::addStorageInput(VkShaderStageFlags stageFlags)
{
  const core::uint32 binding = m_descriptorLayoutBindings.size();
  vk::Storage_Buffer_Descriptor descriptor{ binding, stageFlags };
  m_descriptorLayoutBindings.push_back(descriptor);
  return core::allocateShared<vk::Storage_Buffer_Descriptor[]>(m_graph->m_rendererFrame->getFramesCount(), descriptor);
}

core::shared_ptr<vk::Combined_Image_Sampler_Descriptor[]> Render_Pass::addTextureInput(VkShaderStageFlags stageFlags)
{
  const core::uint32 binding = m_descriptorLayoutBindings.size();
  vk::Combined_Image_Sampler_Descriptor descriptor{ binding, stageFlags };
  m_descriptorLayoutBindings.push_back(descriptor);
  return core::allocateShared<vk::Combined_Image_Sampler_Descriptor[]>(m_graph->m_rendererFrame->getFramesCount(), descriptor);
}

void Render_Pass::setPushConstantRange(const vk::Push_Constant_Range_Base& pushConstantRange)
{
  m_pushConstantRange = pushConstantRange;
}

void Render_Pass::setShaderStagesInfo(const core::vector<vk::Shader_Stage_Create_Info>& shaderStagesInfo)
{
  m_shaderStagesInfo = shaderStagesInfo;
}

void Render_Pass::build(flecs::system renderPassSystem)
{
  m_renderPassSystem = renderPassSystem;
  initializeRenderPass();
  initializeFramebuffers();
  initializePipeline();
}

core::shared_ptr<vk::Graphics_Pipeline> Render_Pass::getGraphicsPipeline()
{
  return m_graphicsPipeline;
}

void Render_Pass::render()
{
  core::shared_ptr<Renderer_Frame> frame = m_graph->m_rendererFrame;

  core::array<VkClearValue, 4u> clearValues;
  clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
  clearValues[1].color = { {0.0f, 0.5f, 0.0f, 1.0f} };
  clearValues[2].color = { {0.0f, 1.0f, 0.0f, 1.0f} };
  clearValues[3].depthStencil = { 1.0f, 0 };

  VkRect2D renderArea;
  renderArea.extent = m_framebufferExtent;
  renderArea.offset = VkOffset2D{ 0, 0 };

  frame->getGraphicsCommandBuffer()->setViewport(vk::Viewport({
    .x = 0.0f, .y = 0.0f,
    .width = static_cast<core::float32>(m_framebufferExtent.width),
    .height = static_cast<core::float32>(m_framebufferExtent.height),
    .minDepth = 0.0f, .maxDepth = 1.0f }));

  frame->getGraphicsCommandBuffer()->setScissor(vk::Scissor({
    .offset = { 0, 0 },
    .extent = m_framebufferExtent}));

  frame->getGraphicsCommandBuffer()->beginRenderPass(m_renderPass, m_framebuffers[frame->getSwapchainImageIndex()], clearValues, renderArea);
  frame->getGraphicsCommandBuffer()->bindPipeline(m_graphicsPipeline);

  m_renderPassSystem.run();

  frame->getGraphicsCommandBuffer()->endRenderPass();
}

void Render_Pass::initializeRenderPass()
{
  core::vector<vk::Attachment_Description> attachmentsDescriptions;
  core::vector<VkImageLayout> colorLayouts;

  attachmentsDescriptions.reserve(m_colorOutputs.size());
  colorLayouts.reserve(m_colorOutputs.size());

  for (auto colorAttachment : m_colorOutputs)
  {
    attachmentsDescriptions.push_back(vk::Attachment_Description({
      .format        = colorAttachment.format,
      .samples       = colorAttachment.samples,
      .colorOp       = vk::attachment_op::gClearStore,
      .stenicilOp    = vk::attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }));

    colorLayouts.push_back(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  }

  if (m_depthStencilOutput.has_value())
  {
    attachmentsDescriptions.push_back(vk::Attachment_Description({
      .format        = m_depthStencilOutput.value().format,
      .samples       = m_depthStencilOutput.value().samples,
      .colorOp       = vk::attachment_op::gClearStore,
      .stenicilOp    = vk::attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }));
  }

  vk::Subpass_Description subpassDescription;
  if (m_depthStencilOutput.has_value())
  {
    subpassDescription = vk::Subpass_Description{ vk::Subpass_Description::Initialize_Color_Array_Depth { colorLayouts, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL } };
  }
  else
  {
    subpassDescription = vk::Subpass_Description{ vk::Subpass_Description::Initialize_Color_Array { colorLayouts } };
  }

  vk::Subpass_Dependency subpassDependency({
    .srcSubpass      = VK_SUBPASS_EXTERNAL,
    .dstSubpass      = 0,
    .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    .srcAccessMask   = 0u,
    .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    .dependencyFlags = 0 });

  m_renderPass = core::allocateShared<vk::Render_Pass>(
    m_graph->m_device,
    core::span<const vk::Subpass_Description>{ &subpassDescription, 1u },
    core::span<const vk::Subpass_Dependency>{ &subpassDependency, 1u },
    attachmentsDescriptions);
}

void Render_Pass::initializeFramebuffers()
{
  core::vector<core::shared_ptr<vk::Image_View>> outputs;
  outputs.reserve(m_colorOutputs.size() + m_depthStencilOutput.has_value());

  for (auto colorAttachment : m_colorOutputs)
  {
    outputs.push_back(core::allocateShared<vk::Image_View>(core::allocateShared<vk::Color_Attachment>(vk::Color_Attachment::Initialize{
      .device          = m_graph->m_device,
      .format          = colorAttachment.format,
      .extent          = colorAttachment.extent,
      .sampled         = true,
      .explicitResolve = false,
      .mipLevelsCount  = 1u,
      .samples         = colorAttachment.samples })));
  }

  if (m_depthStencilOutput.has_value())
  {
    outputs.push_back(core::allocateShared<vk::Image_View>(core::allocateShared<vk::Depth_Stencil_Attachment>(vk::Depth_Stencil_Attachment::Initialize{
      .device             = m_graph->m_device,
      .depthStencilFormat = m_depthStencilOutput.value().format,
      .extent             = m_depthStencilOutput.value().extent,
      .sampled            = true,
      .explicitResolve    = false,
      .mipLevelsCount     = 1u,
      .samples            = m_depthStencilOutput.value().samples })));
  }

  m_framebuffers.reserve(m_graph->m_swapchainImagesCount);
  for (core::uint32 i = 0; i < m_graph->m_swapchainImagesCount; ++i)
  {
    m_framebuffers.push_back(core::allocateShared<vk::Framebuffer>(
      m_graph->m_device,
      m_renderPass,
      outputs,
      m_framebufferExtent,
      1u));
  }
}

void Render_Pass::initializePipeline()
{
  core::array<VkDynamicState, 2> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
  };

  core::vector<vk::Pipeline_Color_Blend_Attachment_State> colorBlendAttachmentsStates;
  colorBlendAttachmentsStates.reserve(m_colorOutputs.size());
  for (auto colorAttachment : m_colorOutputs)
  {
    colorBlendAttachmentsStates.push_back(colorAttachment.blendAttachmentState);
  }

  vk::Graphics_Pipeline::Description description{
    .vertexInputState = vk::Vertex_Input_State_Create_Info({
      .bindings = {},
      .attributes = {} }),

    .tessellationState = vk::Tessellation_State_Create_Info(3u),

    .inputAsseblyState = vk::Input_Assembly_State_Create_Info({
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .primitiveRestartEnable = false }),

    .viewportState = vk::Viewport_State_Create_Info{ 1u, 1u },

    .rasterizationState = vk::Rasterization_State_Create_Info({
      .depthClampEnable = false,
      .rasterizerDiscardEnable = false,
      .polygonMode = VK_POLYGON_MODE_FILL,
      .cullMode = VK_CULL_MODE_BACK_BIT,
      .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
      .depthBiasEnable = false,
      .depthBiasConstantFactor = 0.0f,
      .depthBiasClamp = 0.0f,
      .depthBiasSlopeFactor = 0.0f,
      .lineWidth = 1.f }),

    .multisampleState = vk::Multisample_State_Create_Info({
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
      .sampleShadingEnable = VK_FALSE,
      .minSampleShading = 0.2f,
      .alphaToCoverageEnable = false,
      .alphaToOneEnable = false }),

    .depthStencilState = vk::Depth_Stencil_State_Create_Info({
      .depthTestEnable = true,
      .depthWriteEnable = true,
      .depthCompareOp = VK_COMPARE_OP_LESS,
      .depthBoundsTestEnable = false,
      .stencilTestEnable = false,
      .front = {},
      .back = {},
      .minDepthBounds = 0.0f,
      .maxDepthBounds = 1.0f }),

    .colorBlendState = vk::Color_Blend_State_Create_Info({
      .attachments = colorBlendAttachmentsStates,
      .blendConstants = { 0.f, 0.f, 0.f, 0.f },
      .logicOpEnable = false,
      .logicOp = VK_LOGIC_OP_COPY }),

    .dynamicState = vk::Dynamic_State_Create_Info(dynamicStates),
  };

  core::shared_ptr<vk::Descriptor_Set_Layout> descriptorSetLayout = core::allocateShared<vk::Descriptor_Set_Layout>(m_graph->m_device, m_descriptorLayoutBindings, true);
  core::shared_ptr<vk::Pipeline_Layout> pipelineLayout = core::allocateShared<vk::Pipeline_Layout>(m_graph->m_device, descriptorSetLayout, m_pushConstantRange.value());

  m_graphicsPipeline = core::allocateShared<vk::Graphics_Pipeline>(m_renderPass, pipelineLayout, description);
}

Render_Graph::Render_Graph(core::shared_ptr<Renderer_Frame> rendererFrame, core::uint32 swapchainImagesCount)
  : m_rendererFrame{ rendererFrame }
  , m_device{ rendererFrame->getCore()->getDevice() }
  , m_swapchainImagesCount{ swapchainImagesCount }
{}

void Render_Graph::render()
{
  m_rendererFrame->startFrame();
  for (auto pass : m_passes)
  {
    pass.lock()->render();
  }
  m_rendererFrame->endFrame();
}

core::shared_ptr<Renderer_Frame> Render_Graph::getRendererFrame()
{
  return m_rendererFrame;
}

core::shared_ptr<Render_Pass> Render_Graph::addPass(const VkExtent2D& framebufferExtent, VkPipelineStageFlags queue)
{
  core::shared_ptr<Render_Pass> pass = core::allocateShared<Render_Pass>(Render_Pass(shared_from_this(), framebufferExtent, queue));
  m_passes.push_back(pass);
  return pass;
}

}