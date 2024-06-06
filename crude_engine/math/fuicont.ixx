module;

#include <compare>

export module crude.math.fuicont;

import crude.core.alias;

export namespace crude::math
{

struct FLOAT1
{
  core::float32 x;

  FLOAT1() = default;

  FLOAT1(const FLOAT1&) = default;
  FLOAT1& operator=(const FLOAT1&) = default;

  FLOAT1(FLOAT1&&) = default;
  FLOAT1& operator=(FLOAT1&&) = default;

  constexpr FLOAT1(core::float32 x) noexcept
    :
    x(x) {}

  bool operator==(const FLOAT1&) const = default;
  auto operator<=>(const FLOAT1&) const = default;
};

struct alignas(16) FLOAT1A : public FLOAT1
{
  using FLOAT1::FLOAT1;
};

struct FLOAT2
{
  core::float32 x;
  core::float32 y;

  FLOAT2() = default;

  FLOAT2(const FLOAT2&) = default;
  FLOAT2& operator=(const FLOAT2&) = default;

  FLOAT2(FLOAT2&&) = default;
  FLOAT2& operator=(FLOAT2&&) = default;

  constexpr FLOAT2(core::float32 x, core::float32 y) noexcept
    : 
    x(x), y(y) {}

  bool operator==(const FLOAT2&) const = default;
  auto operator<=>(const FLOAT2&) const = default;
};

struct alignas(16) FLOAT2A : public FLOAT2
{
  using FLOAT2::FLOAT2;
};

struct FLOAT3
{
  core::float32 x;
  core::float32 y;
  core::float32 z;

  FLOAT3() = default;

  FLOAT3(const FLOAT3&) = default;
  FLOAT3& operator=(const FLOAT3&) = default;

  FLOAT3(FLOAT3&&) = default;
  FLOAT3& operator=(FLOAT3&&) = default;

  constexpr FLOAT3(core::float32 x, core::float32 y, core::float32 z) noexcept
    : x(x), y(y), z(z)
  {}

  bool operator==(const FLOAT3&) const = default;
  auto operator<=>(const FLOAT3&) const = default;
};

struct alignas(16) FLOAT3A : public FLOAT3
{
  using FLOAT3::FLOAT3;
};

struct FLOAT4
{
  core::float32 x;
  core::float32 y;
  core::float32 z;
  core::float32 w;

  FLOAT4() = default;

  FLOAT4(const FLOAT4&) = default;
  FLOAT4& operator=(const FLOAT4&) = default;

  FLOAT4(FLOAT4&&) = default;
  FLOAT4& operator=(FLOAT4&&) = default;

  constexpr FLOAT4(core::float32 x, core::float32 y, core::float32 z, core::float32 w) noexcept
    : x(x), y(y), z(z), w(w)
  {}

  bool operator==(const FLOAT4&) const = default;
  auto operator<=>(const FLOAT4&) const = default;
};

struct alignas(16) FLOAT4A : public FLOAT4
{
  using FLOAT4::FLOAT4;
};

struct FLOAT4X4
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

  FLOAT4X4() = default;

  FLOAT4X4(const FLOAT4X4&) = default;
  FLOAT4X4& operator=(const FLOAT4X4&) = default;

  FLOAT4X4(FLOAT4X4&&) = default;
  FLOAT4X4& operator=(FLOAT4X4&&) = default;

  constexpr FLOAT4X4(core::float32 m00, core::float32 m01, core::float32 m02, core::float32 m03,
                      core::float32 m10, core::float32 m11, core::float32 m12, core::float32 m13,
                      core::float32 m20, core::float32 m21, core::float32 m22, core::float32 m23,
                      core::float32 m30, core::float32 m31, core::float32 m32, core::float32 m33) noexcept
    : _00(m00), _01(m01), _02(m02), _03(m03)
    , _10(m10), _11(m11), _12(m12), _13(m13)
    , _20(m20), _21(m21), _22(m22), _23(m23)
    , _30(m30), _31(m31), _32(m32), _33(m33)
  {}

  bool operator==(const FLOAT4X4&) const = default;
  auto operator<=>(const FLOAT4X4&) const = default;
};

struct alignas(64) FLOAT4X4A : public FLOAT4X4
{
  using FLOAT4X4::FLOAT4X4;
};

struct INT1
{
  core::int32 x;

  INT1() = default;

  INT1(const INT1&) = default;
  INT1& operator=(const INT1&) = default;

  INT1(INT1&&) = default;
  INT1& operator=(INT1&&) = default;

  constexpr INT1(core::int32 x) noexcept
    :
    x(x) {}

