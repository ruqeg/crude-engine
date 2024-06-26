module;

#include <vulkan/vulkan.h>

export module crude.graphics.attachment_description;

export import crude.graphics.format;
export import crude.graphics.attachment_load_store_op;
export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Swap_Chain;
class Depth_Stencil_Attachment;
class Color_Attachment;

class Attachment_Description : public VkAttachmentDescription
{
public:
  struct Initialize_Info;
public:
  explicit Attachment_Description(const Initialize_Info& info);
};

class Color_Attachment_Description : public Attachment_Description
{
public:
  struct Initialize_Info;
public:
  explicit Color_Attachment_Description(const Initialize_Info& info);
};

class Depth_Attachment_Description : public Attachment_Description
{
public:
  struct Initialize_Info;
public:
  explicit Depth_Attachment_Description(const Initialize_Info& info);
};

class Swapchain_Attachment_Description : public Attachment_Description
{
public:
  struct Initialize_Info;
public:
  explicit Swapchain_Attachment_Description(const Initialize_Info& info);
};

struct Attachment_Description::Initialize_Info
{
  VkFormat                  format;
  VkSampleCountFlagBits     samples;
  Attachment_Load_Store_OP  colorOp;
  Attachment_Load_Store_OP  stenicilOp;
  VkImageLayout             initialLayout;
  VkImageLayout             finalLayout;
};

struct Color_Attachment_Description::Initialize_Info
{
  core::shared_ptr<Color_Attachment>  attachment;
  Attachment_Load_Store_OP            colorOp = attachment_op::gClearStore;
  Attachment_Load_Store_OP            stenicilOp = attachment_op::gDontCare;
};

struct Depth_Attachment_Description::Initialize_Info
{
  core::shared_ptr<Depth_Stencil_Attachment> attachment;
};

struct Swapchain_Attachment_Description::Initialize_Info
{
  core::shared_ptr<Swap_Chain>  swapchain;
  Attachment_Load_Store_OP      colorOp;
};

}