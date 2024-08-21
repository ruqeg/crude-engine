#include <DirectXMath.h>
#include <flecs.h>
#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

module crude.engine;

import crude.core.logger;
import crude.core.memory;
import crude.scene.free_camera_script;
import crude.scene.window_script;
import crude.scene.camera;
import crude.platform.sdl_helper;
import crude.platform.sdl_window_container;
import crude.platform.input_system;
import crude.resources.gltf_loader;
import crude.graphics.renderer_core_component;
import crude.graphics.deferred_gbuffer_pass_system;
import crude.graphics.fullscreen_pbr_pass_system;
import crude.graphics.renderer_frame_system;
import crude.graphics.device;
import crude.graphics.physical_device;
import crude.graphics.swap_chain;
import crude.graphics.swap_chain_image;
import crude.graphics.image_attachment;
import crude.graphics.queue;
import crude.graphics.surface;
import crude.graphics.instance;
import crude.graphics.render_pass;
import crude.graphics.subpass_dependency;
import crude.graphics.descriptor_pool;
import crude.graphics.format_helper;
import crude.graphics.framebuffer;
import crude.graphics.command_buffer;
import crude.network.network_system;
import crude.graphics.vulkan_utils;


namespace crude
{

void Engine::preinitialize(core::uint32 defaultFreeRBTCapacity)
{
  core::initializeMemory(defaultFreeRBTCapacity);
  platform::initializeSDL();
  platform::loadSDLVulkan();
}

void Engine::postdeinitialize()
{
  platform::deinitializeSDL();
  platform::unloadSDLVulkan();
}

void Engine::initialize(const char* title, core::uint32 width, core::uint32 height)
{
  initializeSystems();

  auto windowContainer = crude::core::allocateShared<crude::platform::SDL_Window_Container>(
    title, width, height, crude::platform::SDL_WINDOW_CONTAINER_FLAG_VULKAN);

  graphics::Renderer_Core_Component rendererCoreComponent(windowContainer);
  m_world.set<graphics::Renderer_Core_Component>(rendererCoreComponent);
  m_world.set<graphics::Deferred_GBuffer_Pass_Component>({ 
    rendererCoreComponent.device, 
    rendererCoreComponent.swapchain->getExtent(), 
    static_cast<core::uint32>(rendererCoreComponent.swapchainImagesViews.size()) });

  m_world.set<graphics::Fullscreen_PBR_Pass_Component>({
    m_world.get_mut<graphics::Deferred_GBuffer_Pass_Component>()->gbuffer,
    rendererCoreComponent.swapchain });

  m_world.set<graphics::Renderer_Frame_Component>({ rendererCoreComponent.device, rendererCoreComponent.graphicsCommandPool });

  resources::GLTF_Loader gltfLoader(m_world.get_mut<graphics::Renderer_Core_Component>()->transferCommandPool, m_world);
  m_sceneNode = gltfLoader.loadNodeFromFile("../../crude_example/basic_triangle_examle/resources/sponza.glb");

  flecs::entity cameraNode = m_world.entity("camera node");
  cameraNode.set<scene::Camera>([windowContainer](){
    scene::Camera camera;
    camera.calculateViewToClipMatrix(DirectX::XM_PIDIV4, windowContainer->getAspect(), 0.05f, 100.0f);
    return camera;
  }());
  cameraNode.set<scene::Transform>([&cameraNode]() {
    scene::Transform transform(cameraNode);
    transform.setTranslation(0.0, 0.0, -2.0);
    return transform;
  }());
  cameraNode.set<scene::script::Free_Camera_Component>(scene::script::Free_Camera_Component());
  cameraNode.child_of(m_sceneNode);
  m_world.get_mut<graphics::Renderer_Frame_Component>()->cameraNode = cameraNode;
  
  m_timer.setFrameRate(60);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForVulkan(windowContainer->getHandle().get());

  constexpr bool clearEnable = false;

  core::array<graphics::Subpass_Description, 1> subpassDescriptions =
  { 
    graphics::Subpass_Description(graphics::Subpass_Description::Initialize_Color{VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL})
  };

  core::vector<graphics::Attachment_Description> attachmentsDescriptions =
  {
    graphics::Attachment_Description({
      .format        = rendererCoreComponent.swapchain->getSurfaceFormat().format,
      .samples       = VK_SAMPLE_COUNT_1_BIT,
      .colorOp       = clearEnable ? graphics::attachment_op::gClearStore : graphics::attachment_op::gDontCareStore,
      .stenicilOp    = graphics::attachment_op::gDontCare,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR })
  };

  core::array<graphics::Subpass_Dependency, 1u> subpassesDependencies =
  {
    graphics::Subpass_Dependency({
      .srcSubpass      = VK_SUBPASS_EXTERNAL,
      .dstSubpass      = 0,
      .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .srcAccessMask   = 0u,
      .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0})
  };

  m_renderPass = core::allocateShared<graphics::Render_Pass>(rendererCoreComponent.device, subpassDescriptions, subpassesDependencies, attachmentsDescriptions);

  core::array<graphics::Descriptor_Pool_Size, 1> poolSizes =
  {
      graphics::Descriptor_Pool_Size(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u)
  };

  m_descriptorPool = core::allocateShared<graphics::Descriptor_Pool>(rendererCoreComponent.device, poolSizes);

  ImGui_ImplVulkan_InitInfo init_info{
    .Instance        = rendererCoreComponent.instance->getHandle(),
    .PhysicalDevice  = rendererCoreComponent.device->getPhysicalDevice()->getHandle(),
    .Device          = rendererCoreComponent.device->getHandle(),
    .QueueFamily     = rendererCoreComponent.graphicsQueue->getFamilyIndex(),
    .Queue           = rendererCoreComponent.graphicsQueue->getHandle(),
    .DescriptorPool  = m_descriptorPool->getHandle(),
    .RenderPass      = m_renderPass->getHandle(),
    .MinImageCount   = 3,
    .ImageCount      = 3,
    .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
    .PipelineCache   = VK_NULL_HANDLE,
    .Subpass         = 0,
    .Allocator       = nullptr,
    .CheckVkResultFn = [](VkResult err) {
      graphics::vulkanHandleResult(err, "ImGUI vulkan error!");
    }
  };
  ImGui_ImplVulkan_Init(&init_info);


  m_framebuffers.reserve(rendererCoreComponent.swapchainImages.size());
  for (core::uint32 i = 0; i < rendererCoreComponent.swapchainImages.size(); ++i)
  {
    m_framebuffers.push_back(core::allocateShared<graphics::Framebuffer>(rendererCoreComponent.device, m_renderPass, core::vector{ rendererCoreComponent.swapchainImagesViews[i] }, rendererCoreComponent.swapchainImages[i]->getExtent2D(), 1u));
  }
}

void Engine::mainLoop()
{
  const auto windowComponent = m_world.get<scene::script::Window_Component>();
  while (!windowComponent->shouldClose)
  {
    m_inputSystem.run();

    core::float64 elapsed;
    if (m_timer.frameElasped(elapsed))
    {
      core::logInfo(core::Debug::Channel::All, "fps: %i\n", (int)(1.0 / elapsed));
      m_freeCameraUpdateSystem.run(elapsed);
      m_rendererFrameStartSystem.run();
      m_deferredGBufferPassSystem.run();
      m_fullscreenPBRPassSystem.run();



      // Start the Dear ImGui frame
      ImGui_ImplVulkan_NewFrame();
      ImGui_ImplSDL3_NewFrame();
      ImGui::NewFrame();

      // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).

      bool show_demo_window = true;
      bool show_another_window = true;
      ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
      if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

      // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
      {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
          counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
      }

      // 3. Show another simple window.
      if (show_another_window)
      {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
          show_another_window = false;
        ImGui::End();
      }

      // Rendering
      ImGui::Render();
      ImDrawData* draw_data = ImGui::GetDrawData();
      const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
      if (!is_minimized)
      {
        graphics::Renderer_Core_Component* coreComponent = m_world.get_mut<graphics::Renderer_Core_Component>();
        graphics::Renderer_Frame_Component* frameComponent = m_world.get_mut<graphics::Renderer_Frame_Component>();

        core::array<VkClearValue, 2u> clearValues;;
        clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        VkRect2D renderArea;
        renderArea.extent = coreComponent->swapchain->getExtent();
        renderArea.offset = VkOffset2D{ 0, 0 };

        frameComponent->getFrameGraphicsCommandBuffer()->beginRenderPass(m_renderPass, m_framebuffers[frameComponent->swapchainImageIndex], clearValues, renderArea);

        ImGui_ImplVulkan_RenderDrawData(draw_data, frameComponent->getFrameGraphicsCommandBuffer()->getHandle());

        frameComponent->getFrameGraphicsCommandBuffer()->endRenderPass();
      }

      m_rendererFrameSubmitSystem.run();
    }
  }
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplSDL3_Shutdown();
}

void Engine::initializeSystems()
{
  m_world.set<scene::script::Window_Component>({});

  m_world.set<platform::Input_System_Component>(platform::Input_System_Component({ 
    m_world.system<scene::script::Free_Camera_Component>("FreeCameraUpdateEvent")
      .kind(flecs::OnUpdate)
      .run(scene::script::freeCameraUpdateEventSystemProcess),
    m_world.system("WindowUpdateEvent")
      .kind(flecs::OnUpdate)
      .run(scene::script::windowUpdateEventSystemProcess) 
  }));

  m_inputSystem = m_world.system("InputSystem")
    .kind(flecs::PreUpdate)
    .run(platform::inputSystemProcess);

  m_freeCameraUpdateSystem = m_world.system<scene::script::Free_Camera_Component, scene::Transform>("FreeCameraUpdateSystem")
    .kind(flecs::OnUpdate)
    .run(scene::script::freeCameraUpdateSystemProcess);

  m_deferredGBufferPassSystem = m_world.system<core::shared_ptr<graphics::Mesh_Buffer>, core::shared_ptr<scene::Mesh>>("DeferredGBufferPassSystem")
    .kind(flecs::PreStore)
    .run(graphics::deferredGBufferPassSystemProcess);

  m_fullscreenPBRPassSystem = m_world.system("FullscreenPBRPassSystem")
    .kind(flecs::PreStore)
    .run(graphics::fullscreenPBRPassSystemProcess);

  m_rendererFrameStartSystem = m_world.system("RendererFrameStartSystem")
    .kind(flecs::PreStore)
    .run(graphics::rendererFrameStartSystemProcess);

  m_rendererFrameSubmitSystem = m_world.system("RendererFrameSubmitSystem")
    .kind(flecs::OnStore)
    .run(graphics::rendererFrameSubmitSystemProcess);
}

}