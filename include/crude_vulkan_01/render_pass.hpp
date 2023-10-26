#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <algorithm>
#include <optional>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace crude_vulkan_01
{

class Device;

struct SubpassDescription final : public VkSubpassDescription
{
  explicit SubpassDescription(VkPipelineBindPoint                          pipelineBindPoint,
                              const std::vector<VkAttachmentReference>&    inputAttachments,
                              const std::vector<VkAttachmentReference>&    colorAttachments,
                              //!TODO const std::vector<Attachment_Reference>&  resolveAttachments,
                              const std::optional<VkAttachmentReference>&  depthStencilAttachment,
                              const std::vector<uint32>&                   preserveAttachments);
  ~SubpassDescription();
  SubpassDescription(const SubpassDescription& other);
  SubpassDescription& operator=(const SubpassDescription& other);
  SubpassDescription(SubpassDescription&& other);
  SubpassDescription& operator=(SubpassDescription&& other);
};

struct SubpassDependency final : public VkSubpassDependency
{
  explicit SubpassDependency(uint32                srcSubpass,
                             uint32                dstSubpass,
                             VkPipelineStageFlags  srcStageMask,
                             VkPipelineStageFlags  dstStageMask,
                             VkAccessFlags         srcAccessMask,
                             VkAccessFlags         dstAccessMask,
                             VkDependencyFlags     dependencyFlags);
};

struct AttachmentDescription final : public VkAttachmentDescription
{
  AttachmentDescription(VkFormat               format,
                        VkSampleCountFlagBits  samples,
                        VkAttachmentLoadOp     loadOp,
                        VkAttachmentStoreOp    storeOp,
                        VkAttachmentLoadOp     stencilLoadOp,
                        VkAttachmentStoreOp    stencilStoreOp,
                        VkImageLayout          initialLayout,
                        VkImageLayout          finalLayout);
};

struct RenderPassCreateInfo 
{
  std::shared_ptr<const Device>       device;
  std::vector<SubpassDescription>     subpasses;
  std::vector<SubpassDependency>      subpassesDependencies;
  std::vector<AttachmentDescription>  attachments;
  explicit RenderPassCreateInfo(std::shared_ptr<const Device>              device,
                                const std::vector<SubpassDescription>&     subpasses,
                                const std::vector<SubpassDependency>&      subpassesDependencies,
                                const std::vector<AttachmentDescription>&  attachments);
};

class Render_Pass : public TObject<VkRenderPass>
{
public:
  explicit Render_Pass(const RenderPassCreateInfo& createInfo);
  ~Render_Pass();
private:
  std::shared_ptr<const Device>  m_device;
};

}
