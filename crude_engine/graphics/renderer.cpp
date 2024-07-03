#include <vulkan/vulkan.hpp>
#include <cstring>
#include <algorithm>
#include <crude_shaders/shader.frag.inl>
#include <crude_shaders/shader.mesh.inl>
#include <crude_shaders/shader.task.inl>

module crude.graphics.renderer;

import crude.core.logger;
import crude.math.fuicont;
import crude.math.convert;
import crude.math.matrix;
import crude.scene.image;
import crude.resources.image_loader;
import crude.math.constants;
import crude.graphics.format_helper;
import crude.graphics.generate_mipmaps;
import crude.graphics.flush;
import crude.graphics.vertex_gpu;
import crude.graphics.index_gpu;
import crude.scene.world;

namespace crude::graphics
{

constexpr core::array<scene::Vertex_CPU, 8u> vertices = {
  scene::Vertex_CPU{math::Float3{-0.5f, -0.5f, 0.0f}, math::Float3{1.0f, 0.0f, 0.0f}, math::Float2{1.0f, 0.0f}, math::Float3{}, math::Float3{}, math::Float3{}},
  scene::Vertex_CPU{math::Float3{0.5f, -0.5f, 0.0f}, math::Float3{0.0f, 1.0f, 0.0f}, math::Float2{0.0f, 0.0f}, math::Float3{}, math::Float3{}, math::Float3{}},
  scene::Vertex_CPU{math::Float3{0.5f, 0.5f, 0.0f}, math::Float3{0.0f, 0.0f, 1.0f}, math::Float2{0.0f, 1.0f}, math::Float3{}, math::Float3{}, math::Float3{}},
  scene::Vertex_CPU{math::Float3{-0.5f, 0.5f, 0.0f}, math::Float3{1.0f, 1.0f, 0.0f}, math::Float2{1.0f, 1.0f}, math::Float3{}, math::Float3{}, math::Float3{}},
  
  scene::Vertex_CPU{math::Float3{-0.5f, -0.5f, -0.5f}, math::Float3{1.0f, 0.0f, 0.0f}, math::Float2{1.0f, 0.0f}, math::Float3{}, math::Float3{}, math::Float3{}},
  scene::Vertex_CPU{math::Float3{0.5f, -0.5f, -0.5f}, math::Float3{0.0f, 1.0f, 0.0f}, math::Float2{0.0f, 0.0f}, math::Float3{}, math::Float3{}, math::Float3{}},
  scene::Vertex_CPU{math::Float3{0.5f, 0.5f, -0.5f}, math::Float3{0.0f, 0.0f, 1.0f}, math::Float2{0.0f, 1.0f}, math::Float3{}, math::Float3{}, math::Float3{}},
  scene::Vertex_CPU{math::Float3{-0.5f, 0.5f, -0.5f}, math::Float3{1.0f, 1.0f, 0.0f}, math::Float2{1.0f, 1.0f}, math::Float3{}, math::Float3{}, math::Float3{}}
};

core::array<scene::Index_Triangle_CPU, 4> indices = 
{
  scene::Index_Triangle_CPU{0u, 1u, 2u},
  scene::Index_Triangle_CPU{2u, 3u, 0u},
  scene::Index_Triangle_CPU{4u, 5u, 6u}, 
  scene::Index_Triangle_CPU{6u, 7u, 4u}
};

constexpr core::array<const char* const, 4> deviceEnabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SPIRV_1_4_EXTENSION_NAME, VK_EXT_MESH_SHADER_EXTENSION_NAME, VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME };
constexpr core::array<const char*, 1> instanceEnabledLayers = { "VK_LAYER_KHRONOS_validation" };

Renderer::Renderer(core::shared_ptr<system::SDL_Window_Container> windowContainer, core::shared_ptr<scene::World> world)
  : m_windowContainer(windowContainer)
  , m_currentFrame(0u)
  , m_uniformBufferDesc{ 
      Uniform_Buffer_Descriptor(0u, VK_SHADER_STAGE_MESH_BIT_EXT),
      Uniform_Buffer_Descriptor(0u, VK_SHADER_STAGE_MESH_BIT_EXT)}
  , m_textureSamplerDesc{Combined_Image_Sampler_Descriptor(1u, VK_SHADER_STAGE_FRAGMENT_BIT), Combined_Image_Sampler_Descriptor(1u, VK_SHADER_STAGE_FRAGMENT_BIT)}
  , m_world(world)
{
  initializeInstance();
  initializeSurface();
  initializeDevice();
  initializeSwapchain();
  initalizeDescriptorSet();
  initalizeCommandPool();
  initializeColorResources();
  initializeDepthImage();
  initalizeGraphicsPipeline();
  initializeSwapchainFramebuffers();
  initializeTextureImage();
  initializeSampler();
  initializeModelBuffer();
  initializeUniformBuffers();
  initializeCommandBuffers();
  initializeSyncObjects();
}

