#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.framebuffer;

import crude_engine.graphics.vulkan.device;
import crude_engine.graphics.vulkan.render_pass;
import crude_engine.graphics.vulkan.image_view;
import crude_engine.graphics.vulkan.vulkan_utils;
import crude_engine.core.algorithms;

namespace crude_engine
{


Framebuffer::Framebuffer(Shared_Ptr<const Device>               device,
                         Shared_Ptr<Render_Pass>                renderPass,
                         const vector<Shared_Ptr<Image_View>>&  attachments,
                         uint32                                 width,
                         uint32                                 height,
                         uint32                                 layers)
  :
  m_device(device),
  m_attachments(attachments),
  m_renderPass(renderPass)
{
  vector<VkImageView> attachmentsHandles(m_attachments.size());
  Algorithms::copyc(m_attachments.begin(), m_attachments.end(), attachmentsHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
  });

  VkFramebufferCreateInfo vkCreateInfo{};
  vkCreateInfo.sType            = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  vkCreateInfo.pNext            = nullptr;
  vkCreateInfo.flags            = 0u;

  vkCreateInfo.renderPass       = m_renderPass->getHandle();
  vkCreateInfo.attachmentCount  = static_cast<uint32>(attachmentsHandles.size());
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