#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
 
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#undef max

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

import crude.core.filesystem;
import crude.core.optional;
import crude.core.shared_ptr;
import crude.core.std_containers_heap;
import crude.core.std_containers_stack;

import crude.graphics.instance;
import crude.graphics.debug_utils_messenger;
import crude.graphics.surface;
import crude.graphics.physical_device;
import crude.graphics.device;
import crude.graphics.queue;
import crude.graphics.swap_chain;
import crude.graphics.image_view;
import crude.graphics.swap_chain_image;
import crude.graphics.render_pass;
import crude.graphics.descriptor_set_layout;
import crude.graphics.shader_module;
import crude.graphics.pipeline_layout;
import crude.graphics.shader_stage_create_info;
import crude.graphics.pipeline;
import crude.graphics.command_pool;
import crude.graphics.device_memory;
import crude.graphics.command_buffer;
import crude.graphics.image_memory_barrier;
import crude.graphics.framebuffer;
import crude.graphics.buffer;
import crude.graphics.sampler;
import crude.graphics.descriptor_pool;
import crude.graphics.descriptor_set;
import crude.graphics.write_descriptor_set;
import crude.graphics.fence;
import crude.graphics.semaphore;
import crude.graphics.subpass_description;
import crude.graphics.subpass_dependency;
import crude.graphics.descriptor_image_info;
import crude.graphics.attachment_description;
import crude.graphics.viewport_state_create_info;
import crude.graphics.vertex_input_state_create_info;
import crude.graphics.tessellation_state_create_info;
import crude.graphics.rasterization_state_create_info;
import crude.graphics.multisample_state_create_info;
import crude.graphics.depth_stencil_state_create_info;
import crude.graphics.input_assembly_state_create_info;
import crude.graphics.dynamic_state_create_info;
import crude.graphics.descriptor_set_layout;
import crude.graphics.image_subresource_range;
import crude.graphics.application;
import crude.graphics.color_blend_state_create_info;

import crude.system.sdl_window_container;

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <limits>

class Test_Application
{
public:
  struct Queue_Family_Indices
  {
    crude::core::Optional<uint32_t> graphicsFamily;
    crude::core::Optional<uint32_t> presentFamily;

    bool isComplete() const
    {
      return graphicsFamily.hasValue() && presentFamily.hasValue();
    }
  };
public:
  void run(
    HINSTANCE  hinstance
  )
  {
    initWindow(
      hinstance
    );
    initVulkan();
    mainLoop();
  }
private:
  void initWindow(
    HINSTANCE  hinstance
  )
  {
    m_window = crude::core::makeShared<crude::system::SDL_Window_Container>("SDL Tutorial", 800, 600, SDL_WINDOW_VULKAN);
  }

