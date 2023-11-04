#pragma once

#include "subpass_description.hpp"
#include <algorithm>

namespace crude_vulkan_01
{

class Device;

struct Subpass_Dependency final : public VkSubpassDependency
{
  explicit Subpass_Dependency(uint32                srcSubpass,
                             uint32                dstSubpass,
                             VkPipelineStageFlags  srcStageMask,
                             VkPipelineStageFlags  dstStageMask,
                             VkAccessFlags         srcAccessMask,
                             VkAccessFlags         dstAccessMask,
                             VkDependencyFlags     dependencyFlags);
};

struct Attachment_Description final : public VkAttachmentDescription
{
  Attachment_Description(VkFormat               format,
                        VkSampleCountFlagBits  samples,
                        VkAttachmentLoadOp     loadOp,
                        VkAttachmentStoreOp    storeOp,
                        VkAttachmentLoadOp     stencilLoadOp,
                        VkAttachmentStoreOp    stencilStoreOp,
                        VkImageLayout          initialLayout,
                        VkImageLayout          finalLayout);
};

struct Render_Pass_Create_Info
{
  std::shared_ptr<const Device>       device;
  std::vector<Subpass_Description>     subpasses;
  std::vector<Subpass_Dependency>      subpassesDependencies;
  std::vector<Attachment_Description>  attachments;
  explicit Render_Pass_Create_Info(std::shared_ptr<const Device>              device,
                                const std::vector<Subpass_Description>&     subpasses,
                                const std::vector<Subpass_Dependency>&      subpassesDependencies,
                                const std::vector<Attachment_Description>&  attachments);
};

class Render_Pass : public TObject<VkRenderPass>
{
public:
  explicit Render_Pass(const Render_Pass_Create_Info& createInfo);
  ~Render_Pass();
private:
  std::shared_ptr<const Device>  m_device;
};

}
