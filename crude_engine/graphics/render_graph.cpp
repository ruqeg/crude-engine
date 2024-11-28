#include <vulkan/vulkan.hpp>

module crude.gfx.render_graph;

import crude.gfx.vk.render_pass;
import crude.gfx.vk.device;
import crude.gfx.vk.image_view;
import crude.gfx.vk.attachment_description;
import crude.gfx.vk.image_attachment;
import crude.gfx.vk.framebuffer;

namespace crude::gfx
{

Render_Pass::Render_Pass(const Initialize& initialize)
  : m_graph{ initialize.graph }
  , m_queue{ initialize.queue }
  , m_name{ initialize.name }
  , m_framebufferExtent{ initialize.framebufferExtent }
{
  core::vector<vk::Attachment_Description> attachmentsDescriptions;
  core::vector<VkImageLayout> colorLayouts;

  attachmentsDescriptions.reserve(initialize.colorAttachments.size());
  colorLayouts.reserve(initialize.colorAttachments.size());

  for (auto colorAttachment : initialize.colorAttachments)
  {
    m_outputs.push_back(core::allocateShared<vk::Image_View>(core::allocateShared<vk::Color_Attachment>(vk::Color_Attachment::Initialize{
      .device          = m_graph.m_device,
      .format          = colorAttachment.format,
      .extent          = colorAttachment.extent,
      .sampled         = true,
      .explicitResolve = false,
      .mipLevelsCount  = 1u,
      .samples         = colorAttachment.samples })));

    attachmentsDescriptions.push_back(vk::Attachment_Description({
      .format        = colorAttachment.format,
      .samples       = colorAttachment.samples,
      .colorOp       = vk::attachment_op::gDontCareStore,
      .stenicilOp    = vk::attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }));

    colorLayouts.push_back(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  }

  if (initialize.depthAttachment.has_value())
  {
    m_outputs.push_back(core::allocateShared<vk::Image_View>(core::allocateShared<vk::Depth_Stencil_Attachment>(vk::Depth_Stencil_Attachment::Initialize{
      .device             = m_graph.m_device,
      .depthStencilFormat = initialize.depthAttachment.value().format,
      .extent             = initialize.depthAttachment.value().extent,
      .sampled            = true,
      .explicitResolve    = false,
      .mipLevelsCount     = 1u,
      .samples            = initialize.depthAttachment.value().samples })));

    attachmentsDescriptions.push_back(vk::Attachment_Description({
      .format        = initialize.depthAttachment.value().format,
      .samples       = initialize.depthAttachment.value().samples,
      .colorOp       = vk::attachment_op::gDontCareStore,
      .stenicilOp    = vk::attachment_op::gClearStore,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }));
  }

  vk::Subpass_Description subpassDescription;
  if (initialize.depthAttachment.has_value())
  {
    subpassDescription = vk::Subpass_Description{ vk::Subpass_Description::Initialize_Color_Array_Depth { colorLayouts, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL } };
  }
  else
  {
    subpassDescription = vk::Subpass_Description{ vk::Subpass_Description::Initialize_Color_Array { colorLayouts } };
  }

  vk::Subpass_Dependency subpassDependency({
    .srcSubpass      = VK_SUBPASS_EXTERNAL,
    .dstSubpass      = 0,
    .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    .srcAccessMask   = 0u,
    .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    .dependencyFlags = 0 });

  m_renderPass = core::allocateShared<vk::Render_Pass>(
    m_graph.m_device, 
    core::span<const vk::Subpass_Description>{ &subpassDescription, 1u },
    core::span<const vk::Subpass_Dependency>{ &subpassDependency, 1u },
    attachmentsDescriptions);

  m_framebuffers.reserve(m_graph.m_swapchainImagesCount);
  for (core::uint32 i = 0; i < m_graph.m_swapchainImagesCount; ++i)
  {
    m_framebuffers.push_back(core::allocateShared<vk::Framebuffer>(
      m_graph.m_device, m_renderPass, 
      m_outputs,
      m_framebufferExtent, 1u));
  }
}

Render_Graph::Render_Graph(const Initialize& initialize)
  : m_device{ initialize.device }
  , m_swapchainImagesCount{ initialize.swapchainImagesCount }
{
  for (const Render_Pass_Info& renderPassInfo : initialize.renderPassesInfos)
  {
    m_passes.push_back(core::allocateShared<Render_Pass>(Render_Pass::Initialize{
      .graph             = *this,
      .queue             = renderPassInfo.queue,
      .name              = renderPassInfo.name,
      .colorAttachments  = renderPassInfo.colorAttachments,
      .depthAttachment   = renderPassInfo.depthAttachment,
      .framebufferExtent = renderPassInfo.framebufferExtent
      }));
  }
}

}