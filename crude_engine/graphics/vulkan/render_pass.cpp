#include <vulkan/vulkan.hpp>

module crude.gfx.vk.render_pass;

import crude.gfx.vk.device;
import crude.gfx.vk.vulkan_utils;
import crude.gfx.vk.image_view;
import crude.gfx.vk.image;

namespace crude::gfx::vk
{

Render_Pass::Render_Pass(core::shared_ptr<const Device>        device,
                         core::span<const Subpass_Description> subpasses,
                         core::span<const Subpass_Dependency>  subpassesDependencies,
                         core::vector<Attachment_Description>  attachmentsDescriptions)
  : m_device(device)
  , m_attachmentsDesc(attachmentsDescriptions)
{
  VkRenderPassCreateInfo vkCreateInfo{};
  vkCreateInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  vkCreateInfo.pNext            = nullptr;
  vkCreateInfo.flags            = 0u;

  vkCreateInfo.attachmentCount  = static_cast<core::uint32>(m_attachmentsDesc.size());
  vkCreateInfo.pAttachments     = m_attachmentsDesc.data();
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
