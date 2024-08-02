#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <cstring>
#include <algorithm>
#include <directxmath/DirectXMath.h>
#include <crude_shaders/shader.frag.inl>
#include <crude_shaders/shader.mesh.inl>
#include <crude_shaders/shader.task.inl>
#include <functional>

module crude.graphics.renderer;

import crude.core.logger;
import crude.scene.image;
import crude.graphics.format_helper;
import crude.graphics.generate_mipmaps;
import crude.graphics.flush;
import crude.graphics.mesh_buffer;
import crude.scene.free_camera_script;
import crude.resources.gltf_loader;
import crude.graphics.renderer_system;

namespace crude::graphics
{

Renderer::Renderer(core::shared_ptr<Renderer_Base> rendererBase, flecs::world world)
  : m_rendererBase(rendererBase)
  , m_currentFrame(0u)
  , m_perFrameUniformBufferDesc{
      Uniform_Buffer_Descriptor(0u, VK_SHADER_STAGE_MESH_BIT_EXT),
      Uniform_Buffer_Descriptor(0u, VK_SHADER_STAGE_MESH_BIT_EXT)}
  , m_submeshesDrawsBufferDescriptor(1u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT)
  , m_meshletBufferDescriptor(2u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT)
  , m_vertexBufferDescriptor(3u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT)
  , m_primitiveIndicesBufferDescriptor(4u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT)
  , m_vertexIndicesBufferDescriptor(5u, VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT)
  , m_textureSamplerDesc{Combined_Image_Sampler_Descriptor(6u, VK_SHADER_STAGE_FRAGMENT_BIT), Combined_Image_Sampler_Descriptor(6u, VK_SHADER_STAGE_FRAGMENT_BIT)}
  , m_world(world)
  , m_cameraNode(world, "camera node")
{
  initalizeDescriptorSet();
  initalizeCommandPool();
  initializeColorResources();
  initializeDepthImage();
  initalizeGraphicsPipeline();
  initializeSwapchainFramebuffers();

  resources::GLTF_Loader gltfLoader(m_transferCommandPool, world);
  m_node = gltfLoader.loadNodeFromFile("../../crude_example/basic_triangle_examle/resources/sponza.glb");

  scene::Camera camera;
  camera.calculateViewToClipMatrix(DirectX::XM_PIDIV4, m_rendererBase->getWindowContainer()->getAspect(), 0.05f, 100.0f);
  m_cameraNode.set<scene::Camera>(std::move(camera));
  scene::Transform transform(m_cameraNode);
  transform.setTranslation(0.0, 0.0, -2.0);
  m_cameraNode.set<scene::Transform>(std::move(transform));
  scene::script::Free_Camera_Component freeCamera;
  m_cameraNode.set<scene::script::Free_Camera_Component>(std::move(freeCamera));
  m_cameraNode.child_of(m_node);

  initializeUniformBuffers();
  initializeCommandBuffers();
  initializeSyncObjects();
}

void Renderer::drawFrame()
{
  m_inFlightFences[m_currentFrame]->wait();
  const Swap_Chain_Next_Image acquireNextImageResult = m_rendererBase->getSwapchain()->acquireNextImage(m_imageAvailableSemaphores[m_currentFrame]);

  if (acquireNextImageResult.outOfDate())
  {
    // !TODO
    return;
  }
  else if (acquireNextImageResult.failedAcquire())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to acquire swap chain image!");
  }

  core::uint32 imageIndex = acquireNextImageResult.getImageIndex().value();

  m_inFlightFences[m_currentFrame]->reset();

  recordCommandBuffer(m_graphicsCommandBuffers[m_currentFrame], imageIndex);

  core::uint32 waitStageMasks[1] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  const bool graphicsQueueSubmited = m_rendererBase->getGraphicsQueue()->sumbit(
    core::span(&m_graphicsCommandBuffers[m_currentFrame], 1u),
    waitStageMasks,
    core::span(&m_imageAvailableSemaphores[m_currentFrame], 1u),
    core::span(&m_renderFinishedSemaphores[m_currentFrame], 1u),
    m_inFlightFences[m_currentFrame]);

