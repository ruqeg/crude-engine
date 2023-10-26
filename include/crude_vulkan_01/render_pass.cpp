#include "render_pass.hpp"
#include "core.hpp"
#include "device.hpp"
#include <vulkan/vulkan_core.h>

namespace crude_vulkan_01 
{
  
SubpassDescription::SubpassDescription(VkPipelineBindPoint                          pipelineBindPoint,
                                       const std::vector<VkAttachmentReference>&    inputAttachments,
                                       const std::vector<VkAttachmentReference>&    colorAttachments,
                                       // !TODO const std::vector<VkAttachmentReference>&  resolveAttachments,
                                       const std::optional<VkAttachmentReference>&  depthStencilAttachment,
                                       const std::vector<uint32>&                   preserveAttachments)
{
  this->flags                    = 0u;
  this->pipelineBindPoint        = pipelineBindPoint;
  this->inputAttachmentCount     = static_cast<uint32>(inputAttachments.size());
  this->colorAttachmentCount     = static_cast<uint32>(colorAttachments.size());
  this->preserveAttachmentCount  = static_cast<uint32>(preserveAttachments.size());
  this->pInputAttachments        = nullptr;
  this->pColorAttachments        = nullptr;
  this->pPreserveAttachments     = nullptr;
  this->pDepthStencilAttachment  = nullptr;
  
  // !TODO
  this->pResolveAttachments      = nullptr;

  //!TODO upgrade code
  
  if (colorAttachmentCount > 0u)
  {
    this->pColorAttachments = new VkAttachmentReference[colorAttachmentCount];
    std::copy(colorAttachments.begin(), colorAttachments.end(), const_cast<VkAttachmentReference*>(pColorAttachments));
  }

  if (inputAttachmentCount > 0u)
  {
    this->pInputAttachments = new VkAttachmentReference[inputAttachmentCount];
    std::copy(inputAttachments.begin(), inputAttachments.end(), const_cast<VkAttachmentReference*>(pInputAttachments));
  }

  if (preserveAttachmentCount > 0u)
  {
    this->pPreserveAttachments = new uint32[preserveAttachmentCount];
    std::copy(preserveAttachments.begin(), preserveAttachments.end(), const_cast<uint32*>(pPreserveAttachments));
  }

  if (depthStencilAttachment.has_value())
  {
    this->pDepthStencilAttachment = new VkAttachmentReference(depthStencilAttachment.value()); 
  }
}

SubpassDescription::~SubpassDescription()
{
  if (pColorAttachments)  delete[] pColorAttachments;
  if (pInputAttachments)  delete[] pInputAttachments;
  if (pDepthStencilAttachment)  delete pDepthStencilAttachment;
  if (pPreserveAttachments)  delete[] pPreserveAttachments;
}
  
SubpassDescription::SubpassDescription(const SubpassDescription& other)
{
  this->flags                    = other.flags;
  this->pipelineBindPoint        = other.pipelineBindPoint;
  this->inputAttachmentCount     = other.inputAttachmentCount; 
  this->colorAttachmentCount     = other.colorAttachmentCount;
  this->preserveAttachmentCount  = other.preserveAttachmentCount;
  this->pInputAttachments        = nullptr;
  this->pColorAttachments        = nullptr;
  this->pPreserveAttachments     = nullptr;
  this->pDepthStencilAttachment  = nullptr;

  // !TODO
  this->pResolveAttachments      = nullptr;

  //!TODO upgrade code
  
  if (colorAttachmentCount > 0u)
  {
    this->pColorAttachments = new VkAttachmentReference[colorAttachmentCount];
    std::copy(other.pColorAttachments, other.pColorAttachments + colorAttachmentCount, const_cast<VkAttachmentReference*>(pColorAttachments));
  }

  if (inputAttachmentCount > 0u)
  {
    this->pInputAttachments = new VkAttachmentReference[inputAttachmentCount];
    std::copy(other.pInputAttachments, other.pInputAttachments + inputAttachmentCount, const_cast<VkAttachmentReference*>(pInputAttachments));
  }

  if (preserveAttachmentCount > 0u)
  {
    this->pPreserveAttachments = new uint32[preserveAttachmentCount];
    std::copy(other.pPreserveAttachments, other.pPreserveAttachments + preserveAttachmentCount, const_cast<uint32*>(pPreserveAttachments));
  }

  if (other.pDepthStencilAttachment != nullptr)
  {
    this->pDepthStencilAttachment = new VkAttachmentReference(*other.pDepthStencilAttachment); 
  }
}
  
SubpassDescription& SubpassDescription::operator=(const SubpassDescription& other)
{
  this->flags                    = other.flags;
  this->pipelineBindPoint        = other.pipelineBindPoint;
  this->inputAttachmentCount     = other.inputAttachmentCount; 
  this->colorAttachmentCount     = other.colorAttachmentCount;
  this->preserveAttachmentCount  = other.preserveAttachmentCount;
  this->pInputAttachments        = nullptr;
  this->pColorAttachments        = nullptr;
  this->pPreserveAttachments     = nullptr;
  this->pDepthStencilAttachment  = nullptr;

  // !TODO
  this->pResolveAttachments      = nullptr;

  //!TODO upgrade code
  
  if (colorAttachmentCount > 0u)
  {
    this->pColorAttachments = new VkAttachmentReference[colorAttachmentCount];
    std::copy(other.pColorAttachments, other.pColorAttachments + colorAttachmentCount, const_cast<VkAttachmentReference*>(pColorAttachments));
  }

  if (inputAttachmentCount > 0u)
  {
    this->pInputAttachments = new VkAttachmentReference[inputAttachmentCount];
    std::copy(other.pInputAttachments, other.pInputAttachments + inputAttachmentCount, const_cast<VkAttachmentReference*>(pInputAttachments));
  }

  if (preserveAttachmentCount > 0u)
  {
    this->pPreserveAttachments = new uint32[preserveAttachmentCount];
    std::copy(other.pPreserveAttachments, other.pPreserveAttachments + preserveAttachmentCount, const_cast<uint32*>(pPreserveAttachments));
  }

  if (other.pDepthStencilAttachment != nullptr)
  {
    this->pDepthStencilAttachment = new VkAttachmentReference(*other.pDepthStencilAttachment); 
  }

  return *this;
}
  
SubpassDescription::SubpassDescription(SubpassDescription&& other)
{
  this->flags                    = other.flags;
  this->pipelineBindPoint        = other.pipelineBindPoint;
  this->inputAttachmentCount     = other.inputAttachmentCount; 
  this->colorAttachmentCount     = other.colorAttachmentCount;
  this->preserveAttachmentCount  = other.preserveAttachmentCount;
  this->pInputAttachments        = other.pInputAttachments;
  this->pColorAttachments        = other.pColorAttachments;
  this->pPreserveAttachments     = other.pPreserveAttachments;
  this->pDepthStencilAttachment  = other.pDepthStencilAttachment;
  this->pResolveAttachments      = other.pResolveAttachments;

  other.pInputAttachments        = nullptr;
  other.pColorAttachments        = nullptr;
  other.pPreserveAttachments     = nullptr;
  other.pDepthStencilAttachment  = nullptr;
  other.pResolveAttachments      = nullptr;
}

SubpassDescription& SubpassDescription::operator=(SubpassDescription&& other)
{
  this->flags                    = other.flags;
  this->pipelineBindPoint        = other.pipelineBindPoint;
  this->inputAttachmentCount     = other.inputAttachmentCount; 
  this->colorAttachmentCount     = other.colorAttachmentCount;
  this->preserveAttachmentCount  = other.preserveAttachmentCount;
  this->pInputAttachments        = other.pInputAttachments;
  this->pColorAttachments        = other.pColorAttachments;
  this->pPreserveAttachments     = other.pPreserveAttachments;
  this->pDepthStencilAttachment  = other.pDepthStencilAttachment;
  this->pResolveAttachments      = other.pResolveAttachments;

  other.pInputAttachments        = nullptr;
  other.pColorAttachments        = nullptr;
  other.pPreserveAttachments     = nullptr;
  other.pDepthStencilAttachment  = nullptr;
  other.pResolveAttachments      = nullptr;

  return *this;
}
  
SubpassDependency::SubpassDependency(uint32                srcSubpass,
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
  
AttachmentDescription::AttachmentDescription(VkFormat               format,
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
  
RenderPassCreateInfo::RenderPassCreateInfo(std::shared_ptr<const Device>              device,
                                           const std::vector<SubpassDescription>&     subpasses,
                                           const std::vector<SubpassDependency>&      subpassesDependencies,
                                           const std::vector<AttachmentDescription>&  attachments)
  :
  device(device),
  subpasses(subpasses),
  subpassesDependencies(subpassesDependencies),
  attachments(attachments)
{}

Render_Pass::Render_Pass(const RenderPassCreateInfo& createInfo)
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