  void initVulkan()
  {
    const auto surfaceExtensions = crude::graphics::Surface::requiredExtensions();
    const auto debugUtilsExtensions = crude::graphics::Debug_Utils_Messenger::requiredExtensions();

    crude::core::vector<const char*> enabledExtensions(surfaceExtensions.size() + debugUtilsExtensions.size());
    for (std::size_t i = 0u; i < surfaceExtensions.size(); ++i)
    {
      enabledExtensions[i] = surfaceExtensions[i];
    }
    for (std::size_t i = 0u; i < debugUtilsExtensions.size(); ++i)
    {
      enabledExtensions[i + surfaceExtensions.size()] = debugUtilsExtensions[i];
    }
    
    // Initialize instance
    crude::core::array<const char*, 1u> enabledLayers = { "VK_LAYER_KHRONOS_validation" };
    m_instance = crude::core::makeShared<crude::graphics::Instance>(
      debugCallback,
      crude::graphics::Application(),
      crude::core::span(enabledExtensions.data(), enabledExtensions.size()),
      crude::core::span(enabledLayers.data(), enabledLayers.size()));

    // Initialize debugCallback
    m_debugUtilsMessenger = crude::core::makeShared<crude::graphics::Debug_Utils_Messenger>(m_instance, debugCallback);

    // Initialize surface
    m_surface = crude::core::makeShared<crude::graphics::Surface>(m_instance, m_window);

    // Pick physical device
    auto physicalDevices = m_instance->getPhysicalDevices();
    for (auto& physicalDevice : physicalDevices)
    {
      const Queue_Family_Indices queueIndices = findQueueFamilies(*physicalDevice);
      const bool extensionsSupported = checkDeviceExtensionSupport(*physicalDevice);
      const bool swapChainAdequate = checkSurfaceSupport(*physicalDevice, m_surface);
      const auto& supportedFeatures = physicalDevice->getFeatures();

      if (queueIndices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy)
      {
        std::cout << "Selected physical device " << physicalDevice->getProperties().deviceName << " " << physicalDevice->getProperties().deviceType << std::endl;
        m_physicalDevice = physicalDevice;
        break;
      }
    }

    if (!m_physicalDevice)
    {
      throw std::runtime_error("failed to find suitable physical device");
    }

    // Initialize logic device
    const Queue_Family_Indices queueIndices = findQueueFamilies(*m_physicalDevice);

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    float queuePriorities[] = { 1.f };

    crude::core::vector<crude::graphics::Device_Queue_Create_Info> deviceQueueCreateInfos =
    {
      crude::graphics::Device_Queue_Create_Info(queueIndices.graphicsFamily.value(), crude::core::span<float>(queuePriorities, 1u)),
      crude::graphics::Device_Queue_Create_Info(queueIndices.presentFamily.value(), crude::core::span<float>(queuePriorities, 1u))
    };
    
    crude::core::vector<const char*> deviceEnabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    m_device = crude::core::makeShared<crude::graphics::Device>(
      m_physicalDevice,
      crude::core::span<crude::graphics::Device_Queue_Create_Info>(deviceQueueCreateInfos.data(), deviceQueueCreateInfos.size()),
      deviceFeatures,
      crude::core::span<const char*>(deviceEnabledExtensions.data(), deviceEnabledExtensions.size()),
      crude::core::span<const char*>(enabledLayers.data(), enabledLayers.size()));

    m_graphicsQueue = m_device->getQueue(queueIndices.graphicsFamily.value(), 0u);
    m_presentQueue = m_device->getQueue(queueIndices.presentFamily.value(), 0u);

    // Initialize swapchain
    VkSurfaceCapabilitiesKHR surfaceCapabilites = m_physicalDevice->getSurfaceCapabilitis(m_surface);
    const VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(m_physicalDevice->getSurfaceFormats(m_surface));
    const VkPresentModeKHR presentMode = chooseSwapPresentMode(m_physicalDevice->getSurfacePresentModes(m_surface));
    const VkExtent2D extent = chooseSwapExtent(surfaceCapabilites);

    uint32_t imageCount = surfaceCapabilites.minImageCount + 1u;
    if (surfaceCapabilites.maxImageCount > 0u && imageCount > surfaceCapabilites.maxImageCount) {
      imageCount = surfaceCapabilites.maxImageCount;
    }

    crude::core::vector<uint32_t> queueFamilyIndices = { queueIndices.graphicsFamily.value(), queueIndices.presentFamily.value() };

    m_swapchain = crude::core::makeShared<crude::graphics::Swap_Chain>(
      m_device,
      m_surface,
      surfaceFormat,
      extent,
      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
      imageCount,
      1u,
      VK_SHARING_MODE_CONCURRENT,
      crude::core::span(queueFamilyIndices.data(), queueFamilyIndices.size()),
      surfaceCapabilites.currentTransform,
      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      presentMode,
      VK_TRUE,
      0u,
      nullptr);

    m_swapchainImages = m_swapchain->getSwapchainImages();
    m_swapchainImagesViews.resize(m_swapchainImages.size());
    for (uint32_t i = 0; i < m_swapchainImages.size(); ++i)
    {
      m_swapchainImagesViews[i] = crude::core::makeShared<crude::graphics::Image_View>(
        m_device,
        m_swapchainImages[i],
        surfaceFormat.format,
        crude::graphics::Image_Subresource_Range(m_swapchainImages[i]));
    }

    crude::graphics::Attachment_Description colorAttachment(
      surfaceFormat.format,
      VK_SAMPLE_COUNT_1_BIT,
      VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE,
      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    crude::core::array<VkAttachmentReference, 1u> colorAttachmentRef;
    colorAttachmentRef[0].attachment = 0u;
    colorAttachmentRef[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    crude::graphics::Attachment_Description depthAttachment(
      findDepthFormat(),
      VK_SAMPLE_COUNT_1_BIT,
      VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE,
      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    VkAttachmentReference depthAttachmentRef;
    depthAttachmentRef.attachment = 1u;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    crude::graphics::Subpass_Description subpass(
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      {},
      crude::core::span(colorAttachmentRef.data(), colorAttachmentRef.size()),
      &depthAttachmentRef,
      {});

    crude::graphics::Subpass_Dependency subpassDependency(
      VK_SUBPASS_EXTERNAL,
      0u,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      0u,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      0u);

    crude::graphics::Attachment_Description attachments[] =
    {
      colorAttachment,
      depthAttachment
    };
    attachments[0];

    m_renderPass = crude::core::makeShared<crude::graphics::Render_Pass>(
      m_device,
      crude::core::span(&subpass, 1u),
      crude::core::span(&subpassDependency, 1u),
      crude::core::span(attachments, 2u));

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.binding          = 0;
    uboLayoutBinding.descriptorCount  = 1;
    uboLayoutBinding.stageFlags       = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.binding          = 0u/* 1 */;
    samplerLayoutBinding.descriptorCount  = 1;
    samplerLayoutBinding.stageFlags       = VK_SHADER_STAGE_FRAGMENT_BIT;

    m_descriptorSetLayout = crude::core::makeShared<crude::graphics::Descriptor_Set_Layout>(
      m_device,
      crude::core::span(&samplerLayoutBinding, 1u));

    const char* vertShaderPathA = "../../crude_example/shader.vert.spv";
    const char* fragShaderPathA = "../../crude_example/shader.frag.spv";

    const auto vertShaderCode = readFile(vertShaderPathA);
    const auto fragShaderCode = readFile(fragShaderPathA);
    auto vertShaderModule = crude::core::makeShared<crude::graphics::Shader_Module>(
      m_device,
      crude::core::span(vertShaderCode.data(), vertShaderCode.size()));
    auto fragShaderModule = crude::core::makeShared<crude::graphics::Shader_Module>(
      m_device,
      crude::core::span(fragShaderCode.data(), fragShaderCode.size()));

    crude::core::array<crude::graphics::Shader_Stage_Create_Info, 2u> shaderStagesInfo =
    {
      crude::graphics::Shader_Stage_Create_Info(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule, "main"),
      crude::graphics::Shader_Stage_Create_Info(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule, "main"),
    };

    auto vertexInputStateInfo = crude::graphics::Vertex_Input_State_Create_Info({}, {});
    auto inputAssembly = crude::graphics::Input_Assembly_State_Create_Info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
    auto viewportState = crude::graphics::Viewport_State_Create_Info(1u, 1u);
    auto rasterizer = crude::graphics::Rasterization_State_Create_Info(
      VK_FALSE,
      VK_FALSE,
      VK_POLYGON_MODE_FILL,
      VK_CULL_MODE_BACK_BIT,
      VK_FRONT_FACE_CLOCKWISE,
      VK_FALSE,
      0.0f,
      0.0f,
      0.0f,
      1.f);
    auto multisampling = crude::graphics::Multisample_State_Create_Info(
      VK_SAMPLE_COUNT_1_BIT,
      VK_FALSE,
      1.0f,
      VK_FALSE,
      VK_FALSE);
    auto depthStencil = crude::graphics::Depth_Stencil_State_Create_Info(
      VK_TRUE,
      VK_TRUE,
      VK_COMPARE_OP_LESS,
      VK_FALSE,
      VK_FALSE,
      {},
      {},
      0.0f,
      1.0f);

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    auto colorBlending = crude::graphics::Color_Blend_State_Create_Info(
      crude::core::span(&colorBlendAttachment, 1u),
      { 0.f, 0.f, 0.f, 0.f },
      VK_FALSE,
      VK_LOGIC_OP_COPY);

    crude::core::array<VkDynamicState, 2u> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    auto dynamicState = crude::graphics::Dynamic_State_Create_Info(crude::core::span(dynamicStates.data(), dynamicStates.size()));

    crude::core::vector<crude::core::Shared_Ptr<crude::graphics::Descriptor_Set_Layout>> descriptorSetLayouts = { m_descriptorSetLayout };
    m_pipelineLayout = crude::core::makeShared<crude::graphics::Pipeline_Layout>(
      m_device, 
      crude::core::span(descriptorSetLayouts.data(), descriptorSetLayouts.size()));

    m_graphicsPipeline = crude::core::makeShared<crude::graphics::Pipeline>(
      m_device,
      m_renderPass,
      m_pipelineLayout,
      nullptr,
      crude::core::span(shaderStagesInfo.data(), shaderStagesInfo.size()),
      vertexInputStateInfo,
      crude::graphics::Tessellation_State_Create_Info(3u),
      inputAssembly,
      viewportState,
      rasterizer,
      multisampling,
      depthStencil,
      colorBlending,
      dynamicState,
      0u);

    m_commandPool = crude::core::makeShared<crude::graphics::Command_Pool>(
      m_device, 
      VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      queueIndices.graphicsFamily.value());

    createDepthImage();

    m_swapchainFramebuffers.resize(m_swapchainImagesViews.size());
    for (uint32_t i = 0; i < m_swapchainFramebuffers.size(); ++i)
    {
      crude::core::vector<crude::core::Shared_Ptr<crude::graphics::Image_View>> attachments = { m_swapchainImagesViews[i], m_depthImageView };

      m_swapchainFramebuffers[i] = crude::core::makeShared<crude::graphics::Framebuffer>(
        m_device,
        m_renderPass,
        attachments,
        extent.width,
        extent.height,
        1u);
    }

    createTextureImageFromFile("../../crude_example/test.png", m_textureImage, m_textureImageMemory);

    m_textureImageView = crude::core::makeShared<crude::graphics::Image_View>(
      m_device,
      m_textureImage,
      m_textureImage->getFormat(),
      crude::graphics::Image_Subresource_Range(m_textureImage));

    m_sampler = crude::core::makeShared<crude::graphics::Sampler>(
      m_device,
      VK_FILTER_LINEAR,
      VK_FILTER_LINEAR,
      VK_SAMPLER_MIPMAP_MODE_LINEAR,
      VK_SAMPLER_ADDRESS_MODE_REPEAT,
      VK_SAMPLER_ADDRESS_MODE_REPEAT,
      VK_SAMPLER_ADDRESS_MODE_REPEAT,
      0.f,
      VK_TRUE,
      m_physicalDevice->getProperties().limits.maxSamplerAnisotropy,
      VK_FALSE,
      VK_COMPARE_OP_ALWAYS,
      0.f,
      0.f,
      VK_BORDER_COLOR_INT_OPAQUE_BLACK,
      VK_FALSE);

    crude::core::vector<VkDescriptorPoolSize> poolSizes = { VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2u} };
    m_descriptorPool = crude::core::makeShared<crude::graphics::Descriptor_Pool>(
      m_device,
      crude::core::span(poolSizes.data(), poolSizes.size()),
      2u);

    m_descriptorSets.resize(2u);
    for (auto& descriptorSet : m_descriptorSets)
    {
      descriptorSet = crude::core::makeShared<crude::graphics::Descriptor_Set>(
        m_device,
        descriptorSetLayouts,
        m_descriptorPool);
    }

    for (uint32_t i = 0; i < 2u; i++) {
      crude::graphics::Descriptor_Image_Info imageInfo(
        m_sampler,
        m_textureImageView,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

      auto descriptorWrites = crude::graphics::Write_Descriptor_Set(
        m_descriptorSets[i],
        0u,
        0u,
        1u,
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        imageInfo,
        crude::core::nullopt);

      m_device->updateDescriptorSets(crude::core::span(&descriptorWrites, 1u), {});
    }

    m_commandBuffers.resize(2u);
    for (auto& commandBuffer : m_commandBuffers)
    {
      commandBuffer = crude::core::makeShared<crude::graphics::Command_Buffer>(
        m_device,
        m_commandPool,
        VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    }

    m_imageAvailableSemaphores.resize(2u);
    m_renderFinishedSemaphores.resize(2u);
    m_inFlightFences.resize(2u);

    for (size_t i = 0; i < 2u; i++) {
      m_imageAvailableSemaphores[i] = crude::core::makeShared<crude::graphics::Semaphore>(m_device);
      m_renderFinishedSemaphores[i] = crude::core::makeShared<crude::graphics::Semaphore>(m_device);
      m_inFlightFences[i] = crude::core::makeShared<crude::graphics::Fence>(m_device, VK_FENCE_CREATE_SIGNALED_BIT);
    }
  }

  void createDepthImage()
  {
    VkSurfaceCapabilitiesKHR surfaceCapabilites = m_physicalDevice->getSurfaceCapabilitis(m_surface);
    const VkExtent2D extent = chooseSwapExtent(surfaceCapabilites);

    const VkFormat depthFormat = findDepthFormat();
    m_depthImage = crude::core::makeShared<crude::graphics::Image>(
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

    m_depthImageDeviceMemory = crude::core::makeShared<crude::graphics::Device_Memory>(
      m_device,
      memRequirements.size,
      memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_depthImageDeviceMemory->bind(*m_depthImage);

    auto commandBuffer = crude::core::makeShared<crude::graphics::Command_Buffer>(
      m_device,
      m_commandPool,
      VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    commandBuffer->begin();

    crude::graphics::Image_Memory_Barrier barrier(
      m_depthImage,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      crude::graphics::Image_Subresource_Range(m_depthImage, 0u, 1u, 0u, 1u));
    crude::core::span barriers(&barrier, 1);
    commandBuffer->barrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, barriers);
    commandBuffer->end();

    m_graphicsQueue->sumbit(crude::core::span(&commandBuffer, 1u));
    m_graphicsQueue->waitIdle();

    m_depthImageView = crude::core::makeShared<crude::graphics::Image_View>(
      m_device,
      m_depthImage,
      depthFormat,
      crude::graphics::Image_Subresource_Range(m_depthImage));
  }

  void createTextureImageFromFile(const char* path, crude::core::Shared_Ptr<crude::graphics::Image>& textureImage, crude::core::Shared_Ptr<crude::graphics::Device_Memory>& textureImageMemory)
  {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    size_t pixelsbsize = 4u * texWidth * texHeight;

    if (!pixels)
    {
      throw std::runtime_error("failed to load texture image!");
    }

    auto stagingBuffer = crude::core::makeShared<crude::graphics::Buffer>(
      m_device,
      4 * texWidth * texHeight,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_SHARING_MODE_EXCLUSIVE,
      crude::core::span<crude::core::uint32>());
    
    VkMemoryRequirements memRequirements = m_depthImage->getMemoryRequirements();

    auto stagingBufferMemory = crude::core::makeShared<crude::graphics::Device_Memory>(
      m_device,
      memRequirements.size,
      memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    stagingBufferMemory->bind(*stagingBuffer);

    void* data = stagingBufferMemory->map().value();
    memcpy(data, pixels, static_cast<size_t>(pixelsbsize));
    stagingBufferMemory->unmap();

    textureImage = crude::core::makeShared<crude::graphics::Image>(
      m_device,
      0u,
      VK_FORMAT_R8G8B8A8_SRGB,
      VkExtent2D{ static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) },
      1u,
      1u,
      VK_SAMPLE_COUNT_1_BIT,
      VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_SHARING_MODE_EXCLUSIVE,
      VK_IMAGE_LAYOUT_UNDEFINED);

    memRequirements = textureImage->getMemoryRequirements();

    textureImageMemory = crude::core::makeShared<crude::graphics::Device_Memory>(
      m_device,
      memRequirements.size,
      memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    textureImageMemory->bind(*textureImage);

    auto commandBuffer = crude::core::makeShared<crude::graphics::Command_Buffer>(
      m_device,
      m_commandPool,
      VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    commandBuffer->begin();

    crude::graphics::Image_Memory_Barrier barrier(
      textureImage,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      crude::graphics::Image_Subresource_Range(textureImage, 0u, 1u, 0u, 1u));

    commandBuffer->barrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, crude::core::span(&barrier, 1u));

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
      static_cast<uint32_t>(texWidth),
      static_cast<uint32_t>(texHeight),
      1
    };
    commandBuffer->copyBufferToImage(stagingBuffer, textureImage, crude::core::span(&region, 1u));

    barrier = crude::graphics::Image_Memory_Barrier(
      textureImage,
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      crude::graphics::Image_Subresource_Range(textureImage, 0u, 1u, 0u, 1u));

    commandBuffer->barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, crude::core::span(&barrier, 1u));

    commandBuffer->end();

    m_graphicsQueue->sumbit(crude::core::span(&commandBuffer, 1u));
    m_graphicsQueue->waitIdle();
  }

  void recreateSwapChain()
  {

  }

  static crude::core::vector<char> readFile(const char* filename) {
    if (!crude::core::Filesystem::getInstance().fileAccess(filename))
    {
      throw std::runtime_error("failed to access file!");
    }

    crude::core::vector<char> buffer;
    if (crude::core::Filesystem::getInstance().read(filename, buffer) != crude::core::Filesystem::RESULT_SUCCESS)
    {
      throw std::runtime_error("failed to read file!");
    }

    return buffer;
  }

  Queue_Family_Indices findQueueFamilies(crude::graphics::Physical_Device& physicalDevice) {
    Queue_Family_Indices indices;
      
    const auto& queueProperties = physicalDevice.getQueueFamilyProperties();
    for (uint32_t i = 0; i < queueProperties.size(); ++i)
    {
      if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        indices.graphicsFamily = i;
      }

      if (physicalDevice.getSupportSurface(m_surface, i))
      {
        indices.presentFamily = i;
      }
    }

    return indices;
  }

  bool checkDeviceExtensionSupport(crude::graphics::Physical_Device& physicalDevice)
  {
    static const crude::core::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    
    const auto& availableExtensions = physicalDevice.getExtensionProperties();
    crude::core::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }

  bool checkSurfaceSupport(crude::graphics::Physical_Device& physicalDevice, crude::core::Shared_Ptr<const crude::graphics::Surface> surface)
  {
    return !physicalDevice.getSurfaceFormats(surface).empty() && !physicalDevice.getSurfacePresentModes(surface).empty();
  }

  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const crude::core::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
      }
    }

    return availableFormats[0];
  }

  VkPresentModeKHR chooseSwapPresentMode(const crude::core::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        return availablePresentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
      return capabilities.currentExtent;
    }

    VkExtent2D actualExtent = {
      static_cast<uint32_t>(m_width),
      static_cast<uint32_t>(m_height)
    };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
  }

