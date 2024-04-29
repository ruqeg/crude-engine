#include <graphics/vulkan/framebuffer.hpp>
#include <graphics/vulkan/render_pass.hpp>
#include <graphics/vulkan/image_view.hpp>
#include <graphics/vulkan/device.hpp>
#include <core/algorithms.hpp>

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
    *dst = CRUDE_OBJECT_HANDLE(*src);
  });

  VkFramebufferCreateInfo vkCreateInfo{};
  vkCreateInfo.sType            = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  vkCreateInfo.pNext            = nullptr;
  vkCreateInfo.flags            = 0u;

  vkCreateInfo.renderPass       = CRUDE_OBJECT_HANDLE(m_renderPass);
  vkCreateInfo.attachmentCount  = static_cast<uint32>(attachmentsHandles.size());
  vkCreateInfo.pAttachments     = attachmentsHandles.data();
  vkCreateInfo.width            = width;
  vkCreateInfo.height           = height;
  vkCreateInfo.layers           = layers;

  VkResult result = vkCreateFramebuffer(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create framebuffer");
}

Framebuffer::~Framebuffer()
{
  vkDestroyFramebuffer(CRUDE_OBJECT_HANDLE(m_device), m_handle, getPVkAllocationCallbacks());
}

}