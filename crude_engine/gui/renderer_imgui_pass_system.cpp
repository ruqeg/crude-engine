#include <flecs.h>
#include <vulkan/vulkan.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

module crude.gui.renderer_imgui_pass_system;

import crude.gfx.vk.device;
import crude.gfx.vk.render_pass;
import crude.gfx.vk.framebuffer;
import crude.gfx.vk.descriptor_pool;
import crude.gfx.vk.swap_chain;
import crude.gfx.vk.swap_chain_image;
import crude.gfx.vk.image_view;
import crude.gfx.vk.command_buffer;
import crude.gfx.vk.instance;
import crude.gfx.vk.queue;
import crude.gfx.vk.physical_device;
import crude.gfx.vk.vulkan_utils;
import crude.platform.sdl_window_container;

namespace crude::gui
{

void rendererImguiPassSystemProcess(flecs::iter& it)
{
  Renderer_ImGui_Pass_System_Ctx* imguiPassCtx = it.ctx<Renderer_ImGui_Pass_System_Ctx>();
  core::shared_ptr<gfx::Renderer_Frame_System_Ctx> frameCtx = imguiPassCtx->frameCtx;
  core::shared_ptr<gfx::Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  for (auto& drawLayoutSystem : imguiPassCtx->layoutsDrawSystems)
  {
    drawLayoutSystem.run();
  }

  ImGui::Render();

  ImDrawData* drawData = ImGui::GetDrawData();
  const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
  if (!isMinimized)
  {
    core::array<VkClearValue, 2u> clearValues;
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRect2D renderArea;
    renderArea.extent = coreCtx->swapchain->getExtent();
    renderArea.offset = VkOffset2D{ 0, 0 };

    frameCtx->getFrameGraphicsCommandBuffer()->beginRenderPass(imguiPassCtx->renderPass, imguiPassCtx->framebuffers[frameCtx->swapchainImageIndex], clearValues, renderArea);

    ImGui_ImplVulkan_RenderDrawData(drawData, frameCtx->getFrameGraphicsCommandBuffer()->getHandle());

    frameCtx->getFrameGraphicsCommandBuffer()->endRenderPass();
  }
}

Renderer_ImGui_Pass_System_Ctx::Renderer_ImGui_Pass_System_Ctx(core::shared_ptr<gfx::Renderer_Frame_System_Ctx> frameCtx)
  : frameCtx(frameCtx)
{
  core::shared_ptr<gfx::Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  initializeRenderPass();
  initializeDescriptorPool();
  initializeFramebuffers();

  ImGui_ImplSDL3_InitForVulkan(coreCtx->windowContainer->getHandle().get());

  ImGui_ImplVulkan_InitInfo init_info{
    .Instance        = coreCtx->instance->getHandle(),
    .PhysicalDevice  = coreCtx->device->getPhysicalDevice()->getHandle(),
    .Device          = coreCtx->device->getHandle(),
    .QueueFamily     = coreCtx->graphicsQueue->getFamilyIndex(),
    .Queue           = coreCtx->graphicsQueue->getHandle(),
    .DescriptorPool  = descriptorPool->getHandle(),
    .RenderPass      = renderPass->getHandle(),
    .MinImageCount   = 3,
    .ImageCount      = 3,
    .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
    .PipelineCache   = VK_NULL_HANDLE,
    .Subpass         = 0,
    .Allocator       = nullptr,
    .CheckVkResultFn = [](VkResult err) {
      gfx::vk::vulkanHandleResult(err, "ImGUI vulkan error!");
    }
  };
  ImGui_ImplVulkan_Init(&init_info);
}

Renderer_ImGui_Pass_System_Ctx::~Renderer_ImGui_Pass_System_Ctx()
{
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplSDL3_Shutdown();
}

void Renderer_ImGui_Pass_System_Ctx::initializeRenderPass()
{
  core::shared_ptr<gfx::Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  core::array<gfx::vk::Subpass_Description, 1> subpassDescriptions =
  {
    gfx::vk::Subpass_Description(gfx::vk::Subpass_Description::Initialize_Color{VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL})
  };

  core::vector<gfx::vk::Attachment_Description> attachmentsDescriptions =
  {
    gfx::vk::Attachment_Description({
      .format        = coreCtx->swapchain->getSurfaceFormat().format,
      .samples       = VK_SAMPLE_COUNT_1_BIT,
      .colorOp       = gfx::vk::attachment_op::gClearStore,
      .stenicilOp    = gfx::vk::attachment_op::gDontCare,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR })
  };

  core::array<gfx::vk::Subpass_Dependency, 1u> subpassesDependencies =
  {
    gfx::vk::Subpass_Dependency({
      .srcSubpass      = VK_SUBPASS_EXTERNAL,
      .dstSubpass      = 0,
      .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .srcAccessMask   = 0u,
      .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0})
  };

  renderPass = core::allocateShared<gfx::vk::Render_Pass>(coreCtx->device, subpassDescriptions, subpassesDependencies, attachmentsDescriptions);
}

void Renderer_ImGui_Pass_System_Ctx::initializeDescriptorPool()
{
  core::shared_ptr<gfx::Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  core::array<gfx::vk::Descriptor_Pool_Size, 1> poolSizes =
  {
      gfx::vk::Descriptor_Pool_Size(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u)
  };

  descriptorPool = core::allocateShared<gfx::vk::Descriptor_Pool>(coreCtx->device, poolSizes, 4, true);
}

void Renderer_ImGui_Pass_System_Ctx::initializeFramebuffers()
{
  core::shared_ptr<gfx::Renderer_Core_System_Ctx> coreCtx = frameCtx->coreCtx;

  framebuffers.reserve(coreCtx->swapchainImages.size());
  for (core::uint32 i = 0; i < coreCtx->swapchainImages.size(); ++i)
  {
    framebuffers.push_back(core::allocateShared<gfx::vk::Framebuffer>(
      coreCtx->device,
      renderPass,
      core::vector{ coreCtx->swapchainImagesViews[i] },
      coreCtx->swapchainImages[i]->getExtent2D(),
      1u));
  }
}

}