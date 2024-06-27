module;

export module crude.scene.index_cpu;

export import crude.math.fuicont;

export namespace crude::scene
{

struct Index_Triangle_CPU
{
  Index_Triangle_CPU() = default;
  Index_Triangle_CPU(core::uint32 p0, core::uint32 p1, core::uint32 p2)
    : p0(p0), p1(p1), p2(p2) {}
  explicit operator math::Uint3() const noexcept { return math::Uint3(p0, p1, p2); }
  core::uint32 p0;
  core::uint32 p1;
  core::uint32 p2;
};

}