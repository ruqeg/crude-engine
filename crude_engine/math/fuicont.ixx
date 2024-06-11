module;

export module crude.math.fuicont;

import <compare>;
export import crude.core.alias;

export namespace crude::math
{

struct Float1
{
  core::float32 x;

  Float1() = default;

  Float1(const Float1&) = default;
  Float1& operator=(const Float1&) = default;

  Float1(Float1&&) = default;
  Float1& operator=(Float1&&) = default;

  constexpr Float1(core::float32 x) noexcept
    :
    x(x) {}

  bool operator==(const Float1&) const = default;
  auto operator<=>(const Float1&) const = default;
};

struct alignas(16) Float1A : public Float1
{
  using Float1::Float1;
};

struct Float2
{
  core::float32 x;
  core::float32 y;

  Float2() = default;

  Float2(const Float2&) = default;
  Float2& operator=(const Float2&) = default;

  Float2(Float2&&) = default;
  Float2& operator=(Float2&&) = default;

  constexpr Float2(core::float32 x, core::float32 y) noexcept
    : 
    x(x), y(y) {}

  bool operator==(const Float2&) const = default;
  auto operator<=>(const Float2&) const = default;
};

struct alignas(16) Float2A : public Float2
{
  using Float2::Float2;
};

struct Float3
{
  core::float32 x;
  core::float32 y;
  core::float32 z;

  Float3() = default;

  Float3(const Float3&) = default;
  Float3& operator=(const Float3&) = default;

  Float3(Float3&&) = default;
  Float3& operator=(Float3&&) = default;

  constexpr Float3(core::float32 x, core::float32 y, core::float32 z) noexcept
    : x(x), y(y), z(z)
  {}

  bool operator==(const Float3&) const = default;
  auto operator<=>(const Float3&) const = default;
};

struct alignas(16) Float3A : public Float3
{
  using Float3::Float3;
};

struct Float4
{
  core::float32 x;
  core::float32 y;
  core::float32 z;
  core::float32 w;

  Float4() = default;

  Float4(const Float4&) = default;
  Float4& operator=(const Float4&) = default;

  Float4(Float4&&) = default;
  Float4& operator=(Float4&&) = default;

  constexpr Float4(core::float32 x, core::float32 y, core::float32 z, core::float32 w) noexcept
    : x(x), y(y), z(z), w(w)
  {}

  bool operator==(const Float4&) const = default;
  auto operator<=>(const Float4&) const = default;
};

struct alignas(16) Float4A : public Float4
{
  using Float4::Float4;
};

struct Float4x4
{
  union
  {
    struct
    {
      core::float32 _00;
      core::float32 _01;
      core::float32 _02;
      core::float32 _03;
      core::float32 _10;
      core::float32 _11;
      core::float32 _12;
      core::float32 _13;
      core::float32 _20;
      core::float32 _21;
      core::float32 _22;
      core::float32 _23;
      core::float32 _30;
      core::float32 _31;
      core::float32 _32;
      core::float32 _33;
    };
    core::float32 m[4][4];
  };

  Float4x4() = default;

  Float4x4(const Float4x4&) = default;
  Float4x4& operator=(const Float4x4&) = default;

  Float4x4(Float4x4&&) = default;
  Float4x4& operator=(Float4x4&&) = default;

  constexpr Float4x4(core::float32 m00, core::float32 m01, core::float32 m02, core::float32 m03,
                      core::float32 m10, core::float32 m11, core::float32 m12, core::float32 m13,
                      core::float32 m20, core::float32 m21, core::float32 m22, core::float32 m23,
                      core::float32 m30, core::float32 m31, core::float32 m32, core::float32 m33) noexcept
    : _00(m00), _01(m01), _02(m02), _03(m03)
    , _10(m10), _11(m11), _12(m12), _13(m13)
    , _20(m20), _21(m21), _22(m22), _23(m23)
    , _30(m30), _31(m31), _32(m32), _33(m33)
  {}

  bool operator==(const Float4x4&) const = default;
  auto operator<=>(const Float4x4&) const = default;
};

struct alignas(64) Float4x4A : public Float4x4
{
  using Float4x4::Float4x4;
};

struct Int1
{
  core::int32 x;

  Int1() = default;

  Int1(const Int1&) = default;
  Int1& operator=(const Int1&) = default;

  Int1(Int1&&) = default;
  Int1& operator=(Int1&&) = default;

  constexpr Int1(core::int32 x) noexcept
    :
    x(x) {}