Renderer::~Renderer()
{
  m_device->waitIdle();
}

void Renderer::drawFrame()
{
  m_inFlightFences[m_currentFrame]->wait();
  const Swap_Chain_Next_Image acquireNextImageResult = m_swapchain->acquireNextImage(m_imageAvailableSemaphores[m_currentFrame]);

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
  const bool graphicsQueueSubmited = m_graphicsQueue->sumbit(
    core::span(&m_graphicsCommandBuffers[m_currentFrame], 1u),
    waitStageMasks,
    core::span(&m_imageAvailableSemaphores[m_currentFrame], 1u),
    core::span(&m_renderFinishedSemaphores[m_currentFrame], 1u),
    m_inFlightFences[m_currentFrame]);

  if (!graphicsQueueSubmited)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to submit draw command buffer!");
  }

  const Queue_Present_Result presentResult = m_presentQueue->present(
    core::span(&m_swapchain, 1u),
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

void Renderer::initializeInstance()
{
  // Calculate extensions for instance
  const auto surfaceExtensions = Surface::requiredExtensions();
  const auto debugUtilsExtensions = Debug_Utils_Messenger::requiredExtensions();
  
  core::vector<const char*> enabledExtensions(surfaceExtensions.size() + debugUtilsExtensions.size());
  for (core::size_t i = 0u; i < surfaceExtensions.size(); ++i)
  {
    enabledExtensions[i] = surfaceExtensions[i];
  }
  for (core::size_t i = 0u; i < debugUtilsExtensions.size(); ++i)
  {
    enabledExtensions[i + surfaceExtensions.size()] = debugUtilsExtensions[i];
  }
  
  
  auto debugCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                          VkDebugUtilsMessageTypeFlagsEXT              messageType,
                          const VkDebugUtilsMessengerCallbackDataEXT*  pCallbackData,
                          void*                                        pUserData) -> VKAPI_ATTR VkBool32 {
      core::log(core::Debug::Channel::Graphics, "validation layer: %s", pCallbackData->pMessage);
      return VK_FALSE;
   };

  // Initialize instance
  const Application application({
    .pApplicationName   = "crude_example", 
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0), 
    .pEngineName        = "crude_engine", 
    .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion         = VK_API_VERSION_1_0});

  m_instance = core::allocateShared<Instance>(debugCallback, application, enabledExtensions, instanceEnabledLayers);

  // Initialize debugCallback
  m_debugUtilsMessenger = core::allocateShared<Debug_Utils_Messenger>(m_instance, debugCallback);
}

void Renderer::initializeSurface()
{
  m_surface = core::allocateShared<Surface>(m_instance, m_windowContainer);
}

void Renderer::initializeDevice()
{
  core::shared_ptr<Physical_Device> physicalDevice = pickPhysicalDevice();
  if (!physicalDevice)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to find suitable physical device!");
    return;
  }
  initializeLogicDevice(physicalDevice);
}

void Renderer::initializeSwapchain()
{
  Surface_Capabilities_KHR surfaceCapabilites = m_device->getPhysicalDevice()->getSurfaceCapabilitis(m_surface);
  const VkSurfaceFormatKHR surfaceFormat = m_device->getPhysicalDevice()->findSurfaceFormat(m_surface, VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
  const VkPresentModeKHR presentMode = m_device->getPhysicalDevice()->findSurfacePresentMode(m_surface, VK_PRESENT_MODE_MAILBOX_KHR);
  const VkExtent2D extent = surfaceCapabilites.calculateSurfaceExtentInPixels({ m_windowContainer->getWidth(), m_windowContainer->getHeight()});

  core::uint32 imageCount = surfaceCapabilites.getMinImageCount() + 1u;
  if (surfaceCapabilites.getMinImageCount() > 0u && imageCount > surfaceCapabilites.getMaxImageCount())
  {
    imageCount = surfaceCapabilites.getMaxImageCount();
  }

  core::vector<core::uint32> queueFamilyIndices = { m_graphicsQueue->getFamilyIndex(), m_presentQueue->getFamilyIndex() };

  m_swapchain = core::allocateShared<Swap_Chain>(
    m_device,
    m_surface,
    surfaceFormat,
    extent,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    imageCount,
    1u,
    queueFamilyIndices,
    surfaceCapabilites.getCurrentTransform(),
    VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    presentMode,
    true,
    0u,
    nullptr);

  m_swapchainImages = m_swapchain->getSwapchainImages();
  m_swapchainImagesViews.resize(m_swapchainImages.size());
  for (core::uint32 i = 0; i < m_swapchainImages.size(); ++i)
  {
    m_swapchainImagesViews[i] = core::allocateShared<Image_View>(m_swapchainImages[i], surfaceFormat.format, Image_Subresource_Range(m_swapchainImages[i]));
  }
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
      .swapchain = m_swapchain, 
      .colorOp = attachment_op::gDontCare })
  };

  auto renderPass = core::allocateShared<Render_Pass>(m_device, subpasses, subpassesDependencies, attachments);
  return renderPass;
}

