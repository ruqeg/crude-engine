#include <vulkan/vulkan.hpp>
#include <cstring>

module crude.graphics.renderer;

import crude.core.logger;
import crude.math.fuicont;

namespace crude::graphics
{

struct Vertex
{
  math::MFLOAT3 pos;
  math::MFLOAT3 color;

  static const VkVertexInputBindingDescription& getBindingDescription() {
    static VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
  }

  static const core::array<VkVertexInputAttributeDescription, 2>& getAttributeDescriptions() {
    static core::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
  }
};

constexpr core::array<Vertex, 3u> vertices = 
{
    Vertex{math::MFLOAT3{ 0.0f,-0.5f, 0.0f}, math::MFLOAT3{1.0f, 0.0f, 0.0f}},
    Vertex{math::MFLOAT3{ 0.5f, 0.5f, 0.0f}, math::MFLOAT3{0.0f, 1.0f, 0.0f}},
    Vertex{math::MFLOAT3{-0.5f, 0.5f, 0.0f}, math::MFLOAT3{0.0f, 0.0f, 1.0f}}
};

constexpr core::array<const char*, 1> deviceEnabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
constexpr core::array<const char*, 1> instanceEnabledLayers = { "VK_LAYER_KHRONOS_validation" };

Renderer::Renderer(core::Shared_Ptr<system::SDL_Window_Container> windowContainer)
  :
  m_windowContainer(windowContainer),
  m_currentFrame(0u)
{
  initializeInstance();
  initializeSurface();
  initializeDevice();
  initializeSwapchain();
  initalizeGraphicsPipeline();
  initalizeCommandPool();
  initializeDepthImage();
  initializeSwapchainFramebuffers();
  initializeVertexBuffer();
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

  recordCommandBuffer(m_commandBuffers[m_currentFrame], imageIndex);

  core::uint32 waitStageMasks[1] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  const bool graphicsQueueSubmited = m_graphicsQueue->sumbit(
    core::span(&m_commandBuffers[m_currentFrame], 1u),
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

  m_instance = core::makeShared<Instance>(debugCallback, application, enabledExtensions, instanceEnabledLayers);

  // Initialize debugCallback
  m_debugUtilsMessenger = core::makeShared<Debug_Utils_Messenger>(m_instance, debugCallback);
}

void Renderer::initializeSurface()
{
  m_surface = core::makeShared<Surface>(m_instance, m_windowContainer);
}

void Renderer::initializeDevice()
{
  core::Shared_Ptr<Physical_Device> physicalDevice = pickPhysicalDevice();
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
  core::vector<uint32_t> queueFamilyIndices = { queueIndices.graphicsFamily.value(), queueIndices.presentFamily.value() };

  m_swapchain = core::makeShared<Swap_Chain>(
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
    m_swapchainImagesViews[i] = core::makeShared<Image_View>(m_device, m_swapchainImages[i], surfaceFormat.format, Image_Subresource_Range(m_swapchainImages[i]));
  }
}

core::Shared_Ptr<Render_Pass> Renderer::initializeRenderPass()
{
  core::array<Attachment_Reference, 1u> colorAttachmentsRef = 
  { 
    Attachment_Reference(0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) 
  };
  Attachment_Reference depthAttachmentRef(1u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

  core::array<Subpass_Description, 1u> subpasses = 
  {
    Subpass_Description(VK_PIPELINE_BIND_POINT_GRAPHICS, {}, colorAttachmentsRef, &depthAttachmentRef, {})
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
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
      VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE),

    Attachment_Description( // depth
      findDepthFormat(),
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE)
  };

  auto renderPass = core::makeShared<Render_Pass>(m_device, subpasses, subpassesDependencies, attachments);
  return renderPass;
}

core::Shared_Ptr<Descriptor_Set_Layout> Renderer::initalizeDescriptorSetLayout()
{
  core::array<Descriptor_Set_Layout_Binding, 1u> layoutBindings =
  {
    Descriptor_Set_Layout_Binding(0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT)
  };

  auto descriptorSetLayout = crude::core::makeShared<crude::graphics::Descriptor_Set_Layout>(m_device, layoutBindings);
  return descriptorSetLayout;
}

void Renderer::recordCommandBuffer(core::Shared_Ptr<Command_Buffer> commandBuffer, core::uint32 imageIndex)
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

  core::array<Vertex_Buffer_Bind, 1u> vertexBuffersBinds =
  {
    Vertex_Buffer_Bind(m_vertexBuffer, 0u)
  };

  commandBuffer->bindVertexBuffers(0u, vertexBuffersBinds);
  commandBuffer->draw(vertices.size(), 1, 0, 0);

  commandBuffer->endRenderPass();

  if (!commandBuffer->end())
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to record command buffer!");
  }
}

