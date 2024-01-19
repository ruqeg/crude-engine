#include <graphics/vulkan/render_pass.hpp>
#include <graphics/vulkan/device.hpp>

namespace crude_engine 
{
  
Render_Pass::Render_Pass(Shared_Ptr<const Device>                     device,
                         const Array_Unsafe<Subpass_Description>&     subpasses,
                         const Array_Unsafe<Subpass_Dependency>&      subpassesDependencies,
                         const Array_Unsafe<Attachment_Description>&  attachments)
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

  const VkResult result = vkCreateRenderPass(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create render pass");
}
  
Render_Pass::~Render_Pass()
{
  vkDestroyRenderPass(CRUDE_OBJECT_HANDLE(m_device), m_handle, getPVkAllocationCallbacks());
}

}