void Renderer::initalizeDescriptorSet()
{
  core::array<Descriptor_Set_Layout_Binding, 2u> layoutBindings =
  {
    m_uniformBufferDesc[0],
    m_textureSamplerDesc[0]
  };

  core::array<Descriptor_Pool_Size, 2u> poolSizes =
  {
    Uniform_Buffer_Pool_Size(cFramesCount),
    Combined_Image_Sampler_Pool_Size(cFramesCount)
  };

  auto descriptorPool = core::allocateShared<Descriptor_Pool>(m_device, poolSizes);
  auto descriptorSetLayout = core::allocateShared<Descriptor_Set_Layout>(m_device, layoutBindings);

  for (auto& m_descriptorSet : m_descriptorSets)
    m_descriptorSet = core::allocateShared<Descriptor_Set>(m_device, descriptorPool, descriptorSetLayout);
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
  renderArea.extent = m_swapchain->getExtent();
  renderArea.offset.x = 0.f;
  renderArea.offset.y = 0.f;

  commandBuffer->beginRenderPass(m_graphicsPipeline->getRenderPass(), m_swapchainFramebuffers[imageIndex], clearValues, renderArea);

  commandBuffer->bindPipeline(m_graphicsPipeline);

  commandBuffer->setViewport(Viewport({
    .x = 0.0f, .y = 0.0f,
    .width = static_cast<core::float32>(m_swapchain->getExtent().width),
    .height = static_cast<core::float32>(m_swapchain->getExtent().height),
    .minDepth = 0.0f, .maxDepth = 1.0f }));

  commandBuffer->setScissor(Scissor({
    .offset = { 0, 0 },
    .extent = m_swapchain->getExtent() }));

  commandBuffer->bindModelBuffer(0u, m_modelBuffer);

  updateUniformBuffer(m_currentFrame);
  commandBuffer->bindDescriptorSets(m_graphicsPipeline, core::span(&m_descriptorSets[m_currentFrame], 1u), {});
  
  commandBuffer->drawMeshTasks(1u);
  //commandBuffer->drawIndexed(3 * indices.size(), 1, 0, 0);

  commandBuffer->endRenderPass();

  if (!commandBuffer->end())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to record command buffer!");
  }
}

void Renderer::updateUniformBuffer(core::uint32 currentImage)
{
  Camera_GPU* data = m_uniformBuffer[currentImage]->mapUnsafe();
  *data = Camera_GPU(m_world->getCamera());
  m_uniformBuffer[currentImage]->unmap();
}

