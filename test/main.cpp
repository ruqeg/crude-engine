#include <vulkan/vulkan_core.h>
#ifdef __linux__ 
#define VK_USE_PLATFORM_XCB_KHR
#include <xcb/xcb.h>
#elif _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#endif

#include "../include/crude_vulkan_01/instance.hpp"
#include "../include/crude_vulkan_01/debug_utils_messenger.hpp"
#include "../include/crude_vulkan_01/surface.hpp"
#include "../include/crude_vulkan_01/physical_device.hpp"
#include "../include/crude_vulkan_01/device.hpp"
#include "../include/crude_vulkan_01/queue.hpp"
#include "../include/crude_vulkan_01/swap_chain.hpp"
#include "../include/crude_vulkan_01/image_view.hpp"
#include "../include/crude_vulkan_01/swap_chain_image.hpp"
#include "../include/crude_vulkan_01/render_pass.hpp"
#include "../include/crude_vulkan_01/descriptor_set_layout.hpp"
#include "../include/crude_vulkan_01/shader_module.hpp"
#include "../include/crude_vulkan_01/vertex_input_state_create_info.hpp"
#include "../include/crude_vulkan_01/input_assembly_state_create_info.hpp"
#include "../include/crude_vulkan_01/viewport_state_create_info.hpp"

#include <algorithm>
#include <set>
#include <memory>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <limits>
#include <fstream>

struct Vertex {
  float pos[3];
  float color[3];
  float texcoord[2];

  static const VkVertexInputBindingDescription& getBindingDescription() {
    static VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding   = 0u;
    bindingDescription.stride    = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
  }

  static const std::array<VkVertexInputAttributeDescription, 3>& getAttributeDescriptions() {
    static std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
    attributeDescriptions[0].binding  = 0u;
    attributeDescriptions[0].location = 0u;
    attributeDescriptions[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset   = offsetof(Vertex, pos);

    attributeDescriptions[1].binding  = 0u;
    attributeDescriptions[1].location = 1u;
    attributeDescriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset   = offsetof(Vertex, color);

    attributeDescriptions[2].binding  = 0u;
    attributeDescriptions[2].location = 2u;
    attributeDescriptions[2].format   = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset   = offsetof(Vertex, texcoord);

    return attributeDescriptions;
  }
};


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
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
#endif

