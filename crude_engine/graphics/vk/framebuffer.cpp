#include <vulkan/vulkan.hpp>

module crude.gfx.vk.framebuffer;

import crude.gfx.vk.device;
import crude.gfx.vk.render_pass;
import crude.gfx.vk.image_view;
import crude.gfx.vk.vulkan_utils;
import crude.core.algorithms;

namespace crude::gfx::vk
{

Framebuffer::Framebuffer(core::shared_ptr<const Device>                     device,
                         core::shared_ptr<Render_Pass>                      renderPass,
                         const core::vector<core::shared_ptr<Image_View>>&  attachments,
                         const VkExtent2D&                                  extent,
                         core::uint32                                       layers)
  : Framebuffer(device, renderPass, attachments, extent.width, extent.height, layers)
{}

Framebuffer::Framebuffer(core::shared_ptr<const Device>                     device,
                         core::shared_ptr<Render_Pass>                      renderPass,
                         const core::vector<core::shared_ptr<Image_View>>&  attachments,
                         core::uint32                                       width,
                         core::uint32                                       height,
                         core::uint32                                       layers)
  :
  m_device(device),
  m_attachments(attachments),
  m_renderPass(renderPass)
{
  core::vector<VkImageView> attachmentsHandles(m_attachments.size());
  core::copyc(m_attachments.begin(), m_attachments.end(), attachmentsHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
  });

  VkFramebufferCreateInfo vkCreateInfo{};
  vkCreateInfo.sType            = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  vkCreateInfo.pNext            = nullptr;
  vkCreateInfo.flags            = 0u;

  vkCreateInfo.renderPass       = m_renderPass->getHandle();
  vkCreateInfo.attachmentCount  = static_cast<core::uint32>(attachmentsHandles.size());
  vkCreateInfo.pAttachments     = attachmentsHandles.data();
  vkCreateInfo.width            = width;
  vkCreateInfo.height           = height;
  vkCreateInfo.layers           = layers;

  VkResult result = vkCreateFramebuffer(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create framebuffer");
}

Framebuffer::~Framebuffer()
{
  vkDestroyFramebuffer(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}