void Renderer::initalizeGraphicsPipeline()
{
  core::shared_ptr<Shader_Module> taskShaderModule = core::allocateShared<Shader_Module>(m_device, crude::shaders::shader::task);
  core::shared_ptr<Shader_Module> meshShaderModule = core::allocateShared<Shader_Module>(m_device, crude::shaders::shader::mesh);
  core::shared_ptr<Shader_Module> fragShaderModule = core::allocateShared<Shader_Module>(m_device, crude::shaders::shader::frag);

  core::array<Shader_Stage_Create_Info, 3u> shaderStagesInfo =
  {
    Task_Shader_Stage_Create_Info(taskShaderModule, "main"),
    Mesh_Shader_Stage_Create_Info(meshShaderModule, "main"),
    Fragment_Shader_Stage_Create_Info(fragShaderModule, "main"),
  };

  Vertex_Input_State_Create_Info vertexInputStateInfo({
    .bindings = core::span(&Vertex_GPU::getBindingDescription(), 1u),
    .attributes = Vertex_GPU::getAttributeDescriptions()});
  
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
    .frontFace = VK_FRONT_FACE_CLOCKWISE,
    .depthBiasEnable = false,
    .depthBiasConstantFactor = 0.0f,
    .depthBiasClamp = 0.0f,
    .depthBiasSlopeFactor = 0.0f,
    .lineWidth = 1.f});

  Multisample_State_Create_Info multisampling({
    .rasterizationSamples = m_device->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount(),
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
  
  core::shared_ptr<Pipeline_Layout> pipelineLayout = core::allocateShared<Pipeline_Layout>(m_device, core::vector<core::shared_ptr<const Descriptor_Set_Layout>>{ m_descriptorSets[0]->getSetLayout() });

  m_graphicsPipeline = core::allocateShared<Pipeline>(
    m_device,
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
  m_graphicsCommandPool = core::allocateShared<Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, m_graphicsQueue->getFamilyIndex());
  m_transferCommandPool = m_graphicsCommandPool; // = core::allocateShared<Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueIndices.transferFamily.value());
}

void Renderer::initializeColorResources()
{
  m_colorAttachment = core::allocateShared<Color_Attachment>(
    m_device, m_swapchainImages.front()->getFormat(), m_swapchain->getExtent(), 1u, 
    m_device->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount(), VK_SHARING_MODE_EXCLUSIVE);
  m_colorAttachmentView = core::allocateShared<Image_View>(m_colorAttachment, Image_Subresource_Range(m_colorAttachment));
}

void Renderer::initializeDepthImage()
{
  const VkFormat depthFormat = findDepthFormatSupportedByDevice(m_device->getPhysicalDevice(), depth_formats::gDepthStencilCandidates);
  m_depthStencilAttachment = core::allocateShared<Depth_Stencil_Attachment>(
    m_device, depthFormat, m_swapchain->getExtent(), 1u,
    m_device->getPhysicalDevice()->getProperties().getMaximumUsableSampleCount(), VK_SHARING_MODE_EXCLUSIVE);

  auto commandBuffer = core::allocateShared<Command_Buffer>(m_transferCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  m_depthStencilAttachment->transitionMipLayoutUpload(commandBuffer, 0u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

  m_depthImageView = core::allocateShared<Image_View>(m_depthStencilAttachment, Image_Subresource_Range(m_depthStencilAttachment));
}

void Renderer::initializeSwapchainFramebuffers()
{
  m_swapchainFramebuffers.resize(m_swapchainImagesViews.size());
  for (core::uint32 i = 0; i < m_swapchainFramebuffers.size(); ++i)
  {
    core::vector<core::shared_ptr<Image_View>> attachments = { m_colorAttachmentView, m_depthImageView, m_swapchainImagesViews[i] };
    m_swapchainFramebuffers[i] = core::allocateShared<Framebuffer>(m_device, m_graphicsPipeline->getRenderPass(), attachments, m_swapchain->getExtent(), 1u);
  }
}

void Renderer::initializeTextureImage()
{
  scene::Image image = resources::loadImage("../../crude_example/basic_triangle_examle/texture.jpg", scene::IMAGE_FORMAT_RGB_ALPHA).value();
  auto commandBuffer = core::allocateShared<Command_Buffer>(m_transferCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  VkExtent3D extent;
  extent.width = image.getWidth();
  extent.height = image.getHeight();
  extent.depth = 1u;
  m_texture = core::allocateShared<Image_2D>(
    commandBuffer, 
    VK_FORMAT_R8G8B8A8_SRGB, 
    Image::Mip_Data(extent, image.getTexelsSpan()),
    image.calculateMaximumMipLevelsCount(),
    VK_SHARING_MODE_EXCLUSIVE);
  commandBuffer->begin();
  generateMipmaps(commandBuffer, m_texture, VK_FILTER_LINEAR);
  commandBuffer->end();
  flush(commandBuffer);
  m_textureView = core::allocateShared<Image_View>(m_texture, Image_Subresource_Range(m_texture));
}

void Renderer::initializeSampler()
{
  m_sampler = core::allocateShared<Sampler>(m_device, csamlper_state::gMagMinMipLinearRepeat);
}

void Renderer::initializeModelBuffer()
{
  scene::Mesh_Range range;
  range.vertexOffset = 0;
  range.indexOffset = 0;
  range.vertexNum = vertices.size();
  range.indexNum = indices.size() * 3;
  scene::Mesh mesh;
  mesh.setVertices(vertices);
  mesh.setIndices(indices);
  scene::Model_Geometry modelGeometry;
  modelGeometry.setRanges(core::span(&range, 1u));
  modelGeometry.setMeshes(core::span(&mesh, 1u));
  core::shared_ptr<graphics::Command_Buffer> commandBuffer = core::allocateShared<graphics::Command_Buffer>(m_transferCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  m_modelBuffer = core::allocateShared<graphics::Model_Buffer>(commandBuffer, modelGeometry);
}

void Renderer::initializeUniformBuffers()
{
  for (core::uint32 i = 0; i < cFramesCount; ++i)
  {
    m_uniformBuffer[i] = core::allocateShared<Uniform_Buffer<Camera_GPU>>(m_device);
    m_uniformBufferDesc[i].update(m_uniformBuffer[i]);
  }
  for (core::uint32 i = 0; i < cFramesCount; ++i)
  {
    m_textureSamplerDesc[i].update(m_textureView, m_sampler);
  }
  for (core::uint32 i = 0; i < cFramesCount; i++)
  {
    const core::array<Write_Descriptor_Set, 2> descriptorWrites =
    {
      Write_Descriptor_Set(m_descriptorSets[i], m_uniformBufferDesc[i]),
      Write_Descriptor_Set(m_descriptorSets[i], m_textureSamplerDesc[i])
    };
    m_device->updateDescriptorSets(descriptorWrites, {});
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
    m_imageAvailableSemaphores[i] = core::allocateShared<Semaphore>(m_device);
    m_renderFinishedSemaphores[i] = core::allocateShared<Semaphore>(m_device);
    m_inFlightFences[i] = core::allocateShared<Fence>(m_device, VK_FENCE_CREATE_SIGNALED_BIT);
  }
}

core::shared_ptr<Physical_Device> Renderer::pickPhysicalDevice()
{
  auto physicalDevices = m_instance->getPhysicalDevices();
  for (auto& physicalDevice : physicalDevices)
  {
    bool supportGraphics = false;
    bool supportPresent = false;

    // !TODO
    for (core::uint32 i = 0; i < physicalDevice->getQueueFamilyProperties().size(); ++i)
    {
      if (physicalDevice->getQueueFamilyProperties()[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) supportGraphics = true;
      if (physicalDevice->checkPresentSupport(m_surface, i)) supportPresent = true;
    }

    const bool extensionsSupported = physicalDevice->checkExtensionSupport(deviceEnabledExtensions);
    const bool swapChainAdequate = physicalDevice->checkSurfaceSupport(m_surface);
    const auto& supportedFeatures = physicalDevice->getFeatures();

    if (supportGraphics && supportPresent && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy)
    {
      core::logInfo(core::Debug::Channel::Graphics, "Selected physical device %s %i", physicalDevice->getProperties().getDeviceName(), physicalDevice->getProperties().getDeviceType());
      return physicalDevice;
    }
  }
  return nullptr;
}

void Renderer::initializeLogicDevice(core::shared_ptr<const Physical_Device> physicalDevice)
{
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;
  deviceFeatures.sampleRateShading = VK_TRUE;
 
  core::array<Device_Queue_Descriptor, 2> queueInfos =
  {
    Device_Queue_Descriptor(physicalDevice, VK_QUEUE_GRAPHICS_BIT),
    Device_Queue_Descriptor(physicalDevice, m_surface),
  };

  m_device = core::allocateShared<Device>(physicalDevice, queueInfos, deviceFeatures, deviceEnabledExtensions, instanceEnabledLayers);

  for (auto it = m_device->getQueueDescriptors().begin(); it != m_device->getQueueDescriptors().end(); ++it)
  {
    if (it->getDenotation() == DEVICE_QUEUE_DENOTATION_GRAPHICS_BITS)
      m_graphicsQueue = m_device->getQueueByFamily(it->queueFamilyIndex, 0u).valueOr(nullptr);
    if (it->getDenotation() == DEVICE_QUEUE_DENOTATION_PRESENT_BITS)
      m_presentQueue = m_device->getQueueByFamily(it->queueFamilyIndex, 0u).valueOr(nullptr);
  }
  if (!m_presentQueue) m_presentQueue = m_graphicsQueue;
  if (!m_transferQueue) m_presentQueue = m_graphicsQueue;
}

}