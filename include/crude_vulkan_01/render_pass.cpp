#include "render_pass.hpp"
#include "device.hpp"

namespace crude_vulkan_01 
{
  
Subpass_Dependency::Subpass_Dependency(uint32                srcSubpass,
                                     uint32                dstSubpass,
                                     VkPipelineStageFlags  srcStageMask,
                                     VkPipelineStageFlags  dstStageMask,
                                     VkAccessFlags         srcAccessMask,
                                     VkAccessFlags         dstAccessMask,
                                     VkDependencyFlags     dependencyFlags)
{
  this->srcSubpass       = srcSubpass;
  this->dstSubpass       = dstSubpass;
  this->srcStageMask     = srcStageMask;
  this->dstStageMask     = dstStageMask;
  this->srcAccessMask    = srcAccessMask;
  this->dstAccessMask    = dstAccessMask;
  this->dependencyFlags  = dependencyFlags;
}
  
Attachment_Description::Attachment_Description(VkFormat               format,
                                             VkSampleCountFlagBits  samples,
                                             VkAttachmentLoadOp     loadOp,
                                             VkAttachmentStoreOp    storeOp,
                                             VkAttachmentLoadOp     stencilLoadOp,
                                             VkAttachmentStoreOp    stencilStoreOp,
                                             VkImageLayout          initialLayout,
                                             VkImageLayout          finalLayout)
{
  this->flags           = 0u;
  this->format          = format;
  this->samples         = samples;
  this->loadOp          = loadOp;
  this->storeOp         = storeOp;
  this->stencilLoadOp   = stencilLoadOp;
  this->stencilStoreOp  = stencilStoreOp;
  this->initialLayout   = initialLayout;
  this->finalLayout     = finalLayout;
}
  
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
  vkCreateInfo.attachmentCount  = static_cast<uint32>(createInfo.attachments.size());
  vkCreateInfo.pAttachments     = createInfo.attachments.data();
  vkCreateInfo.subpassCount     = static_cast<uint32>(createInfo.subpasses.size());
  vkCreateInfo.pSubpasses       = createInfo.subpasses.data();
  vkCreateInfo.dependencyCount  = static_cast<uint32>(createInfo.subpassesDependencies.size());
  vkCreateInfo.pDependencies    = createInfo.subpassesDependencies.data();
  vkCreateInfo.pNext            = nullptr;
  vkCreateInfo.flags            = 0u;

  const VkResult result = vkCreateRenderPass(
    CRUDE_VULKAN_01_HANDLE(m_device), 
    &vkCreateInfo, 
    nullptr, 
    &m_handle);

  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create render pass");
}
  
Render_Pass::~Render_Pass()
{
  vkDestroyRenderPass(
    CRUDE_VULKAN_01_HANDLE(m_device), 
    m_handle,
    nullptr);
}

}
