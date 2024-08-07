module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.color_blend_state_create_info;

import crude.core.alias;
import crude.core.std_containers_stack;
import crude.core.memory;

export namespace crude::graphics
{

struct Pipeline_Color_Blend_Attachment_State : VkPipelineColorBlendAttachmentState
{
public:
  struct Initialize
  {
    VkBool32                 blendEnable;
    VkColorComponentFlags    colorWriteMask;
    VkBlendFactor            srcColorBlendFactor;
    VkBlendFactor            dstColorBlendFactor;
    VkBlendOp                colorBlendOp;
    VkBlendFactor            srcAlphaBlendFactor;
    VkBlendFactor            dstAlphaBlendFactor;
    VkBlendOp                alphaBlendOp;
  };
public:
  Pipeline_Color_Blend_Attachment_State() = default;
  explicit Pipeline_Color_Blend_Attachment_State(const Initialize& info)
  {
    this->blendEnable          = info.blendEnable;
    this->srcColorBlendFactor  = info.srcColorBlendFactor;
    this->dstColorBlendFactor  = info.dstColorBlendFactor;
    this->colorBlendOp         = info.colorBlendOp;
    this->srcAlphaBlendFactor  = info.srcAlphaBlendFactor;
    this->dstAlphaBlendFactor  = info.dstAlphaBlendFactor;
    this->alphaBlendOp         = info.alphaBlendOp;
    this->colorWriteMask       = info.colorWriteMask;
  }
};

class Color_Blend_State_Create_Info final : public VkPipelineColorBlendStateCreateInfo
{
public:
  struct Initialize
  {
    core::span<const  Pipeline_Color_Blend_Attachment_State>  attachments;
    core::array<core::float32, 4>                             blendConstants;
    VkBool32                                                  logicOpEnable;
    VkLogicOp                                                 logicOp;
  };
public:
  explicit Color_Blend_State_Create_Info(const Initialize& info)
  {
    VkPipelineColorBlendAttachmentState* newAttachments = core::defaultCxxAllocate<VkPipelineColorBlendAttachmentState>(info.attachments.size());

    this->attachmentCount = newAttachments ? info.attachments.size() : 0u;

    for (core::uint32 i = 0; i < this->attachmentCount; ++i)
    {
      newAttachments[i] = static_cast<VkPipelineColorBlendAttachmentState>(info.attachments[i]);
    }

    this->sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    this->pNext              = nullptr;
    this->flags              = 0u;

    this->pAttachments       = newAttachments;
    this->blendConstants[0]  = info.blendConstants[0];
    this->blendConstants[1]  = info.blendConstants[1];
    this->blendConstants[2]  = info.blendConstants[2];
    this->blendConstants[3]  = info.blendConstants[3];
    this->logicOpEnable      = info.logicOpEnable;
    this->logicOp            = info.logicOp;
  }
  Color_Blend_State_Create_Info(const Color_Blend_State_Create_Info& other)
  {
    *this = other;
  }
  Color_Blend_State_Create_Info(Color_Blend_State_Create_Info&& other)
  {
    *this = std::move(other);
  }
  Color_Blend_State_Create_Info& operator=(const Color_Blend_State_Create_Info& other)
  {
    if (this != &other)
    {
      VkPipelineColorBlendAttachmentState* newAttachments = core::defaultCxxAllocate<VkPipelineColorBlendAttachmentState>(other.attachmentCount);
      
      this->attachmentCount = newAttachments ? other.attachmentCount : 0u;

      for (core::uint32 i = 0; i < this->attachmentCount; ++i)
      {
        newAttachments[i] = other.pAttachments[i];
      }

      this->sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      this->pNext              = nullptr;
      this->flags              = 0u;

      this->pAttachments       = newAttachments;
      this->blendConstants[0]  = other.blendConstants[0];
      this->blendConstants[1]  = other.blendConstants[1];
      this->blendConstants[2]  = other.blendConstants[2];
      this->blendConstants[3]  = other.blendConstants[3];
      this->logicOpEnable      = other.logicOpEnable;
      this->logicOp            = other.logicOp;
    }
    return *this;
  }
  Color_Blend_State_Create_Info& operator=(Color_Blend_State_Create_Info&& other)
  {
    if (this != &other)
    {
      this->sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      this->pNext              = nullptr;
      this->flags              = 0u;

      this->pAttachments       = other.pAttachments;
      this->attachmentCount    = other.attachmentCount;
      this->blendConstants[0]  = other.blendConstants[0];
      this->blendConstants[1]  = other.blendConstants[1];
      this->blendConstants[2]  = other.blendConstants[2];
      this->blendConstants[3]  = other.blendConstants[3];
      this->logicOpEnable      = other.logicOpEnable;
      this->logicOp            = other.logicOp;

      other.pAttachments       = nullptr;
      other.attachmentCount    = 0u;
    }
    return *this;
  }
  ~Color_Blend_State_Create_Info()
  {
    if (this->attachmentCount > 0)
    {
      core::defaultCxxDeallocate(this->attachmentCount, this->pAttachments);
    }
  }
};

}
