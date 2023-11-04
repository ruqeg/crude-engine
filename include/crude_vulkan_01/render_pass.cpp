#include "render_pass.hpp"
#include "device.hpp"

namespace crude_vulkan_01 
{
  
Render_Pass_Create_Info::Render_Pass_Create_Info(std::shared_ptr<const Device>              device,
                                           const std::vector<Subpass_Description>&     subpasses,
                                           const std::vector<Subpass_Dependency>&      subpassesDependencies,
                                           const std::vector<Attachment_Description>&  attachments)
  :
  device(device),
  subpasses(subpasses),
  subpassesDependencies(subpassesDependencies),
  attachments(attachments)
{}

Render_Pass::Render_Pass(const Render_Pass_Create_Info& createInfo)
{
  m_device = createInfo.device;

  VkRenderPassCreateInfo vkCreateInfo{};
  vkCreateInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  vkCreateInfo.pNext            = nullptr;
  vkCreateInfo.flags            = 0u;

  vkCreateInfo.attachmentCount  = static_cast<uint32>(createInfo.attachments.size());
  vkCreateInfo.pAttachments     = createInfo.attachments.data();
  vkCreateInfo.subpassCount     = static_cast<uint32>(createInfo.subpasses.size());
  vkCreateInfo.pSubpasses       = createInfo.subpasses.data();
  vkCreateInfo.dependencyCount  = static_cast<uint32>(createInfo.subpassesDependencies.size());
  vkCreateInfo.pDependencies    = createInfo.subpassesDependencies.data();

  const VkResult result = vkCreateRenderPass(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create render pass");
}
  
Render_Pass::~Render_Pass()
{
  vkDestroyRenderPass(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}
