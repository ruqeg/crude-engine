#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.render_pass;

import crude_engine.graphics.vulkan.device;
import crude_engine.graphics.vulkan.vulkan_utils;

namespace crude_engine 
{
  
Render_Pass::Render_Pass(Shared_Ptr<const Device>             device,
                         const span<Subpass_Description>&     subpasses,
                         const span<Subpass_Dependency>&      subpassesDependencies,
                         const span<Attachment_Description>&  attachments)
  :
  m_device(device)
{
  VkRenderPassCreateInfo vkCreateInfo{};
  vkCreateInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  vkCreateInfo.pNext            = nullptr;
  vkCreateInfo.flags            = 0u;

  vkCreateInfo.attachmentCount  = static_cast<uint32>(attachments.size());
  vkCreateInfo.pAttachments     = attachments.data();
  vkCreateInfo.subpassCount     = static_cast<uint32>(subpasses.size());
  vkCreateInfo.pSubpasses       = subpasses.data();
  vkCreateInfo.dependencyCount  = static_cast<uint32>(subpassesDependencies.size());
  vkCreateInfo.pDependencies    = subpassesDependencies.data();

  const VkResult result = vkCreateRenderPass(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create render pass");
}
  
Render_Pass::~Render_Pass()
{
  vkDestroyRenderPass(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}
