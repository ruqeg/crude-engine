#include <array>

#ifdef __linux__ 
#define VK_USE_PLATFORM_XCB_KHR
#include <xcb/xcb.h>
#elif _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#undef max
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../crude_engine/graphics/vulkan/instance.hpp"
#include "../crude_engine/graphics/vulkan/debug_utils_messenger.hpp"
#include "../crude_engine/graphics/vulkan/surface.hpp"
#include "../crude_engine/graphics/vulkan/physical_device.hpp"
#include "../crude_engine/graphics/vulkan/device.hpp"
#include "../crude_engine/graphics/vulkan/queue.hpp"
#include "../crude_engine/graphics/vulkan/swap_chain.hpp"
#include "../crude_engine/graphics/vulkan/image_view.hpp"
#include "../crude_engine/graphics/vulkan/swap_chain_image.hpp"
#include "../crude_engine/graphics/vulkan/render_pass.hpp"
#include "../crude_engine/graphics/vulkan/descriptor_set_layout.hpp"
#include "../crude_engine/graphics/vulkan/shader_module.hpp"
#include "../crude_engine/graphics/vulkan/pipeline_layout.hpp"
#include "../crude_engine/graphics/vulkan/shader_stage_create_info.hpp"
#include "../crude_engine/graphics/vulkan/pipeline.hpp"
#include "../crude_engine/graphics/vulkan/command_pool.hpp"
#include "../crude_engine/graphics/vulkan/device_memory.hpp"
#include "../crude_engine/graphics/vulkan/command_buffer.hpp"
#include "../crude_engine/graphics/vulkan/image_memory_barrier.hpp"
#include "../crude_engine/graphics/vulkan/framebuffer.hpp"
#include "../crude_engine/graphics/vulkan/buffer.hpp"
#include "../crude_engine/graphics/vulkan/sampler.hpp"
#include "../crude_engine/graphics/vulkan/descriptor_pool.hpp"
#include "../crude_engine/graphics/vulkan/descriptor_set.hpp"
#include "../crude_engine/graphics/vulkan/write_descriptor_set.hpp"
#include "../crude_engine/graphics/vulkan/fence.hpp"
#include "../crude_engine/graphics/vulkan/semaphore.hpp"

#include <algorithm>
#include <set>
#include <memory>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <limits>
#include <fstream>

class Test_Application
{
public:
  struct Queue_Family_Indices
  {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const
    {
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };
public:
  void run(
#ifdef _WIN32 
    HINSTANCE  hinstance
#endif
  )
  {
    initWindow(
#ifdef _WIN32 
      hinstance
#endif
    );
    initVulkan();
    mainLoop();
    cleanup();
  }
private:
  void initWindow(
#ifdef _WIN32 
    HINSTANCE  hinstance
#endif
  )
  {
#ifdef __linux__ 
    createXCBWindow();
#elif _WIN32
    m_hinstance = hinstance;
    createWin32Window();
#endif
  }

#ifdef __linux__ 
  // UNTESTED
  void createXCBWindow()
  {
    m_pConnection = xcb_connect(NULL, NULL);

    const xcb_setup_t* pSetup = xcb_get_setup(m_pConnection);
    xcb_screen_iterator_t  iter = xcb_setup_roots_iterator(pSetup);
    xcb_screen_t* pScreen = iter.data;

    m_window = xcb_generate_id(m_pConnection);
    xcb_create_window(m_pConnection,
      XCB_COPY_FROM_PARENT,
      m_window,
      pScreen->root,
      0u, 0u,
      m_width, m_height,
      10u,
      XCB_WINDOW_CLASS_INPUT_OUTPUT,
      pScreen->root_visual,
      0, NULL);
    xcb_map_window(m_pConnection, m_window);
    xcb_flush(m_pConnection);
  };
#elif _WIN32
  void createWin32Window()
  {
    WNDCLASSEX wc{};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WindowProc; // !TODO
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_windowClassWide.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);