  VkFormat findDepthFormat() {
    return findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  }

  VkFormat findSupportedFormat(const crude::core::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
      VkFormatProperties props = m_physicalDevice->getFormatProperties(format);
      
      if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
        return format;
      } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
        return format;
      }
    }

    throw std::runtime_error("failed to find supported format!");
  }

  void mainLoop()
  {
    MSG message;

    while (GetMessage(&message, NULL, 0, 0)) {
      TranslateMessage(&message);
      DispatchMessage(&message);
      drawFrame();
    }

    m_device->waitIdle();
  }

  void drawFrame()
  {
    m_inFlightFences[m_currentFrame]->wait();
    const crude::graphics::Swap_Chain_Next_Image acquireNextImageResult = m_swapchain->acquireNextImage(m_imageAvailableSemaphores[m_currentFrame]);

    if (acquireNextImageResult.outOfDate())
    {
      recreateSwapChain();
      return;
    }
    else if (acquireNextImageResult.failedAcquire())
    {
      throw std::runtime_error("failed to acquire swap chain image!");
    }

    uint32_t imageIndex = acquireNextImageResult.getImageIndex().value();

    m_inFlightFences[m_currentFrame]->reset();

    recordCommandBuffer(m_commandBuffers[m_currentFrame], imageIndex);

    crude::core::uint32 waitStageMasks[1] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    const bool graphicsQueueSubmited = m_graphicsQueue->sumbit(
      crude::core::span(&m_commandBuffers[m_currentFrame], 1u),
      crude::core::span(waitStageMasks, 1u),
      crude::core::span(&m_imageAvailableSemaphores[m_currentFrame], 1u),
      crude::core::span(&m_renderFinishedSemaphores[m_currentFrame], 1u),
      m_inFlightFences[m_currentFrame]);

    if (!graphicsQueueSubmited)
    {
      throw std::runtime_error("failed to submit draw command buffer!");
    }

    const crude::graphics::Queue_Present_Result presentResult = m_presentQueue->present(
      crude::core::span(&m_swapchain, 1u), 
      crude::core::span(&imageIndex, 1u), 
      crude::core::span(&m_renderFinishedSemaphores[m_currentFrame], 1u));

    if (presentResult.outOfDate() || presentResult.suboptimal() || m_framebufferResized)
    {
      m_framebufferResized = false;
      recreateSwapChain();
    }
    else if (presentResult.failed())
    {
      throw std::runtime_error("failed to present swap chain image!");
    }

    m_currentFrame = (m_currentFrame + 1u) % 2u;
  }

  void recordCommandBuffer(crude::core::Shared_Ptr<crude::graphics::Command_Buffer> commandBuffer, uint32_t imageIndex) {
    if (!commandBuffer->begin())
    {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    crude::core::array<VkClearValue, 2u> clearValues;
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRect2D renderArea;
    renderArea.extent = m_swapchain->getExtent();
    renderArea.offset.x = 0.f;
    renderArea.offset.y = 0.f;

    commandBuffer->beginRenderPass(
      m_renderPass,
      m_swapchainFramebuffers[imageIndex],
      crude::core::span(clearValues.data(), clearValues.size()),
      renderArea);

    commandBuffer->bindPipeline(m_graphicsPipeline);

    VkViewport viewport{};
    viewport.x         = 0.0f;
    viewport.y         = 0.0f;
    viewport.width     = static_cast<float>(m_swapchain->getExtent().width);
    viewport.height    = static_cast<float>(m_swapchain->getExtent().height);
    viewport.minDepth  = 0.0f;
    viewport.maxDepth  = 1.0f;
    commandBuffer->setViewport(crude::core::span(&viewport, 1u));

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = m_swapchain->getExtent();
    commandBuffer->setScissor(crude::core::span(&scissor, 1u));

    commandBuffer->bindDescriptorSets(m_graphicsPipeline, crude::core::span(&m_descriptorSets[m_currentFrame], 1u));

    commandBuffer->draw(3u, 1u);
    
    commandBuffer->endRenderPass();

    if (!commandBuffer->end())
    {
      throw std::runtime_error("failed to record command buffer!");
    }
  }

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                      VkDebugUtilsMessageTypeFlagsEXT              messageType,
                                                      const VkDebugUtilsMessengerCallbackDataEXT*  pCallbackData,
                                                      void*                                        pUserData) 
  {
    //if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    OutputDebugStringA((std::string("validation layer: ") + pCallbackData->pMessage + std::string("\n")).c_str());
      std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    //}
    return VK_FALSE;
  }