void Renderer::initalizeGraphicsPipeline()
{
  const char* vertShaderPathA = "../../crude_example/basic_triangle_examle/shader.vert.spv";
  const char* fragShaderPathA = "../../crude_example/basic_triangle_examle/shader.frag.spv";

  const auto vertShaderCode = readFile(vertShaderPathA);
  const auto fragShaderCode = readFile(fragShaderPathA);
  auto vertShaderModule = core::makeShared<Shader_Module>(m_device, vertShaderCode);
  auto fragShaderModule = core::makeShared<Shader_Module>(m_device, fragShaderCode);

  core::array<Shader_Stage_Create_Info, 2u> shaderStagesInfo =
  {
    Shader_Stage_Create_Info(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule, "main"),
    Shader_Stage_Create_Info(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule, "main"),
  };

  auto vertexInputStateInfo = Vertex_Input_State_Create_Info(core::span(&Vertex::getBindingDescription(), 1u), Vertex::getAttributeDescriptions());
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

  core::vector<core::Shared_Ptr<Descriptor_Set_Layout>> descriptorSetLayouts = { };
  auto m_pipelineLayout = core::makeShared<Pipeline_Layout>(m_device, descriptorSetLayouts);

  m_graphicsPipeline = core::makeShared<Pipeline>(
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
  m_commandPool = core::makeShared<Command_Pool>(m_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueIndices.graphicsFamily.value());
}

void Renderer::initializeDepthImage()
{
  VkSurfaceCapabilitiesKHR surfaceCapabilites = m_device->getPhysicalDevice()->getSurfaceCapabilitis(m_surface);
  const VkExtent2D extent = chooseSwapExtent(surfaceCapabilites);

  const VkFormat depthFormat = findDepthFormat();
  m_depthImage = core::makeShared<Image>(
    m_device,
    0u,
    depthFormat,
    extent,
    1u,
    1u,
    VK_SAMPLE_COUNT_1_BIT,
    VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_SHARING_MODE_EXCLUSIVE,
    VK_IMAGE_LAYOUT_UNDEFINED);

  VkMemoryRequirements memRequirements = m_depthImage->getMemoryRequirements();

  m_depthImageDeviceMemory = core::makeShared<Device_Memory>(m_device, memRequirements.size, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  m_depthImageDeviceMemory->bind(*m_depthImage);

  auto commandBuffer = core::makeShared<Command_Buffer>(m_device, m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  commandBuffer->begin();

  core::array<Image_Memory_Barrier, 1u> barriers = 
  {
    Image_Memory_Barrier(m_depthImage, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, Image_Subresource_Range(m_depthImage, 0u, 1u, 0u, 1u))
  };
  commandBuffer->barrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, barriers);
  commandBuffer->end();

  m_graphicsQueue->sumbit(crude::core::span(&commandBuffer, 1u));
  m_graphicsQueue->waitIdle();

  m_depthImageView = makeShared<Image_View>(m_device, m_depthImage, depthFormat, Image_Subresource_Range(m_depthImage));
}

void Renderer::initializeSwapchainFramebuffers()
{
  m_swapchainFramebuffers.resize(m_swapchainImagesViews.size());
  for (core::uint32 i = 0; i < m_swapchainFramebuffers.size(); ++i)
  {
    core::vector<core::Shared_Ptr<Image_View>> attachments = { m_swapchainImagesViews[i], m_depthImageView };

    m_swapchainFramebuffers[i] = core::makeShared<Framebuffer>(m_device, m_graphicsPipeline->getRenderPass(), attachments, m_swapchain->getExtent().width, m_swapchain->getExtent().height, 1u);
  }
}

void Renderer::initializeVertexBuffer()
{
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

  auto stagingBuffer = core::makeShared<Buffer>(m_device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  auto memRequirements = stagingBuffer->getMemoryRequirements();
  auto staggingBufferMemory = core::makeShared<Device_Memory>(m_device, memRequirements.size, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  staggingBufferMemory->bind(*stagingBuffer);
  auto data = staggingBufferMemory->map();
  if (data.hasValue())
  {
    std::memcpy(data.value(), vertices.data(), bufferSize);
    staggingBufferMemory->unmap();
  }

  m_vertexBuffer = core::makeShared<Buffer>(m_device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
  memRequirements = m_vertexBuffer->getMemoryRequirements();
  m_vertexBufferMemory = core::makeShared<Device_Memory>(m_device, memRequirements.size, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  m_vertexBufferMemory->bind(*m_vertexBuffer);

  auto commandBuffer = core::makeShared<Command_Buffer>(m_device, m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  commandBuffer->begin();
  commandBuffer->copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);
  commandBuffer->end();
  m_graphicsQueue->sumbit(core::span(&commandBuffer, 1u));
  m_graphicsQueue->waitIdle();
}

void Renderer::initializeCommandBuffers()
{
  for (auto& commandBuffer : m_commandBuffers)
  {
    commandBuffer = core::makeShared<Command_Buffer>(m_device, m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  }
}

void Renderer::initializeSyncObjects()
{
  for (core::uint32 i = 0; i < cFramesCount; i++)
  {
    m_imageAvailableSemaphores[i] = core::makeShared<Semaphore>(m_device);
    m_renderFinishedSemaphores[i] = core::makeShared<Semaphore>(m_device);
    m_inFlightFences[i] = core::makeShared<Fence>(m_device, VK_FENCE_CREATE_SIGNALED_BIT);
  }
}

core::Shared_Ptr<Physical_Device> Renderer::pickPhysicalDevice()
{
  auto physicalDevices = m_instance->getPhysicalDevices();
  for (auto& physicalDevice : physicalDevices)
  {
    const Queue_Family_Indices queueIndices = findDeviceQueueFamilies(*physicalDevice);
    const bool extensionsSupported = checkDeviceExtensionSupport(*physicalDevice);
    const bool swapChainAdequate = checkSurfaceSupport(*physicalDevice, m_surface);
    const auto& supportedFeatures = physicalDevice->getFeatures();

    if (queueIndices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy)
    {
      core::logInfo(core::Debug::Channel::Graphics, "Selected physical device %s %i", physicalDevice->getProperties().deviceName, physicalDevice->getProperties().deviceType);
      return physicalDevice;
    }
  }
  return nullptr;
}

Renderer::Queue_Family_Indices Renderer::findDeviceQueueFamilies(core::Shared_Ptr<Physical_Device> physicalDevice)
{
  Queue_Family_Indices indices;
  
  const auto& queueProperties = physicalDevice->getQueueFamilyProperties();
  for (core::uint32 i = 0; i < queueProperties.size(); ++i)
  {
    if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphicsFamily = i;
  
    if (physicalDevice->getSupportSurface(m_surface, i))
      indices.presentFamily = i;
  }

  return indices;
}

bool Renderer::checkDeviceExtensionSupport(core::Shared_Ptr<Physical_Device> physicalDevice)
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

bool Renderer::checkSurfaceSupport(core::Shared_Ptr<Physical_Device> physicalDevice, core::Shared_Ptr<Surface> surface)
{
  return !physicalDevice->getSurfaceFormats(surface).empty() && !physicalDevice->getSurfacePresentModes(surface).empty();
}

void Renderer::initializeLogicDevice(core::Shared_Ptr<Physical_Device> physicalDevice)
{
  const Queue_Family_Indices queueIndices = findDeviceQueueFamilies(physicalDevice);

  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  float queuePriorities[] = { 1.f };

  core::array<Device_Queue_Create_Info, 2> deviceQueueCreateInfos =
  {
    Device_Queue_Create_Info(queueIndices.graphicsFamily.value(), queuePriorities),
    Device_Queue_Create_Info(queueIndices.presentFamily.value(), queuePriorities)
  };

  m_device = core::makeShared<Device>(physicalDevice, deviceQueueCreateInfos, deviceFeatures, deviceEnabledExtensions, instanceEnabledLayers);

  m_graphicsQueue = m_device->getQueue(queueIndices.graphicsFamily.value(), 0u);
  m_presentQueue = m_device->getQueue(queueIndices.presentFamily.value(), 0u);
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