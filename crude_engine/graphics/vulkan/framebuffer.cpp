#include "framebuffer.hpp"
#include "render_pass.hpp"
#include "image_view.hpp"
#include "device.hpp"

namespace crude_engine
{

Framebuffer_Create_Info::Framebuffer_Create_Info(std::shared_ptr<const Device>                    device,
                                                 std::shared_ptr<Render_Pass>                     renderPass,
                                                 const std::vector<std::shared_ptr<Image_View>>&  attachments,
                                                 uint32                                           width,
                                                 uint32                                           height,
                                                 uint32                                           layers)
  :
  device(device),
  renderPass(renderPass),
  attachments(attachments),
  width(width),
  height(height),
  layers(layers)
{}

Framebuffer::Framebuffer(const Framebuffer_Create_Info& createInfo)
{
  m_device       = createInfo.device;
  m_attachments  = createInfo.attachments;
  m_renderPass   = createInfo.renderPass;

  std::vector<VkImageView> attachmentsHandles(m_attachments.size());
  for (uint32 i = 0; i < attachmentsHandles.size(); ++i)
  {
    attachmentsHandles[i] = CRUDE_VULKAN_01_HANDLE(m_attachments[i]);
  }

  VkFramebufferCreateInfo vkCreateInfo{};
  vkCreateInfo.sType            = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  vkCreateInfo.pNext            = nullptr;
  vkCreateInfo.flags            = 0u;

  vkCreateInfo.renderPass       = CRUDE_VULKAN_01_HANDLE(m_renderPass);
  vkCreateInfo.attachmentCount  = static_cast<uint32>(attachmentsHandles.size());
  vkCreateInfo.pAttachments     = attachmentsHandles.data();
  vkCreateInfo.width            = createInfo.width;
  vkCreateInfo.height           = createInfo.height;
  vkCreateInfo.layers           = createInfo.layers;

  VkResult result = vkCreateFramebuffer(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create framebuffer");
}

Framebuffer::~Framebuffer()
{
  vkDestroyFramebuffer(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}