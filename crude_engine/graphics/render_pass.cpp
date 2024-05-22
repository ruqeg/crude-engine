#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.render_pass;

import crude.graphics.vulkan.device;
import crude.graphics.vulkan.vulkan_utils;

namespace crude::graphics
{
  
Render_Pass::Render_Pass(core::Shared_Ptr<const Device>             device,
                         const core::span<Subpass_Description>&     subpasses,
                         const core::span<Subpass_Dependency>&      subpassesDependencies,
                         const core::span<Attachment_Description>&  attachments)
  :
  m_device(device)
{
  VkRenderPassCreateInfo vkCreateInfo{};
  vkCreateInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  vkCreateInfo.pNext            = nullptr;
  vkCreateInfo.flags            = 0u;

  vkCreateInfo.attachmentCount  = static_cast<core::uint32>(attachments.size());
  vkCreateInfo.pAttachments     = attachments.data();
  vkCreateInfo.subpassCount     = static_cast<core::uint32>(subpasses.size());
  vkCreateInfo.pSubpasses       = subpasses.data();
  vkCreateInfo.dependencyCount  = static_cast<core::uint32>(subpassesDependencies.size());
  vkCreateInfo.pDependencies    = subpassesDependencies.data();

  const VkResult result = vkCreateRenderPass(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create render pass");
}
  
Render_Pass::~Render_Pass()
{
  vkDestroyRenderPass(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}
