module;

#include <vulkan/vulkan.h>

export module crude.graphics.attachment_load_store_op;

import crude.core.alias;

export namespace crude::graphics
{

struct Attachment_Load_Store_OP
{
  VkAttachmentLoadOp   load;
  VkAttachmentStoreOp  store;
};

namespace attachment_op
{
constexpr Attachment_Load_Store_OP gClearStore{ VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE };
constexpr Attachment_Load_Store_OP gDontCare{ VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE };
}

}