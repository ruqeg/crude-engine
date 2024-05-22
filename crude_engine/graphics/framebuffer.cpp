#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.framebuffer;

import crude.graphics.vulkan.device;
import crude.graphics.vulkan.render_pass;
import crude.graphics.vulkan.image_view;
import crude.graphics.vulkan.vulkan_utils;
import crude.core.algorithms;

namespace crude::graphics
{


Framebuffer::Framebuffer(core::Shared_Ptr<const Device>                     device,
                         core::Shared_Ptr<Render_Pass>                      renderPass,
                         const core::vector<core::Shared_Ptr<Image_View>>&  attachments,
                         core::uint32                                       width,
                         core::uint32                                       height,
                         core::uint32                                       layers)
  :
  m_device(device),
  m_attachments(attachments),
  m_renderPass(renderPass)
{
  core::vector<VkImageView> attachmentsHandles(m_attachments.size());
  core::Algorithms::copyc(m_attachments.begin(), m_attachments.end(), attachmentsHandles.begin(), [](auto& src, auto& dst) -> void {
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