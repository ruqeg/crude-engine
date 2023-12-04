#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <optional>
#include <vector>

namespace crude_engine
{

class Device;

// ! sizeof(Subpass_Description) == sizeof(VkSubpassDescription)
// for render_pass.cpp
struct Subpass_Description final : public VkSubpassDescription
{
  explicit Subpass_Description(VkPipelineBindPoint                          pipelineBindPoint,
                               const std::vector<VkAttachmentReference>&    inputAttachments,
                               const std::vector<VkAttachmentReference>&    colorAttachments,
                               //!TODO const std::vector<Attachment_Reference>&  resolveAttachments,
                               const std::optional<VkAttachmentReference>&  depthStencilAttachment,
                               const std::vector<uint32>&                   preserveAttachments);
  ~Subpass_Description();
  Subpass_Description(const Subpass_Description& other);
  Subpass_Description& operator=(const Subpass_Description& other);
  Subpass_Description(Subpass_Description&& other) noexcept;
  Subpass_Description& operator=(Subpass_Description&& other) noexcept;
private:
  void copy(VkPipelineBindPoint           opipelineBindPoint,
            const VkAttachmentReference*  opInputAttachments,
            uint32                        oinputAttachmentsCount,
            const VkAttachmentReference*  opColorAttachments,
            uint32                        ocolorAttachmentsCount,
            const VkAttachmentReference*  opDepthStencilAttachment,
            const uint32*                 opPreserveAttachments,
            uint32                        opreserveAttachmentsCount);
  void move(Subpass_Description& other);
};

}