private:
  crude::core::Shared_Ptr<crude::system::SDL_Window_Container>                      m_window;

  crude::core::Shared_Ptr<crude::graphics::Instance>                                m_instance;
  crude::core::Shared_Ptr<crude::graphics::Debug_Utils_Messenger>                   m_debugUtilsMessenger;
  crude::core::Shared_Ptr<crude::graphics::Surface>                                 m_surface;
  crude::core::Shared_Ptr<crude::graphics::Physical_Device>                         m_physicalDevice;
  crude::core::Shared_Ptr<crude::graphics::Device>                                  m_device;
  crude::core::Shared_Ptr<crude::graphics::Queue>                                   m_graphicsQueue;
  crude::core::Shared_Ptr<crude::graphics::Queue>                                   m_presentQueue;
  crude::core::Shared_Ptr<crude::graphics::Swap_Chain>                              m_swapchain;
  crude::core::vector<crude::core::Shared_Ptr<crude::graphics::Swap_Chain_Image>>   m_swapchainImages;
  crude::core::vector<crude::core::Shared_Ptr<crude::graphics::Image_View>>         m_swapchainImagesViews;
  crude::core::Shared_Ptr<crude::graphics::Render_Pass>                             m_renderPass;
  crude::core::Shared_Ptr<crude::graphics::Descriptor_Set_Layout>                   m_descriptorSetLayout;
  crude::core::Shared_Ptr<crude::graphics::Pipeline_Layout>                         m_pipelineLayout;
  crude::core::Shared_Ptr<crude::graphics::Pipeline>                                m_graphicsPipeline;
  crude::core::Shared_Ptr<crude::graphics::Command_Pool>                            m_commandPool;
  crude::core::Shared_Ptr<crude::graphics::Device_Memory>                           m_depthImageDeviceMemory;
  crude::core::Shared_Ptr<crude::graphics::Image>                                   m_depthImage;
  crude::core::Shared_Ptr<crude::graphics::Image_View>                              m_depthImageView;
  crude::core::vector<crude::core::Shared_Ptr<crude::graphics::Framebuffer>>        m_swapchainFramebuffers;
  crude::core::Shared_Ptr<crude::graphics::Image>                                   m_textureImage;
  crude::core::Shared_Ptr<crude::graphics::Device_Memory>                           m_textureImageMemory;
  crude::core::Shared_Ptr<crude::graphics::Image_View>                              m_textureImageView;
  crude::core::Shared_Ptr<crude::graphics::Sampler>                                 m_sampler;
  crude::core::Shared_Ptr<crude::graphics::Descriptor_Pool>                         m_descriptorPool;
  crude::core::vector<crude::core::Shared_Ptr<crude::graphics::Descriptor_Set>>     m_descriptorSets;
  crude::core::vector<crude::core::Shared_Ptr<crude::graphics::Command_Buffer>>     m_commandBuffers;
  crude::core::vector<crude::core::Shared_Ptr<crude::graphics::Semaphore>>          m_imageAvailableSemaphores;
  crude::core::vector<crude::core::Shared_Ptr<crude::graphics::Semaphore>>          m_renderFinishedSemaphores;
  crude::core::vector<crude::core::Shared_Ptr<crude::graphics::Fence>>              m_inFlightFences;
  uint32_t m_currentFrame = 0u;
  uint32_t m_width = 800u;
  uint32_t m_height = 600u;
  bool m_framebufferResized = false;

  const crude::core::char8*   m_titile = "test";
  const crude::core::char16*  m_titileWide = L"test";
  const crude::core::char16*  m_windowClassWide = L"test_class";
};

