#include <flecs.h>
#include <vulkan/vulkan.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

module crude.gui.imgui_renderer_pass_system;

import crude.graphics.device;
import crude.graphics.render_pass;
import crude.graphics.framebuffer;
import crude.graphics.descriptor_pool;
import crude.graphics.renderer_core_component;
import crude.graphics.swap_chain;
import crude.graphics.swap_chain_image;
import crude.graphics.image_view;
import crude.graphics.command_buffer;
import crude.graphics.instance;
import crude.graphics.queue;
import crude.graphics.physical_device;
import crude.graphics.vulkan_utils;
import crude.graphics.renderer_frame_system;
import crude.platform.sdl_window_container;

namespace crude::gui
{

void imguiRendererPassSystemProcess(flecs::iter& it)
{
  ImGui_Renderer_Component* imguiRendererComponent = it.world().get_mut<ImGui_Renderer_Component>();
  ImGui_Layout_Component* imguiGuiComponent = it.world().get_mut<ImGui_Layout_Component>();
  graphics::Renderer_Core_Component* rendererCoreComponent = it.world().get_mut<graphics::Renderer_Core_Component>();
  graphics::Renderer_Frame_Component* rendererFrameComponent = it.world().get_mut<graphics::Renderer_Frame_Component>(); 

  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  for (auto& drawLayoutSystem : imguiGuiComponent->drawLayoutSystems)
  {
    drawLayoutSystem.run();
  }

  ImGui::Render();
  
  ImDrawData* drawData = ImGui::GetDrawData();
  const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
  if (!isMinimized)
  {
    VkRect2D renderArea;
    renderArea.extent = rendererCoreComponent->swapchain->getExtent();
    renderArea.offset = VkOffset2D{ 0, 0 };

    rendererFrameComponent->getFrameGraphicsCommandBuffer()->beginRenderPass(imguiRendererComponent->renderPass, imguiRendererComponent->framebuffers[rendererFrameComponent->swapchainImageIndex], renderArea);

    ImGui_ImplVulkan_RenderDrawData(drawData, rendererFrameComponent->getFrameGraphicsCommandBuffer()->getHandle());

    rendererFrameComponent->getFrameGraphicsCommandBuffer()->endRenderPass();
  }
}

void imguiRendererPassSystemInitialize(flecs::iter& it)
{
  ImGui_Renderer_Component imguiRendererComponent;
  graphics::Renderer_Core_Component* rendererCoreComponent = it.world().get_mut<graphics::Renderer_Core_Component>();

  imguiRendererPassSystemInitializeRenderPass(&imguiRendererComponent, rendererCoreComponent);
  imguiRendererPassSystemInitializeDescriptorPool(&imguiRendererComponent, rendererCoreComponent);
  imguiRendererPassSystemInitializeFramebuffers(&imguiRendererComponent, rendererCoreComponent);

  ImGui_ImplSDL3_InitForVulkan(rendererCoreComponent->windowContainer->getHandle().get());

  ImGui_ImplVulkan_InitInfo init_info{
    .Instance        = rendererCoreComponent->instance->getHandle(),
    .PhysicalDevice  = rendererCoreComponent->device->getPhysicalDevice()->getHandle(),
    .Device          = rendererCoreComponent->device->getHandle(),
    .QueueFamily     = rendererCoreComponent->graphicsQueue->getFamilyIndex(),
    .Queue           = rendererCoreComponent->graphicsQueue->getHandle(),
    .DescriptorPool  = imguiRendererComponent.descriptorPool->getHandle(),
    .RenderPass      = imguiRendererComponent.renderPass->getHandle(),
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

  it.world().set<ImGui_Renderer_Component>(std::move(imguiRendererComponent));
}

void imguiRendererPassSystemDeinitialize(flecs::iter& it)
{
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplSDL3_Shutdown();
}

void imguiRendererPassSystemInitializeRenderPass(ImGui_Renderer_Component* imguiRendererComponent, graphics::Renderer_Core_Component* rendererCoreComponent)
{
  core::array<graphics::Subpass_Description, 1> subpassDescriptions =
  {
    graphics::Subpass_Description(graphics::Subpass_Description::Initialize_Color{VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL})
  };

  core::vector<graphics::Attachment_Description> attachmentsDescriptions =
  {
    graphics::Attachment_Description({
      .format        = rendererCoreComponent->swapchain->getSurfaceFormat().format,
      .samples       = VK_SAMPLE_COUNT_1_BIT,
      .colorOp       = graphics::attachment_op::gDontCareStore,
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

  imguiRendererComponent->renderPass = core::allocateShared<graphics::Render_Pass>(rendererCoreComponent->device, subpassDescriptions, subpassesDependencies, attachmentsDescriptions);
}

void imguiRendererPassSystemInitializeDescriptorPool(ImGui_Renderer_Component* imguiRendererComponent, graphics::Renderer_Core_Component* rendererCoreComponent)
{
  core::array<graphics::Descriptor_Pool_Size, 1> poolSizes =
  {
      graphics::Descriptor_Pool_Size(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u)
  };

  imguiRendererComponent->descriptorPool = core::allocateShared<graphics::Descriptor_Pool>(rendererCoreComponent->device, poolSizes, true);
}

void imguiRendererPassSystemInitializeFramebuffers(ImGui_Renderer_Component* imguiRendererComponent, graphics::Renderer_Core_Component* rendererCoreComponent)
{
  imguiRendererComponent->framebuffers.reserve(rendererCoreComponent->swapchainImages.size());
  for (core::uint32 i = 0; i < rendererCoreComponent->swapchainImages.size(); ++i)
  {
    imguiRendererComponent->framebuffers.push_back(core::allocateShared<graphics::Framebuffer>(
      rendererCoreComponent->device, 
      imguiRendererComponent->renderPass, 
      core::vector{ rendererCoreComponent->swapchainImagesViews[i] }, 
      rendererCoreComponent->swapchainImages[i]->getExtent2D(), 
      1u));
  }
}

}