  bool operator==(const INT1&) const = default;
  auto operator<=>(const INT1&) const = default;
};

struct alignas(16) INT1A : public INT1
{
  using INT1::INT1;
};

struct INT2
{
  core::int32 x;
  core::int32 y;

  INT2() = default;

  INT2(const INT2&) = default;
  INT2& operator=(const INT2&) = default;

  INT2(INT2&&) = default;
  INT2& operator=(INT2&&) = default;

  constexpr INT2(core::int32 x, core::int32 y) noexcept
    :
    x(x), y(y) {}

  bool operator==(const INT2&) const = default;
  auto operator<=>(const INT2&) const = default;
};

struct alignas(16) INT2A : public INT2
{
  using INT2::INT2;
};

struct INT3
{
  core::int32 x;
  core::int32 y;
  core::int32 z;

  INT3() = default;

  INT3(const INT3&) = default;
  INT3& operator=(const INT3&) = default;

  INT3(INT3&&) = default;
  INT3& operator=(INT3&&) = default;

  constexpr INT3(core::int32 x, core::int32 y, core::int32 z) noexcept
    :
    x(x), y(y), z(z) {}

  bool operator==(const INT3&) const = default;
  auto operator<=>(const INT3&) const = default;
};

struct alignas(16) INT3A : public INT3
{
  using INT3::INT3;
};

struct INT4
{
  core::int32 x;
  core::int32 y;
  core::int32 z;
  core::int32 w;

  INT4() = default;

  INT4(const INT4&) = default;
  INT4& operator=(const INT4&) = default;

  INT4(INT4&&) = default;
  INT4& operator=(INT4&&) = default;

  constexpr INT4(core::int32 x, core::int32 y, core::int32 z, core::int32 w) noexcept
    :
    x(x), y(y), z(z), w(w) {}

  bool operator==(const INT4&) const = default;
  auto operator<=>(const INT4&) const = default;
};

struct alignas(16) INT4A : public INT4
{
  using INT4::INT4;
};

struct UINT1
{
  core::uint32 x;

  UINT1() = default;

  UINT1(const UINT1&) = default;
  UINT1& operator=(const UINT1&) = default;

  UINT1(UINT1&&) = default;
  UINT1& operator=(UINT1&&) = default;

  constexpr UINT1(core::uint32 x) noexcept
    :
    x(x) {}

  bool operator==(const UINT1&) const = default;
  auto operator<=>(const UINT1&) const = default;
};

struct alignas(16) UINT1A : public UINT1
{
  using UINT1::UINT1;
};

struct UINT2
{
  core::uint32 x;
  core::uint32 y;

  UINT2() = default;

  UINT2(const UINT2&) = default;
  UINT2& operator=(const UINT2&) = default;

  UINT2(UINT2&&) = default;
  UINT2& operator=(UINT2&&) = default;

  constexpr UINT2(core::uint32 x, core::uint32 y) noexcept
    :
    x(x), y(y) {}

  bool operator==(const UINT2&) const = default;
  auto operator<=>(const UINT2&) const = default;
};

struct alignas(16) UINT2A : public UINT2
{
  using UINT2::UINT2;
};

struct UINT3
{
  core::uint32 x;
  core::uint32 y;
  core::uint32 z;

  UINT3() = default;

  UINT3(const UINT3&) = default;
  UINT3& operator=(const UINT3&) = default;

  UINT3(UINT3&&) = default;
  UINT3& operator=(UINT3&&) = default;

  constexpr UINT3(core::uint32 x, core::uint32 y, core::uint32 z) noexcept
    :
    x(x), y(y), z(z) {}

  bool operator==(const UINT3&) const = default;
  auto operator<=>(const UINT3&) const = default;
};

struct alignas(16) UINT3A : public UINT3
{
  using UINT3::UINT3;
};

struct UINT4
{
  core::uint32 x;
  core::uint32 y;
  core::uint32 z;
  core::uint32 w;

  UINT4() = default;

  UINT4(const UINT4&) = default;
  UINT4& operator=(const UINT4&) = default;

  UINT4(UINT4&&) = default;
  UINT4& operator=(UINT4&&) = default;

  constexpr UINT4(core::uint32 x, core::uint32 y, core::uint32 z, core::uint32 w) noexcept
    :
    x(x), y(y), z(z), w(w) {}

  bool operator==(const UINT4&) const = default;
  auto operator<=>(const UINT4&) const = default;
};

struct alignas(16) UINT4A : public UINT4
{
  using UINT4::UINT4;
};

}