  if (!graphicsQueueSubmited)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to submit draw command buffer!");
  }

  const Queue_Present_Result presentResult = m_rendererBase->getPresentQueue()->present(
    core::span(&m_rendererBase->getSwapchain(), 1u),
    core::span(&imageIndex, 1u),
    core::span(&m_renderFinishedSemaphores[m_currentFrame], 1u));

  bool framebufferResized = false;
  if (presentResult.outOfDate() || presentResult.suboptimal() || framebufferResized)
  {
    // !TODO
  }
  else if (presentResult.failed())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to present swap chain image!");
  }

  m_currentFrame = (m_currentFrame + 1u) % cFramesCount;
}

core::shared_ptr<Render_Pass> Renderer::initializeRenderPass()
{
  core::array<Subpass_Description, 1u> subpasses = 
  {
    Subpass_Description(
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
  };

  core::array<Subpass_Dependency, 1u> subpassesDependencies = 
  {
    Subpass_Dependency(
      VK_SUBPASS_EXTERNAL, 0u,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      0u,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      0u)
  };

  core::array<Attachment_Description, 3> attachments =
  {
    Color_Attachment_Description({
      .attachment = m_colorAttachment}),
    Depth_Attachment_Description({
      .attachment = m_depthStencilAttachment }),
    Swapchain_Attachment_Description({ 
      .swapchain = m_rendererBase->getSwapchain(),
      .colorOp = attachment_op::gDontCare })
  };

  auto renderPass = core::allocateShared<Render_Pass>(m_rendererBase->getDevice(), subpasses, subpassesDependencies, attachments);
  return renderPass;
}

void Renderer::initalizeDescriptorSet()
{
  core::array<Descriptor_Set_Layout_Binding, 7u> layoutBindings =
  {
    m_perFrameUniformBufferDesc[0],
    m_textureSamplerDesc[0],
    m_submeshesDrawsBufferDescriptor,
    m_vertexBufferDescriptor,
    m_meshletBufferDescriptor,
    m_primitiveIndicesBufferDescriptor,
    m_vertexIndicesBufferDescriptor,
  };

  core::array<Descriptor_Pool_Size, 7u> poolSizes =
  {
    Uniform_Buffer_Pool_Size(cFramesCount),
    Combined_Image_Sampler_Pool_Size(cFramesCount),
    Storage_Buffer_Pool_Size(1u),
    Storage_Buffer_Pool_Size(1u),
    Storage_Buffer_Pool_Size(1u),
    Storage_Buffer_Pool_Size(1u),
    Storage_Buffer_Pool_Size(1u),
  };

  auto descriptorPool = core::allocateShared<Descriptor_Pool>(m_rendererBase->getDevice(), poolSizes);
  m_descriptorSetLayout = core::allocateShared<Descriptor_Set_Layout>(m_rendererBase->getDevice(), layoutBindings, true);
}

void Renderer::recordCommandBuffer(core::shared_ptr<Command_Buffer> commandBuffer, core::uint32 imageIndex)
{
  if (!commandBuffer->begin())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to begin recording command buffer!");
  }

  core::array<VkClearValue, 2u> clearValues;
  clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
  clearValues[1].depthStencil = { 1.0f, 0 };

  VkRect2D renderArea;
  renderArea.extent = m_rendererBase->getSwapchain()->getExtent();
  renderArea.offset.x = 0.f;
  renderArea.offset.y = 0.f;

  commandBuffer->beginRenderPass(m_graphicsPipeline->getRenderPass(), m_swapchainFramebuffers[imageIndex], clearValues, renderArea);

  commandBuffer->bindPipeline(m_graphicsPipeline);

  commandBuffer->setViewport(Viewport({
    .x = 0.0f, .y = 0.0f,
    .width = static_cast<core::float32>(m_rendererBase->getSwapchain()->getExtent().width),
    .height = static_cast<core::float32>(m_rendererBase->getSwapchain()->getExtent().height),
    .minDepth = 0.0f, .maxDepth = 1.0f }));

  commandBuffer->setScissor(Scissor({
    .offset = { 0, 0 },
    .extent = m_rendererBase->getSwapchain()->getExtent() }));

  Per_Frame* data = m_perFrameUniformBuffer[m_currentFrame]->mapUnsafe();
  data->camera = Camera_UBO(m_cameraNode);
  m_perFrameUniformBuffer[m_currentFrame]->unmap();

  // !TODO
  Render_Component renderComponent;
  renderComponent.pipeline = m_graphicsPipeline;
  renderComponent.commandBuffer = commandBuffer;
  renderComponent.perFrameUniformBufferDescriptor = &m_perFrameUniformBufferDesc[m_currentFrame];
  renderComponent.textureSamplerDescriptor = &m_textureSamplerDesc[m_currentFrame];
  renderComponent.submeshesDrawsBufferDescriptor = &m_submeshesDrawsBufferDescriptor;
  renderComponent.vertexBufferDescriptor = &m_vertexBufferDescriptor;
  renderComponent.meshletBufferDescriptor = &m_meshletBufferDescriptor;
  renderComponent.primitiveIndicesBufferDescriptor = &m_primitiveIndicesBufferDescriptor;
  renderComponent.vertexIndicesBufferDescriptor = &m_vertexIndicesBufferDescriptor;
  m_world.set<Render_Component>(renderComponent);

  m_world.query<core::shared_ptr<scene::Mesh>, core::shared_ptr<graphics::Mesh_Buffer>>().each(std::function(rendereSystemEach));

  commandBuffer->endRenderPass();

  if (!commandBuffer->end())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to record command buffer!");
  }
}