  bool operator==(const Int1&) const = default;
  auto operator<=>(const Int1&) const = default;
};

struct alignas(16) Int1A : public Int1
{
  using Int1::Int1;
};

struct Int2
{
  core::int32 x;
  core::int32 y;

  Int2() = default;

  Int2(const Int2&) = default;
  Int2& operator=(const Int2&) = default;

  Int2(Int2&&) = default;
  Int2& operator=(Int2&&) = default;

  constexpr Int2(core::int32 x, core::int32 y) noexcept
    :
    x(x), y(y) {}

  bool operator==(const Int2&) const = default;
  auto operator<=>(const Int2&) const = default;
};

struct alignas(16) Int2A : public Int2
{
  using Int2::Int2;
};

struct Int3
{
  core::int32 x;
  core::int32 y;
  core::int32 z;

  Int3() = default;

  Int3(const Int3&) = default;
  Int3& operator=(const Int3&) = default;

  Int3(Int3&&) = default;
  Int3& operator=(Int3&&) = default;

  constexpr Int3(core::int32 x, core::int32 y, core::int32 z) noexcept
    :
    x(x), y(y), z(z) {}

  bool operator==(const Int3&) const = default;
  auto operator<=>(const Int3&) const = default;
};

struct alignas(16) Int3A : public Int3
{
  using Int3::Int3;
};

struct Int4
{
  core::int32 x;
  core::int32 y;
  core::int32 z;
  core::int32 w;

  Int4() = default;

  Int4(const Int4&) = default;
  Int4& operator=(const Int4&) = default;

  Int4(Int4&&) = default;
  Int4& operator=(Int4&&) = default;

  constexpr Int4(core::int32 x, core::int32 y, core::int32 z, core::int32 w) noexcept
    :
    x(x), y(y), z(z), w(w) {}

  bool operator==(const Int4&) const = default;
  auto operator<=>(const Int4&) const = default;
};

struct alignas(16) Int4A : public Int4
{
  using Int4::Int4;
};

struct Uint1
{
  core::uint32 x;

  Uint1() = default;

  Uint1(const Uint1&) = default;
  Uint1& operator=(const Uint1&) = default;

  Uint1(Uint1&&) = default;
  Uint1& operator=(Uint1&&) = default;

  constexpr Uint1(core::uint32 x) noexcept
    :
    x(x) {}

  bool operator==(const Uint1&) const = default;
  auto operator<=>(const Uint1&) const = default;
};

struct alignas(16) Uint1A : public Uint1
{
  using Uint1::Uint1;
};

struct Uint2
{
  core::uint32 x;
  core::uint32 y;

  Uint2() = default;

  Uint2(const Uint2&) = default;
  Uint2& operator=(const Uint2&) = default;

  Uint2(Uint2&&) = default;
  Uint2& operator=(Uint2&&) = default;

  constexpr Uint2(core::uint32 x, core::uint32 y) noexcept
    :
    x(x), y(y) {}

  bool operator==(const Uint2&) const = default;
  auto operator<=>(const Uint2&) const = default;
};

struct alignas(16) Uint2A : public Uint2
{
  using Uint2::Uint2;
};

struct Uint3
{
  core::uint32 x;
  core::uint32 y;
  core::uint32 z;

  Uint3() = default;

  Uint3(const Uint3&) = default;
  Uint3& operator=(const Uint3&) = default;

  Uint3(Uint3&&) = default;
  Uint3& operator=(Uint3&&) = default;

  constexpr Uint3(core::uint32 x, core::uint32 y, core::uint32 z) noexcept
    :
    x(x), y(y), z(z) {}

  bool operator==(const Uint3&) const = default;
  auto operator<=>(const Uint3&) const = default;
};

struct alignas(16) Uint3A : public Uint3
{
  using Uint3::Uint3;
};

struct Uint4
{
  core::uint32 x;
  core::uint32 y;
  core::uint32 z;
  core::uint32 w;

  Uint4() = default;

  Uint4(const Uint4&) = default;
  Uint4& operator=(const Uint4&) = default;

  Uint4(Uint4&&) = default;
  Uint4& operator=(Uint4&&) = default;

  constexpr Uint4(core::uint32 x, core::uint32 y, core::uint32 z, core::uint32 w) noexcept
    :
    x(x), y(y), z(z), w(w) {}

  bool operator==(const Uint4&) const = default;
  auto operator<=>(const Uint4&) const = default;
};

struct alignas(16) Uint4A : public Uint4
{
  using Uint4::Uint4;
};

}