    if (!RegisterClassEx(&wc))
    {
      throw std::runtime_error("Failed to register window class!");
    }

    m_hwnd = CreateWindowEx(
      NULL,
      m_windowClassWide.c_str(),
      m_titileWide.c_str(),
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      m_width, m_height,
      NULL,
      NULL,
      m_hinstance,
      NULL);

    if (m_hwnd == NULL)
    {
      throw std::runtime_error("failed to create win32 window");
    }
  }

  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    switch (uMsg) 
    {
    case WM_DESTROY:
      DestroyWindow(hwnd);
      PostQuitMessage(0);
      break;
    case WM_PAINT:
      ValidateRect(hwnd, NULL);
      break;
    default:
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
      break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
#endif

  void initVulkan()
  {
#ifdef __linux__ 
    const auto& surfaceExtensions = crude_engine::XCB_Surface::requiredExtensions();
#elif _WIN32
    const auto& surfaceExtensions = crude_engine::Win32_Surface::requiredExtensions();
#endif
    const auto& debugUtilsExtensions = crude_engine::Debug_Utils_Messenger::requiredExtensions();

    std::vector<const char*> enabledExtensions;
    enabledExtensions.reserve(surfaceExtensions.size() + debugUtilsExtensions.size());
    enabledExtensions.insert(enabledExtensions.end(), surfaceExtensions.begin(), surfaceExtensions.end());
    enabledExtensions.insert(enabledExtensions.end(), debugUtilsExtensions.begin(), debugUtilsExtensions.end());

    // Initialize instance
    crude_engine::Instance_Create_Info instanceInfo(debugCallback);
    instanceInfo.enabledExtensions = enabledExtensions;
    instanceInfo.enabledLayers = { "VK_LAYER_KHRONOS_validation" };

    m_instance = std::make_shared<crude_engine::Instance>(instanceInfo);

    // Initialize debugCallback
    m_debugUtilsMessenger = std::make_shared<crude_engine::Debug_Utils_Messenger>(crude_engine::DebugUtilsMessengerCreateInfo(
      m_instance,
      debugCallback));

    // Initialize surface
#ifdef __linux__ 
    m_surface = std::make_shared<crude_engine::XCB_Surface>(crude_engine::XCBSurfaceCreateInfo(
      m_instance,
      m_pConnection,
      m_window));
#elif _WIN32
    m_surface = std::make_shared<crude_engine::Win32_Surface>(crude_engine::Win32_Surface_Create_Info(
      m_instance,
      m_hinstance,
      m_hwnd));
#endif

    // Pick physical device
    for (auto& physicalDevice : m_instance->getPhysicalDevices())
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
    
    m_device = std::make_shared<crude_engine::Device>(crude_engine::Device_Create_Info(
      m_physicalDevice,
      {
        crude_engine::Device_Queue_Create_Info(queueIndices.graphicsFamily.value()),
        crude_engine::Device_Queue_Create_Info(queueIndices.presentFamily.value()) },
      deviceFeatures,
      { VK_KHR_SWAPCHAIN_EXTENSION_NAME },
      { "VK_LAYER_KHRONOS_validation" }));

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

    std::vector<uint32_t> queueFamilyIndices = { queueIndices.graphicsFamily.value(), queueIndices.presentFamily.value() };

    m_swapchain = std::make_shared<crude_engine::Swap_Chain>(crude_engine::Swap_Chain_Create_Info(
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
      nullptr));

    m_swapchainImages = m_swapchain->getSwapchainImages();
    m_swapchainImagesViews.resize(m_swapchainImages.size());
    for (uint32_t i = 0; i < m_swapchainImages.size(); ++i)
    {
      m_swapchainImagesViews[i] = std::make_shared<crude_engine::Image_View>(crude_engine::Image_View_Create_Info(
        m_device,
        m_swapchainImages[i],
        surfaceFormat.format,
        crude_engine::Image_Subresource_Range(m_swapchainImages[i])));
    }

    crude_engine::Attachment_Description colorAttachment(
      surfaceFormat.format,
      VK_SAMPLE_COUNT_1_BIT,
      VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE,
      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    VkAttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment = 0u;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    crude_engine::Attachment_Description depthAttachment(
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

    crude_engine::Subpass_Description subpass(
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      {},
      { colorAttachmentRef },
      depthAttachmentRef,
      {});

    crude_engine::Subpass_Dependency subpassDependency(
      VK_SUBPASS_EXTERNAL,
      0u,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      0u,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      0u);

    m_renderPass = std::make_shared<crude_engine::Render_Pass>(crude_engine::Render_Pass_Create_Info(
      m_device,
      { subpass },
      { subpassDependency },
      { colorAttachment, depthAttachment }));

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

    m_descriptorSetLayout = std::make_shared<crude_engine::Descriptor_Set_Layout>(crude_engine::Descriptor_Set_Layout_Create_Info(
      m_device,
      { /* uboLayoutBinding, */ samplerLayoutBinding}));

    std::string vertShaderPathA;
    std::string fragShaderPathA;
#ifdef __linux__ 
    vertShaderPathA = "test/shader.vert.spv";
    fragShaderPathA = "test/shader.frag.spv";
#elif _WIN32
    vertShaderPathA = "../../../test/shader.vert.spv";
    fragShaderPathA = "../../../test/shader.frag.spv";
#endif

    const auto vertShaderCode = readFile(vertShaderPathA);
    const auto fragShaderCode = readFile(fragShaderPathA);
    auto vertShaderModule = std::make_shared<crude_engine::Shader_Module>(crude_engine::Shader_Module_Create_Info(
      m_device,
      vertShaderCode));
    auto fragShaderModule = std::make_shared<crude_engine::Shader_Module>(crude_engine::Shader_Module_Create_Info(
      m_device,
      fragShaderCode));

    std::vector<crude_engine::Shader_Stage_Create_Info> shaderStagesInfo =
    {
      crude_engine::Shader_Stage_Create_Info(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule, "main"),
      crude_engine::Shader_Stage_Create_Info(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule, "main"),
    };

    std::vector<VkVertexInputBindingDescription> bindings = {};
    std::vector<VkVertexInputAttributeDescription> attributes = {};
    auto vertexInputStateInfo = crude_engine::Vertex_Input_State_Create_Info(bindings, attributes);
    auto inputAssembly = crude_engine::Input_Assembly_State_Create_Info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
    auto viewportState = crude_engine::Viewport_State_Create_Info(1u, 1u);
    auto rasterizer = crude_engine::Rasterization_State_Create_Info(
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
    auto multisampling = crude_engine::Multisample_State_Create_Info(
      VK_SAMPLE_COUNT_1_BIT,
      VK_FALSE,
      1.0f,
      VK_FALSE,
      VK_FALSE);
    auto depthStencil = crude_engine::Depth_Stencil_State_Create_Info(
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

    auto colorBlending = crude_engine::Color_Blend_State_Create_Info(
      { colorBlendAttachment },
      { 0.f, 0.f, 0.f, 0.f },
      VK_FALSE,
      VK_LOGIC_OP_COPY);

    auto dynamicState = crude_engine::Dynamic_State_Create_Info({ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR });

    m_pipelineLayout = std::make_shared<crude_engine::Pipeline_Layout>(crude_engine::Pipeline_Layout_Create_Info(
      m_device, 
      { m_descriptorSetLayout }, 
      {}));

    m_graphicsPipeline = std::make_shared<crude_engine::Pipeline>(crude_engine::Graphics_Pipeline_Create_Info(
      m_device,
      m_renderPass,
      m_pipelineLayout,
      nullptr,
      shaderStagesInfo,
      vertexInputStateInfo,
      crude_engine::Tessellation_State_Create_Info(3u),
      inputAssembly,
      viewportState,
      rasterizer,
      multisampling,
      depthStencil,
      colorBlending,
      dynamicState,
      0u));

    m_commandPool = std::make_shared<crude_engine::Command_Pool>(crude_engine::Command_Pool_Create_Info(
      m_device, 
      VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      queueIndices.graphicsFamily.value()));

    createDepthImage();

    m_swapchainFramebuffers.resize(m_swapchainImagesViews.size());
    for (uint32_t i = 0; i < m_swapchainFramebuffers.size(); ++i)
    {
      std::vector<std::shared_ptr<crude_engine::Image_View>> attachments = { m_swapchainImagesViews[i], m_depthImageView };

      m_swapchainFramebuffers[i] = std::make_shared<crude_engine::Framebuffer>(crude_engine::Framebuffer_Create_Info(
        m_device,
        m_renderPass,
        attachments,
        extent.width,
        extent.height,
        1u));
    }

    createTextureImageFromFile("../../../test/test.png", m_textureImage, m_textureImageMemory);
    m_textureImageView = std::make_shared<crude_engine::Image_View>(crude_engine::Image_View_Create_Info(
      m_device,
      m_textureImage,
      m_textureImage->getFormat(),
      crude_engine::Image_Subresource_Range(m_textureImage)));

    m_sampler = std::make_shared<crude_engine::Sampler>(crude_engine::Sampler_Create_Info(
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
      VK_FALSE));

    std::vector<VkDescriptorPoolSize> poolSizes = { VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2u} };
    m_descriptorPool = std::make_shared<crude_engine::Descriptor_Pool>(crude_engine::Descriptor_Pool_Create_Info(
      m_device,
      poolSizes,
      2u));

    m_descriptorSets.resize(2u);
    for (auto& descriptorSet : m_descriptorSets)
    {
      descriptorSet = std::make_shared<crude_engine::Descriptor_Set>(crude_engine::Descriptor_Set_Create_Info(
        m_device,
        { m_descriptorSetLayout },
        m_descriptorPool));
    }

    for (uint32_t i = 0; i < 2u; i++) {
      crude_engine::Descriptor_Image_Info imageInfo(
        m_sampler,
        m_textureImageView,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

      std::vector<crude_engine::Write_Descriptor_Set> descriptorWrites =
      {
        crude_engine::Write_Descriptor_Set(
          m_descriptorSets[i],
          0u,
          0u,
          1u,
          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          imageInfo,
          std::nullopt)
      };
      m_device->updateDescriptorSets(descriptorWrites, {});
    }

    m_commandBuffers.resize(2u);
    for (auto& commandBuffer : m_commandBuffers)
    {
      commandBuffer = std::make_shared<crude_engine::Command_Buffer>(crude_engine::Command_Buffer_Create_Info(
        m_device,
        m_commandPool,
        VK_COMMAND_BUFFER_LEVEL_PRIMARY));
    }

    m_imageAvailableSemaphores.resize(2u);
    m_renderFinishedSemaphores.resize(2u);
    m_inFlightFences.resize(2u);

    for (size_t i = 0; i < 2u; i++) {
      m_imageAvailableSemaphores[i] = std::make_shared<crude_engine::Semaphore>(crude_engine::Semaphore_Create_Info(m_device));
      m_renderFinishedSemaphores[i] = std::make_shared<crude_engine::Semaphore>(crude_engine::Semaphore_Create_Info(m_device));
      m_inFlightFences[i] = std::make_shared<crude_engine::Fence>(crude_engine::Fence_Create_Info(m_device, VK_FENCE_CREATE_SIGNALED_BIT));
    }
  }

  void createDepthImage()
  {
    VkSurfaceCapabilitiesKHR surfaceCapabilites = m_physicalDevice->getSurfaceCapabilitis(m_surface);
    const VkExtent2D extent = chooseSwapExtent(surfaceCapabilites);

    const VkFormat depthFormat = findDepthFormat();
    m_depthImage = std::make_shared<crude_engine::Image>(crude_engine::Image_2D_Create_Info(
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
      VK_IMAGE_LAYOUT_UNDEFINED));


    VkMemoryRequirements memRequirements = m_depthImage->getMemoryRequirements();

    m_depthImageDeviceMemory = std::make_shared<crude_engine::Device_Memory>(crude_engine::Device_Memory_Allocate_Info(
      m_device,
      memRequirements.size,
      memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));

    m_depthImageDeviceMemory->bind(*m_depthImage);

    auto commandBuffer = std::make_shared<crude_engine::Command_Buffer>(crude_engine::Command_Buffer_Create_Info(
      m_device,
      m_commandPool,
      VK_COMMAND_BUFFER_LEVEL_PRIMARY));
    commandBuffer->begin();

    crude_engine::Image_Memory_Barrier barrier(
      m_depthImage,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      crude_engine::Image_Subresource_Range(m_depthImage, 0u, 1u, 0u, 1u));
    commandBuffer->barrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, { barrier });
    commandBuffer->end();
    m_graphicsQueue->sumbit({ commandBuffer });
    m_graphicsQueue->waitIdle();

    m_depthImageView = std::make_shared<crude_engine::Image_View>(crude_engine::Image_View_Create_Info(
      m_device,
      m_depthImage,
      depthFormat,
      crude_engine::Image_Subresource_Range(m_depthImage)));
  }

  void createTextureImageFromFile(const char* path, std::shared_ptr<crude_engine::Image>& textureImage, std::shared_ptr<crude_engine::Device_Memory>& textureImageMemory)
  {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    size_t pixelsbsize = 4u * texWidth * texHeight;

    if (!pixels)
    {
      throw std::runtime_error("failed to load texture image!");
    }

    auto stagingBuffer = std::make_shared<crude_engine::Buffer>(crude_engine::Buffer_Create_Info(
      m_device,
      4 * texWidth * texHeight,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_SHARING_MODE_EXCLUSIVE,
      {}));

    VkMemoryRequirements memRequirements = m_depthImage->getMemoryRequirements();

    auto stagingBufferMemory = std::make_shared<crude_engine::Device_Memory>(crude_engine::Device_Memory_Allocate_Info(
      m_device,
      memRequirements.size,
      memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

    stagingBufferMemory->bind(*stagingBuffer);

    void* data = stagingBufferMemory->map().value();
    memcpy(data, pixels, static_cast<size_t>(pixelsbsize));
    stagingBufferMemory->unmap();

    textureImage = std::make_shared<crude_engine::Image>(crude_engine::Image_2D_Create_Info(
      m_device,
      0u,
      VK_FORMAT_R8G8B8A8_SRGB,
      VkExtent2D { static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) },
      1u,
      1u,
      VK_SAMPLE_COUNT_1_BIT,
      VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_SHARING_MODE_EXCLUSIVE,
      VK_IMAGE_LAYOUT_UNDEFINED));

    memRequirements = textureImage->getMemoryRequirements();

    textureImageMemory = std::make_shared<crude_engine::Device_Memory>(crude_engine::Device_Memory_Allocate_Info(
      m_device,
      memRequirements.size,
      memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));

    textureImageMemory->bind(*textureImage);

    auto commandBuffer = std::make_shared<crude_engine::Command_Buffer>(crude_engine::Command_Buffer_Create_Info(
      m_device,
      m_commandPool,
      VK_COMMAND_BUFFER_LEVEL_PRIMARY));
    commandBuffer->begin();

    commandBuffer->barrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {
      crude_engine::Image_Memory_Barrier(
        textureImage,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        crude_engine::Image_Subresource_Range(textureImage, 0u, 1u, 0u, 1u))
      });
     
    VkBufferImageCopy region{};
    region.bufferOffset                    = 0;
    region.bufferRowLength                 = 0;
    region.bufferImageHeight               = 0;
    region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel       = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount     = 1;
    region.imageOffset                     = {0, 0, 0};
    region.imageExtent = {
      static_cast<uint32_t>(texWidth),
      static_cast<uint32_t>(texHeight),
      1
    };
    commandBuffer->copyBufferToImage(stagingBuffer, textureImage, { region });

    commandBuffer->barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, {
      crude_engine::Image_Memory_Barrier(
        textureImage,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        crude_engine::Image_Subresource_Range(textureImage, 0u, 1u, 0u, 1u))
      });

    commandBuffer->end();

    m_graphicsQueue->sumbit({ commandBuffer });
    m_graphicsQueue->waitIdle();
  }

  void recreateSwapChain()
  {

  }

  static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
      throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
  }

  Queue_Family_Indices findQueueFamilies(crude_engine::Physical_Device& physicalDevice) {
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

  bool checkDeviceExtensionSupport(crude_engine::Physical_Device& physicalDevice) 
  {
    static const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    
    const auto& availableExtensions = physicalDevice.getExtensionProperties();
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }

  bool checkSurfaceSupport(crude_engine::Physical_Device& physicalDevice, std::shared_ptr<const crude_engine::Surface> surface)
  {
    return !physicalDevice.getSurfaceFormats(surface).empty() && !physicalDevice.getSurfacePresentModes(surface).empty();
  }

  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
      }
    }

    return availableFormats[0];
  }

  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
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

  VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
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
#ifdef __linux__ 
#elif _WIN32
    ShowWindow(m_hwnd, SW_SHOW);
    MSG message;

    while (GetMessage(&message, NULL, 0, 0)) {
      TranslateMessage(&message);
      DispatchMessage(&message);
      drawFrame();
    }
#endif

    m_device->waitIdle();
  }

  void drawFrame()
  {
    m_inFlightFences[m_currentFrame]->wait();
    const crude_engine::Swap_Chain_Next_Image acquireNextImageResult = m_swapchain->acquireNextImage(m_imageAvailableSemaphores[m_currentFrame]);

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

    const bool graphicsQueueSubmited = m_graphicsQueue->sumbit(
      { m_commandBuffers[m_currentFrame] },
      { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT },
      { m_imageAvailableSemaphores[m_currentFrame] },
      { m_renderFinishedSemaphores[m_currentFrame] },
      m_inFlightFences[m_currentFrame]);

    if (!graphicsQueueSubmited)
    {
      throw std::runtime_error("failed to submit draw command buffer!");
    }

    const crude_engine::Queue_Present_Result presentResult = m_presentQueue->present({ m_swapchain }, { imageIndex }, { m_renderFinishedSemaphores[m_currentFrame] });

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

  void recordCommandBuffer(std::shared_ptr<crude_engine::Command_Buffer> commandBuffer, uint32_t imageIndex) {
    if (!commandBuffer->begin())
    {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    std::vector<VkClearValue> clearValues(2u);
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRect2D renderArea;
    renderArea.extent = m_swapchain->getExtent();
    renderArea.offset.x = 0.f;
    renderArea.offset.y = 0.f;

    commandBuffer->beginRenderPass(
      m_renderPass,
      m_swapchainFramebuffers[imageIndex],
      clearValues,
      renderArea);

    commandBuffer->bindPipeline(m_graphicsPipeline);

    VkViewport viewport{};
    viewport.x         = 0.0f;
    viewport.y         = 0.0f;
    viewport.width     = static_cast<float>(m_swapchain->getExtent().width);
    viewport.height    = static_cast<float>(m_swapchain->getExtent().height);
    viewport.minDepth  = 0.0f;
    viewport.maxDepth  = 1.0f;
    commandBuffer->setViewport({ viewport });

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = m_swapchain->getExtent();
    commandBuffer->setScissor({ scissor });

    commandBuffer->bindDescriptorSets(m_graphicsPipeline, { m_descriptorSets[m_currentFrame] });

    commandBuffer->draw(3u, 1u);
    
    commandBuffer->endRenderPass();

    if (!commandBuffer->end())
    {
      throw std::runtime_error("failed to record command buffer!");
    }
  }

  void cleanup() 
  {
#ifdef __linux__ 
    xcb_disconnect(m_pConnection);
#elif _WIN32
    UnregisterClass(m_windowClassWide.c_str(), m_hinstance);
    DestroyWindow(m_hwnd);
#endif
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
#ifdef __linux__
  xcb_connection_t*  m_pConnection;
  xcb_window_t       m_window;
#elif _WIN32
  HWND       m_hwnd;
  HINSTANCE  m_hinstance;
#endif
  std::shared_ptr<crude_engine::Instance>                       m_instance;
  std::shared_ptr<crude_engine::Debug_Utils_Messenger>          m_debugUtilsMessenger;
  std::shared_ptr<crude_engine::Surface>                        m_surface;
  std::shared_ptr<crude_engine::Physical_Device>                m_physicalDevice;
  std::shared_ptr<crude_engine::Device>                         m_device;
  std::shared_ptr<crude_engine::Queue>                          m_graphicsQueue;
  std::shared_ptr<crude_engine::Queue>                          m_presentQueue;
  std::shared_ptr<crude_engine::Swap_Chain>                     m_swapchain;
  std::vector<std::shared_ptr<crude_engine::Swap_Chain_Image>>  m_swapchainImages;
  std::vector<std::shared_ptr<crude_engine::Image_View>>        m_swapchainImagesViews;
  std::shared_ptr<crude_engine::Render_Pass>                    m_renderPass;
  std::shared_ptr<crude_engine::Descriptor_Set_Layout>          m_descriptorSetLayout;
  std::shared_ptr<crude_engine::Pipeline_Layout>                m_pipelineLayout;
  std::shared_ptr<crude_engine::Pipeline>                       m_graphicsPipeline;
  std::shared_ptr<crude_engine::Command_Pool>                   m_commandPool;
  std::shared_ptr<crude_engine::Device_Memory>                  m_depthImageDeviceMemory;
  std::shared_ptr<crude_engine::Image>                          m_depthImage;
  std::shared_ptr<crude_engine::Image_View>                     m_depthImageView;
  std::vector<std::shared_ptr<crude_engine::Framebuffer>>       m_swapchainFramebuffers;
  std::shared_ptr<crude_engine::Image>                          m_textureImage;
  std::shared_ptr<crude_engine::Device_Memory>                  m_textureImageMemory;
  std::shared_ptr<crude_engine::Image_View>                     m_textureImageView;
  std::shared_ptr<crude_engine::Sampler>                        m_sampler;
  std::shared_ptr<crude_engine::Descriptor_Pool>                m_descriptorPool;
  std::vector<std::shared_ptr<crude_engine::Descriptor_Set>>    m_descriptorSets;
  std::vector<std::shared_ptr<crude_engine::Command_Buffer>>    m_commandBuffers;
  std::vector<std::shared_ptr<crude_engine::Semaphore>>         m_imageAvailableSemaphores;
  std::vector<std::shared_ptr<crude_engine::Semaphore>>         m_renderFinishedSemaphores;
  std::vector<std::shared_ptr<crude_engine::Fence>>             m_inFlightFences;
  uint32_t m_currentFrame = 0u;
  uint32_t m_width = 800u;
  uint32_t m_height = 600u;
  bool m_framebufferResized = false;

  const std::string   m_titile = "test";
  const std::wstring  m_titileWide = L"test";
  const std::wstring  m_windowClassWide = L"test_class";
};

#ifdef __linux__
int main()
{
  Test_Application testApp;

  try 
  {
    testApp.run();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}
#elif _WIN32
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
  }

  system("pause");

  return EXIT_SUCCESS;
}
#endif