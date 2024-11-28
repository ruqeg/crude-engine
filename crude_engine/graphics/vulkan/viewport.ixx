module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.viewport;

export import crude.core.alias;

export namespace crude::gfx::vk
{

struct Viewport : public VkViewport
{
public:
  struct Initialize
  {
    core::float32  x;
    core::float32  y;
    core::float32  width;
    core::float32  height;
    core::float32  minDepth;
    core::float32  maxDepth;
  };
public:
  explicit Viewport(const Initialize& info)
  {
    this->x         = info.x;
    this->y         = info.y;
    this->width     = info.width;
    this->height    = info.height;
    this->minDepth  = info.minDepth;
    this->maxDepth  = info.maxDepth;
  }
};

}