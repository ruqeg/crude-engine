#include <vulkan/vulkan.hpp>
#include <cstring>
#include <algorithm>

module crude.graphics.renderer;

import crude.core.logger;
import crude.math.fuicont;
import crude.math.convert;
import crude.math.matrix;
import crude.scene.image;
import crude.math.constants;
import crude.graphics.generate_mipmaps;
import crude.graphics.flush;

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

constexpr core::array<const char*, 1> deviceEnabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
constexpr core::array<const char*, 1> instanceEnabledLayers = { "VK_LAYER_KHRONOS_validation" };

Renderer::Renderer(core::shared_ptr<system::SDL_Window_Container> windowContainer)
  :
  m_windowContainer(windowContainer),
  m_currentFrame(0u),
  m_uniformBufferDesc{Uniform_Buffer_Descriptor(0u, VK_SHADER_STAGE_VERTEX_BIT), Uniform_Buffer_Descriptor(0u, VK_SHADER_STAGE_VERTEX_BIT)},
  m_textureSamplerDesc{Combined_Image_Sampler_Descriptor(1u, VK_SHADER_STAGE_FRAGMENT_BIT), Combined_Image_Sampler_Descriptor(1u, VK_SHADER_STAGE_FRAGMENT_BIT)}
{
  initializeInstance();
  initializeSurface();
  initializeDevice();
  initializeSwapchain();
  initalizeDescriptorSet();
  initalizeGraphicsPipeline();
  initalizeCommandPool();
  initializeDepthImage();
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
  const Application application("crude_example", VK_MAKE_VERSION(1, 0, 0), "crude_engine", VK_MAKE_VERSION(1, 0, 0));

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
  VkSurfaceCapabilitiesKHR surfaceCapabilites = m_device->getPhysicalDevice()->getSurfaceCapabilitis(m_surface);
  const VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(m_device->getPhysicalDevice()->getSurfaceFormats(m_surface));
  const VkPresentModeKHR presentMode = chooseSwapPresentMode(m_device->getPhysicalDevice()->getSurfacePresentModes(m_surface));
  const VkExtent2D extent = chooseSwapExtent(surfaceCapabilites);

  core::uint32 imageCount = surfaceCapabilites.minImageCount + 1u;
  if (surfaceCapabilites.maxImageCount > 0u && imageCount > surfaceCapabilites.maxImageCount)
  {
    imageCount = surfaceCapabilites.maxImageCount;
  }

  const Queue_Family_Indices queueIndices = findDeviceQueueFamilies(m_device->getPhysicalDevice());
  core::vector<core::uint32> queueFamilyIndices = { queueIndices.graphicsFamily.value(), queueIndices.presentFamily.value() };

  m_swapchain = core::allocateShared<Swap_Chain>(
    m_device,
    m_surface,
    surfaceFormat,
    extent,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    imageCount,
    1u,
    VK_SHARING_MODE_CONCURRENT,
    queueFamilyIndices,
    surfaceCapabilites.currentTransform,
    VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    presentMode,
    VK_TRUE,
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
    Subpass_Description(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
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

  core::array<Attachment_Description, 2> attachments =
  {
    Attachment_Description( // color
      m_swapchain->getSurfaceFormat().format,
      VK_SAMPLE_COUNT_1_BIT,
      attachment_op::gClearStore,
      attachment_op::gDontCare,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR),

    Attachment_Description( // depth
      findDepthFormat(),
      VK_SAMPLE_COUNT_1_BIT,
      attachment_op::gClearStore,
      attachment_op::gDontCare,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
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

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(m_swapchain->getExtent().width);
  viewport.height = static_cast<float>(m_swapchain->getExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  commandBuffer->setViewport(core::span(&viewport, 1u));

  VkRect2D scissor{};
  scissor.offset = { 0, 0 };
  scissor.extent = m_swapchain->getExtent();
  commandBuffer->setScissor(core::span(&scissor, 1u));

  commandBuffer->bindModelBuffer(0u, m_modelBuffer);

  updateUniformBuffer(m_currentFrame);
  commandBuffer->bindDescriptorSets(m_graphicsPipeline, core::span(&m_descriptorSets[m_currentFrame], 1u), {});
    
  commandBuffer->drawIndexed(3 * indices.size(), 1, 0, 0);

  commandBuffer->endRenderPass();

  if (!commandBuffer->end())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to record command buffer!");
  }
}

void Renderer::updateUniformBuffer(core::uint32 currentImage)
{
  Uniform_Buffer_Object ubo{};

  const core::float32 aspect = m_swapchain->getExtent().width / (core::float32)m_swapchain->getExtent().height;
  m_camera.calculateViewToClipMatrix(math::CPI4, aspect, 0.1f, 10.0f);
  m_camera.calculateWorldToViewMatrix();

  Uniform_Buffer_Object* data = m_uniformBuffer[currentImage]->mapUnsafe();
  math::storeFloat4x4(data->model, math::smatrix::translation(0.0, 0.0, 0.0));
  math::storeFloat4x4(data->view, m_camera.getWorldToViewMatrix());
  math::storeFloat4x4(data->proj, m_camera.getViewToClipMatrix());
  m_uniformBuffer[currentImage]->unmap();
}

void Renderer::initalizeGraphicsPipeline()
{
  const char* vertShaderPathA = "../../crude_example/basic_triangle_examle/shader.vert.spv";
  const char* fragShaderPathA = "../../crude_example/basic_triangle_examle/shader.frag.spv";

  const auto vertShaderCode = readFile(vertShaderPathA);
  const auto fragShaderCode = readFile(fragShaderPathA);
  auto vertShaderModule = core::allocateShared<Shader_Module>(m_device, vertShaderCode);
  auto fragShaderModule = core::allocateShared<Shader_Module>(m_device, fragShaderCode);

  core::array<Shader_Stage_Create_Info, 2u> shaderStagesInfo =
  {
    Shader_Stage_Create_Info(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule, "main"),
    Shader_Stage_Create_Info(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule, "main"),
  };

  auto vertexInputStateInfo = Vertex_Input_State_Create_Info(core::span(&scene::Vertex_GPU::getBindingDescription(), 1u), scene::Vertex_GPU::getAttributeDescriptions());
  auto inputAssembly = Input_Assembly_State_Create_Info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
  auto viewportState = Viewport_State_Create_Info(1u, 1u);
  auto rasterizer = Rasterization_State_Create_Info(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.f);
  auto multisampling = Multisample_State_Create_Info(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, VK_FALSE, VK_FALSE);
  auto depthStencil = Depth_Stencil_State_Create_Info(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 1.0f);

  core::array<Pipeline_Color_Blend_Attachment_State, 1u> colorBlendAttachments =
  {
    Pipeline_Color_Blend_Attachment_State(
      VK_FALSE,
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      VK_BLEND_FACTOR_ONE,
      VK_BLEND_FACTOR_ZERO,
      VK_BLEND_OP_ADD,
      VK_BLEND_FACTOR_ONE,
      VK_BLEND_FACTOR_ZERO,
      VK_BLEND_OP_ADD)
  };

  auto colorBlending = Color_Blend_State_Create_Info(colorBlendAttachments, { 0.f, 0.f, 0.f, 0.f }, VK_FALSE, VK_LOGIC_OP_COPY);

  core::array<VkDynamicState, 2u> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
  auto dynamicState = Dynamic_State_Create_Info(dynamicStates);
  core::vector<core::shared_ptr<const Descriptor_Set_Layout>> descriptorSetLayouts = { m_descriptorSets[0]->getSetLayout() };

  auto m_pipelineLayout = core::allocateShared<Pipeline_Layout>(m_device, descriptorSetLayouts);

  m_graphicsPipeline = core::allocateShared<Pipeline>(
    m_device,
    initializeRenderPass(),
    m_pipelineLayout,
    nullptr,
    shaderStagesInfo,
    vertexInputStateInfo,
    Tessellation_State_Create_Info(3u),
    inputAssembly,
    viewportState,
    rasterizer,
    multisampling,
    depthStencil,
    colorBlending,
    dynamicState,
    0u);
}

void Renderer::initalizeCommandPool()
{
  const Queue_Family_Indices queueIndices = findDeviceQueueFamilies(m_device->getPhysicalDevice());
  m_graphicsCommandPool = core::allocateShared<Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueIndices.graphicsFamily.value());
  m_transferCommandPool = m_graphicsCommandPool; // = core::allocateShared<Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueIndices.transferFamily.value());
}

void Renderer::initializeDepthImage()
{
  VkSurfaceCapabilitiesKHR surfaceCapabilites = m_device->getPhysicalDevice()->getSurfaceCapabilitis(m_surface);
  const VkExtent2D extent = chooseSwapExtent(surfaceCapabilites);
  const VkFormat depthFormat = findDepthFormat();
  m_depthStencilAttachment = core::allocateShared<Depth_Stencil_Attachment>(
    m_device, depthFormat, extent, 1u, VK_SAMPLE_COUNT_1_BIT, VK_SHARING_MODE_EXCLUSIVE);

  auto commandBuffer = core::allocateShared<Command_Buffer>(m_transferCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  m_depthStencilAttachment->transitionMipLayoutUpload(commandBuffer, 0u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

  m_depthImageView = core::allocateShared<Image_View>(m_depthStencilAttachment, Image_Subresource_Range(m_depthStencilAttachment));
}

void Renderer::initializeSwapchainFramebuffers()
{
  m_swapchainFramebuffers.resize(m_swapchainImagesViews.size());
  for (core::uint32 i = 0; i < m_swapchainFramebuffers.size(); ++i)
  {
    core::vector<core::shared_ptr<Image_View>> attachments = { m_swapchainImagesViews[i], m_depthImageView };

    m_swapchainFramebuffers[i] = core::allocateShared<Framebuffer>(m_device, m_graphicsPipeline->getRenderPass(), attachments, m_swapchain->getExtent().width, m_swapchain->getExtent().height, 1u);
  }
}

void Renderer::initializeTextureImage()
{
  scene::Image image;
  if (!image.load("../../crude_example/basic_triangle_examle/texture.jpg", scene::IMAGE_FORMAT_RGB_ALPHA))
    return;

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
    m_uniformBuffer[i] = core::allocateShared<Uniform_Buffer<Uniform_Buffer_Object>>(m_device);
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
    const Queue_Family_Indices queueIndices = findDeviceQueueFamilies(physicalDevice);
    const bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);
    const bool swapChainAdequate = checkSurfaceSupport(physicalDevice, m_surface);
    const auto& supportedFeatures = physicalDevice->getFeatures();

    if (queueIndices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy)
    {
      core::logInfo(core::Debug::Channel::Graphics, "Selected physical device %s %i", physicalDevice->getProperties().deviceName, physicalDevice->getProperties().deviceType);
      return physicalDevice;
    }
  }
  return nullptr;
}

Renderer::Queue_Family_Indices Renderer::findDeviceQueueFamilies(core::shared_ptr<const Physical_Device> physicalDevice)
{
  Queue_Family_Indices indices;
  
  const auto& queueProperties = physicalDevice->getQueueFamilyProperties();
  for (core::uint32 i = 0; i < queueProperties.size(); ++i)
  {
    if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphicsFamily = i;
    //else if (queueProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
    //  indices.transferFamily = i;
  
    if (physicalDevice->getSupportSurface(m_surface, i))
      indices.presentFamily = i;
  }

  return indices;
}

bool Renderer::checkDeviceExtensionSupport(core::shared_ptr<const Physical_Device> physicalDevice)
{
  static const core::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  
  const auto& availableExtensions = physicalDevice->getExtensionProperties();
  core::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
  
  for (const auto& extension : availableExtensions)
  {
    requiredExtensions.erase(extension.extensionName);
  }
  
  return requiredExtensions.empty();
}

bool Renderer::checkSurfaceSupport(core::shared_ptr<const Physical_Device> physicalDevice, core::shared_ptr<Surface> surface)
{
  return !physicalDevice->getSurfaceFormats(surface).empty() && !physicalDevice->getSurfacePresentModes(surface).empty();
}

void Renderer::initializeLogicDevice(core::shared_ptr<const Physical_Device> physicalDevice)
{
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  float queuePriorities[] = { 1.f };

  core::vector<Device_Queue_Create_Info> queueInfos =
  {
    Device_Queue_Create_Info(physicalDevice, VK_QUEUE_GRAPHICS_BIT, queuePriorities),
    Device_Queue_Create_Info(physicalDevice, m_surface, queuePriorities),
  };

  auto uQueueInfos = std::unique(queueInfos.begin(), queueInfos.end());

  m_device = core::allocateShared<Device>(physicalDevice, core::span(queueInfos.begin(), uQueueInfos), deviceFeatures, deviceEnabledExtensions, instanceEnabledLayers);
  m_graphicsQueue = m_device->getQueueByFamily(queueInfos[0].queueFamilyIndex, 0u).valueOr(nullptr);
  m_presentQueue = m_device->getQueueByFamily(queueInfos[1].queueFamilyIndex, 0u).valueOr(nullptr);
  m_transferQueue = m_graphicsQueue;
}

VkSurfaceFormatKHR Renderer::chooseSwapSurfaceFormat(const crude::core::vector<VkSurfaceFormatKHR>& availableFormats)
{
  for (const auto& availableFormat : availableFormats)
  {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      return availableFormat;
  }

  return availableFormats[0];
}

VkPresentModeKHR Renderer::chooseSwapPresentMode(const crude::core::vector<VkPresentModeKHR>& availablePresentModes)
{
  for (const auto& availablePresentMode : availablePresentModes) 
  {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
      return availablePresentMode;
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    return capabilities.currentExtent;

  VkExtent2D actualExtent = 
  {
    static_cast<uint32_t>(m_windowContainer->getWidth()),
    static_cast<uint32_t>(m_windowContainer->getHeight())
  };

  actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
  actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

  return actualExtent;
}

VkFormat Renderer::findDepthFormat()
{
  return findSupportedFormat(
    { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
    VK_IMAGE_TILING_OPTIMAL,
    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkFormat Renderer::findSupportedFormat(const core::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
  for (VkFormat format : candidates)
  {
    VkFormatProperties props = m_device->getPhysicalDevice()->getFormatProperties(format);
    
    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
      return format;
    else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
      return format;
  }

  core::logError(core::Debug::Channel::Graphics, "Failed to find supported format!");
  return VK_FORMAT_UNDEFINED;
}

core::vector<char> Renderer::readFile(const char* filename)
{
  if (!core::Filesystem::getInstance().fileAccess(filename))
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to access file %s!", filename);
  }

  core::vector<char> buffer;
  if (core::Filesystem::getInstance().read(filename, buffer) != core::Filesystem::RESULT_SUCCESS)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to read file %s!", filename);
  }

  return buffer;
}

}