void Renderer::initalizeGraphicsPipeline()
{
  core::shared_ptr<Shader_Module> taskShaderModule = core::allocateShared<Shader_Module>(m_rendererBase->getDevice(), crude::shaders::shader::task);
  core::shared_ptr<Shader_Module> meshShaderModule = core::allocateShared<Shader_Module>(m_rendererBase->getDevice(), crude::shaders::shader::mesh);
  core::shared_ptr<Shader_Module> fragShaderModule = core::allocateShared<Shader_Module>(m_rendererBase->getDevice(), crude::shaders::shader::frag);

  core::array<Shader_Stage_Create_Info, 3u> shaderStagesInfo =
  {
    Task_Shader_Stage_Create_Info(taskShaderModule, "main"),
    Mesh_Shader_Stage_Create_Info(meshShaderModule, "main"),
    Fragment_Shader_Stage_Create_Info(fragShaderModule, "main"),
  };

  Vertex_Input_State_Create_Info vertexInputStateInfo({
    .bindings = {},
    .attributes = {}});
  
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
    .lineWidth = 1.f});

  Multisample_State_Create_Info multisampling({
    .rasterizationSamples = m_rendererBase->getDevice()->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount(),
    .sampleShadingEnable = VK_TRUE,
    .minSampleShading = 0.2f,
    .alphaToCoverageEnable = false,
    .alphaToOneEnable = false});

  Depth_Stencil_State_Create_Info depthStencil({
    .depthTestEnable = true,
    .depthWriteEnable = true,
    .depthCompareOp = VK_COMPARE_OP_LESS,
    .depthBoundsTestEnable = false,
    .stencilTestEnable = false,
    .front = {},
    .back = {},
    .minDepthBounds = 0.0f,
    .maxDepthBounds = 1.0f});

  core::array<Pipeline_Color_Blend_Attachment_State, 1u> colorBlendAttachments = {
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

  Color_Blend_State_Create_Info colorBlending({
    .attachments = colorBlendAttachments, 
    .blendConstants = { 0.f, 0.f, 0.f, 0.f }, 
    .logicOpEnable = false, 
    .logicOp = VK_LOGIC_OP_COPY });
  
  core::array<VkDynamicState, 2> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
  };
  Dynamic_State_Create_Info dynamicStateInfo(dynamicStates);
  
  core::shared_ptr<Pipeline_Layout> pipelineLayout = core::allocateShared<Pipeline_Layout>(
    m_rendererBase->getDevice(), m_descriptorSetLayout,
    graphics::Push_Constant_Range<Per_Mesh>(VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_TASK_BIT_EXT));

  m_graphicsPipeline = core::allocateShared<Pipeline>(
    m_rendererBase->getDevice(),
    initializeRenderPass(),
    pipelineLayout,
    nullptr,
    shaderStagesInfo,
    vertexInputStateInfo,
    tesselationInfo,
    inputAssembly,
    viewportState,
    rasterizer,
    multisampling,
    depthStencil,
    colorBlending,
    dynamicStateInfo,
    0u);
}

