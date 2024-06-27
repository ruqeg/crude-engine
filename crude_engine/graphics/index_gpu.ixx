module;

#include <vulkan/vulkan.h>

export module crude.graphics.index_gpu;

export import crude.core.alias;
export import crude.math.fuicont;
export import crude.scene.index_cpu;

export namespace crude::graphics
{

#pragma pack(push, 1)
struct Index_Triangle_GPU
{
  Index_Triangle_GPU() = default;
  Index_Triangle_GPU(const scene::Index_Triangle_CPU& indexCpu)
    : p0(indexCpu.p0)
    , p1(indexCpu.p1)
    , p2(indexCpu.p2)
  {}
  Index_Triangle_GPU(core::uint16 p0, core::uint16 p1, core::uint16 p2)
    : p0(p0)
    , p1(p1)
    , p2(p2) {}
  explicit operator math::Uint3() const noexcept { return math::Uint3(p0, p1, p2); }
  static VkIndexType getType() { return VK_INDEX_TYPE_UINT16; }
  core::uint16 p0;
  core::uint16 p1;
  core::uint16 p2;
};
#pragma pack(pop)

}