int APIENTRY wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nCmdShow)
{
  // init console
  AllocConsole();
  FILE* dummy;
  auto s = freopen_s(&dummy, "CONOUT$", "w", stdout);
 

  ////The window we'll be rendering to
  //SDL_Window* window = NULL;

  ////The surface contained by the window
  //SDL_Surface* screenSurface = NULL;

  //Initialize SDL
  //if (SDL_Init(SDL_INIT_VIDEO) < 0)
  //{
  //  printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  //}
  //else
  //{
  //  //Create window
  //  window = SDL_CreateWindow("SDL Tutorial", 800, 600, SDL_WINDOW_SHOWN);
  //  if (window == NULL)
  //  {
  //    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
  //  }
  //  else
  //  {
  //    //Get window surface
  //    screenSurface = SDL_GetWindowSurface(window);

  //    //Fill the surface white
  //    //SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

  //    //Update the surface
  //    SDL_UpdateWindowSurface(window);

  //    //Hack to get window to stay up
  //    SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }
  //  }
  //}

  ////Destroy window
  //SDL_DestroyWindow(window);

  ////Quit SDL subsystems
  SDL_Quit();/*

  Test_Application testApp;
  try
  {
    testApp.run(hInstance);
  }
  catch (const std::exception& e)
  {
    std::cerr << "exception: " << e.what() << std::endl;
    system("pause");
    return EXIT_FAILURE;
  }*/

  return EXIT_SUCCESS;
}