  void initVulkan() 
  {
#ifdef __linux__ 
    const auto& surfaceExtensions = crude_vulkan_01::XCB_Surface::requiredExtensions();
#elif _WIN32
    const auto& surfaceExtensions = crude_vulkan_01::Win32_Surface::requiredExtensions();
#endif
    const auto& debugUtilsExtensions = crude_vulkan_01::Debug_Utils_Messenger::requiredExtensions();

    std::vector<const char*> enabledExtensions;
    enabledExtensions.reserve(surfaceExtensions.size() + debugUtilsExtensions.size());
    enabledExtensions.insert(enabledExtensions.end(), surfaceExtensions.begin(), surfaceExtensions.end());
    enabledExtensions.insert(enabledExtensions.end(), debugUtilsExtensions.begin(), debugUtilsExtensions.end());

    // Initialize instance
    crude_vulkan_01::InstanceCreateInfo instanceInfo(debugCallback);
    instanceInfo.enabledExtensions  = enabledExtensions;
    instanceInfo.enabledLayers      = {"VK_LAYER_KHRONOS_validation"};

    m_instance = std::make_shared<crude_vulkan_01::Instance>(instanceInfo);
    
    // Initialize debugCallback
    m_debugUtilsMessenger = std::make_shared<crude_vulkan_01::Debug_Utils_Messenger>(crude_vulkan_01::DebugUtilsMessengerCreateInfo(
      m_instance,
      debugCallback));

    // Initialize surface
#ifdef __linux__ 
    m_surface = std::make_shared<crude_vulkan_01::XCB_Surface>(crude_vulkan_01::XCBSurfaceCreateInfo(
      m_instance,
      m_pConnection,
      m_window));
#elif _WIN32
    m_surface = std::make_shared<crude_vulkan_01::Win32_Surface>(crude_vulkan_01::Win32SurfaceCreateInfo(
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
    m_device = std::make_shared<crude_vulkan_01::Device>(crude_vulkan_01::DeviceCreateInfo(
      m_physicalDevice,
      {
        crude_vulkan_01::DeviceQueueCreateInfo(queueIndices.graphicsFamily.value()), 
        crude_vulkan_01::DeviceQueueCreateInfo(queueIndices.presentFamily.value())},
      {},
      {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
      {"VK_LAYER_KHRONOS_validation"}));

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

    std::vector<uint32_t> queueFamilyIndices = {queueIndices.graphicsFamily.value(), queueIndices.presentFamily.value()};

    m_swapchain = std::make_shared<crude_vulkan_01::Swap_Chain>(crude_vulkan_01::SwapChainCreateInfo(
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
      m_swapchainImagesViews[i] = std::make_shared<crude_vulkan_01::Image_View>(crude_vulkan_01::ImageViewCreateInfo(
        m_device,
        m_swapchainImages[i],
        surfaceFormat.format,
        {},
        VK_IMAGE_ASPECT_COLOR_BIT,
        0u,
        0u,
        1u,
        1u));
    }

    crude_vulkan_01::AttachmentDescription colorAttachment(
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
    colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    crude_vulkan_01::AttachmentDescription depthAttachment(
      findDepthFormat(),
      VK_SAMPLE_COUNT_1_BIT,
      VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE,
      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    VkAttachmentReference depthAttachmentRef;
    depthAttachmentRef.attachment  = 1u;
    depthAttachmentRef.layout      = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    crude_vulkan_01::SubpassDescription subpass(
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      {},
      {colorAttachmentRef},
      depthAttachmentRef,
      {});

    crude_vulkan_01::SubpassDependency subpassDependency(
      VK_SUBPASS_EXTERNAL,
      0u,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      0u,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      0u);

    m_renderPass = std::make_shared<crude_vulkan_01::Render_Pass>(crude_vulkan_01::RenderPassCreateInfo(
      m_device,
      {subpass},
      {subpassDependency},
      {colorAttachment, depthAttachment}));

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.binding          = 0;
    uboLayoutBinding.descriptorCount  = 1;
    uboLayoutBinding.stageFlags       = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.descriptorType    = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.binding           = 1;
    samplerLayoutBinding.descriptorCount   = 1;
    samplerLayoutBinding.stageFlags        = VK_SHADER_STAGE_FRAGMENT_BIT;

    m_descriptorSetLayout = std::make_shared<crude_vulkan_01::Descriptor_Set_Layout>(crude_vulkan_01::DescriptorSetLayoutCreateInfo(
      m_device,
      {uboLayoutBinding, samplerLayoutBinding}));

    const auto vertShaderCode = readFile("test/shader.vert.spv");
    const auto fragShaderCode = readFile("test/shader.frag.spv");
    auto vertShaderModule = std::make_shared<crude_vulkan_01::Shader_Module>(crude_vulkan_01::Shader_Module_Create_Info(
      m_device,
      vertShaderCode));
    auto fragShaderModule = std::make_shared<crude_vulkan_01::Shader_Module>(crude_vulkan_01::Shader_Module_Create_Info(
      m_device,
      fragShaderCode));

    std::vector<VkVertexInputBindingDescription> bindings = {Vertex::getBindingDescription()};
    std::vector<VkVertexInputAttributeDescription> attributes(Vertex::getAttributeDescriptions().begin(), Vertex::getAttributeDescriptions().end());
    auto vertexInputStateInfo = crude_vulkan_01::Vertex_Input_State_Create_Info(bindings, attributes);
    auto inputAssembly = crude_vulkan_01::Input_Assembly_State_Create_Info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
    auto viewportState = crude_vulkan_01::Viewport_State_Create_Info(1u, 1u);
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

  Queue_Family_Indices findQueueFamilies(crude_vulkan_01::Physical_Device& physicalDevice) {
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

  bool checkDeviceExtensionSupport(crude_vulkan_01::Physical_Device& physicalDevice) 
  {
    static const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    
    const auto& availableExtensions = physicalDevice.getExtensionProperties();
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }

  bool checkSurfaceSupport(crude_vulkan_01::Physical_Device& physicalDevice, std::shared_ptr<const crude_vulkan_01::Surface> surface)
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
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
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
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    }
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
  std::shared_ptr<crude_vulkan_01::Instance>                       m_instance;
  std::shared_ptr<crude_vulkan_01::Debug_Utils_Messenger>          m_debugUtilsMessenger;
  std::shared_ptr<crude_vulkan_01::Surface>                        m_surface;
  std::shared_ptr<crude_vulkan_01::Physical_Device>                m_physicalDevice;
  std::shared_ptr<crude_vulkan_01::Device>                         m_device;
  std::shared_ptr<crude_vulkan_01::Queue>                          m_graphicsQueue;
  std::shared_ptr<crude_vulkan_01::Queue>                          m_presentQueue;
  std::shared_ptr<crude_vulkan_01::Swap_Chain>                     m_swapchain;
  std::vector<std::shared_ptr<crude_vulkan_01::Swap_Chain_Image>>  m_swapchainImages;
  std::vector<std::shared_ptr<crude_vulkan_01::Image_View>>        m_swapchainImagesViews;
  std::shared_ptr<crude_vulkan_01::Render_Pass>                    m_renderPass;
  std::shared_ptr<crude_vulkan_01::Descriptor_Set_Layout>          m_descriptorSetLayout;
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
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  system("pause");

  return 0;
}
#endif
