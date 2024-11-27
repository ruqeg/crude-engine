module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.scissor;

export namespace crude::gfx::vk
{

struct Scissor : public VkRect2D
{
public:
  struct Initialize
  {
    VkOffset2D  offset;
    VkExtent2D  extent;
  };
  explicit Scissor(const Initialize& info)
  {
    this->offset = info.offset;
    this->extent = info.extent;
  }
};

}