void Renderer::initalizeCommandPool()
{
  m_graphicsCommandPool = core::allocateShared<Command_Pool>(m_rendererBase->getDevice(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, m_rendererBase->getGraphicsQueue()->getFamilyIndex());
  m_transferCommandPool = m_graphicsCommandPool; // = core::allocateShared<Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueIndices.transferFamily.value());
}

void Renderer::initializeColorResources()
{
  m_colorAttachment = core::allocateShared<Color_Attachment>(
    m_rendererBase->getDevice(), m_rendererBase->getSwapchainImages().front()->getFormat(), m_rendererBase->getSwapchain()->getExtent(), 1u,
    m_rendererBase->getDevice()->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount(), VK_SHARING_MODE_EXCLUSIVE);
  m_colorAttachmentView = core::allocateShared<Image_View>(m_colorAttachment, Image_Subresource_Range(m_colorAttachment));
}

void Renderer::initializeDepthImage()
{
  const VkFormat depthFormat = findDepthFormatSupportedByDevice(m_rendererBase->getDevice()->getPhysicalDevice(), depth_formats::gDepthStencilCandidates);
  m_depthStencilAttachment = core::allocateShared<Depth_Stencil_Attachment>(
    m_rendererBase->getDevice(), depthFormat, m_rendererBase->getSwapchain()->getExtent(), 1u,
    m_rendererBase->getDevice()->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount(), VK_SHARING_MODE_EXCLUSIVE);

  auto commandBuffer = core::allocateShared<Command_Buffer>(m_transferCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  m_depthStencilAttachment->transitionMipLayoutUpload(commandBuffer, 0u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

  m_depthImageView = core::allocateShared<Image_View>(m_depthStencilAttachment, Image_Subresource_Range(m_depthStencilAttachment));
}

void Renderer::initializeSwapchainFramebuffers()
{
  m_swapchainFramebuffers.resize(m_rendererBase->getSwapchainImagesViews().size());
  for (core::uint32 i = 0; i < m_swapchainFramebuffers.size(); ++i)
  {
    core::vector<core::shared_ptr<Image_View>> attachments = { m_colorAttachmentView, m_depthImageView, m_rendererBase->getSwapchainImagesViews()[i] };
    m_swapchainFramebuffers[i] = core::allocateShared<Framebuffer>(m_rendererBase->getDevice(), m_graphicsPipeline->getRenderPass(), attachments, m_rendererBase->getSwapchain()->getExtent(), 1u);
  }
}

void Renderer::initializeUniformBuffers()
{
  for (core::uint32 i = 0; i < cFramesCount; ++i)
  {
    m_perFrameUniformBuffer[i] = core::allocateShared<Uniform_Buffer<Per_Frame>>(m_rendererBase->getDevice());
    m_perFrameUniformBufferDesc[i].update(m_perFrameUniformBuffer[i]);
  }
}

void Renderer::initializeCommandBuffers()
{
  for (core::uint32 i = 0; i < cFramesCount; ++i)
  {
    m_graphicsCommandBuffers[i] = core::allocateShared<Command_Buffer>(m_graphicsCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  }
}

void Renderer::initializeSyncObjects()
{
  for (core::uint32 i = 0; i < cFramesCount; i++)
  {
    m_imageAvailableSemaphores[i] = core::allocateShared<Semaphore>(m_rendererBase->getDevice());
    m_renderFinishedSemaphores[i] = core::allocateShared<Semaphore>(m_rendererBase->getDevice());
    m_inFlightFences[i] = core::allocateShared<Fence>(m_rendererBase->getDevice(), VK_FENCE_CREATE_SIGNALED_BIT);
  }
}

}