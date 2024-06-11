module;

#include <vulkan/vulkan.hpp>

export module crude.scene.index;

export import crude.math.fuicont;

export namespace crude::scene
{

#pragma pack(push, 1)
struct Index_Triangle_GPU
{
  Index_Triangle_GPU() = default;
  Index_Triangle_GPU(core::uint16 p0, core::uint16 p1, core::uint16 p2)
    : p0(p0), p1(p1), p2(p2) {}
  explicit operator math::Uint3() const noexcept { return math::Uint3(p0, p1, p2); }
  core::uint16 p0;
  core::uint16 p1;
  core::uint16 p2;
};
#pragma pack(pop)

struct Index_Triangle_CPU
{
  Index_Triangle_CPU() = default;
  Index_Triangle_CPU(core::uint32 p0, core::uint32 p1, core::uint32 p2)
    : p0(p0), p1(p1), p2(p2) {}
  explicit operator math::Uint3() const noexcept { return math::Uint3(p0, p1, p2); }
  explicit operator Index_Triangle_GPU() const noexcept { return Index_Triangle_GPU(p0, p1, p2); }
  core::uint32 p0;
  core::uint32 p1;
  core::uint32 p2;
};

}