module;

#include <compare>

export module crude_engine.math.fuicont;

import crude_engine.core.alias;

export namespace crude_engine
{

struct MFLOAT1
{
  float32 x;

  MFLOAT1() = default;

  MFLOAT1(const MFLOAT1&) = default;
  MFLOAT1& operator=(const MFLOAT1&) = default;

  MFLOAT1(MFLOAT1&&) = default;
  MFLOAT1& operator=(MFLOAT1&&) = default;

  constexpr MFLOAT1(float32 x) noexcept
    :
    x(x) {}

  bool operator==(const MFLOAT1&) const = default;
  auto operator<=>(const MFLOAT1&) const = default;
};

struct alignas(16) MFLOAT1A : public MFLOAT1
{
  using MFLOAT1::MFLOAT1;
};

struct MFLOAT2
{
  float32 x;
  float32 y;

  MFLOAT2() = default;

  MFLOAT2(const MFLOAT2&) = default;
  MFLOAT2& operator=(const MFLOAT2&) = default;

  MFLOAT2(MFLOAT2&&) = default;
  MFLOAT2& operator=(MFLOAT2&&) = default;

  constexpr MFLOAT2(float32 x, float32 y) noexcept
    : 
    x(x), y(y) {}

  bool operator==(const MFLOAT2&) const = default;
  auto operator<=>(const MFLOAT2&) const = default;
};

struct alignas(16) MFLOAT2A : public MFLOAT2
{
  using MFLOAT2::MFLOAT2;
};

struct MFLOAT3
{
  float32 x;
  float32 y;
  float32 z;

  MFLOAT3() = default;

  MFLOAT3(const MFLOAT3&) = default;
  MFLOAT3& operator=(const MFLOAT3&) = default;

  MFLOAT3(MFLOAT3&&) = default;
  MFLOAT3& operator=(MFLOAT3&&) = default;

  constexpr MFLOAT3(float32 x, float32 y, float32 z) noexcept
    : x(x), y(y), z(z)
  {}

  bool operator==(const MFLOAT3&) const = default;
  auto operator<=>(const MFLOAT3&) const = default;
};

struct alignas(16) MFLOAT3A : public MFLOAT3
{
  using MFLOAT3::MFLOAT3;
};

struct MFLOAT4
{
  float32 x;
  float32 y;
  float32 z;
  float32 w;

  MFLOAT4() = default;

  MFLOAT4(const MFLOAT4&) = default;
  MFLOAT4& operator=(const MFLOAT4&) = default;

  MFLOAT4(MFLOAT4&&) = default;
  MFLOAT4& operator=(MFLOAT4&&) = default;

  constexpr MFLOAT4(float32 x, float32 y, float32 z, float32 w) noexcept
    : x(x), y(y), z(z), w(w)
  {}

  bool operator==(const MFLOAT4&) const = default;
  auto operator<=>(const MFLOAT4&) const = default;
};

struct alignas(16) MFLOAT4A : public MFLOAT4
{
  using MFLOAT4::MFLOAT4;
};

struct MINT1
{
  int32 x;

  MINT1() = default;

  MINT1(const MINT1&) = default;
  MINT1& operator=(const MINT1&) = default;

  MINT1(MINT1&&) = default;
  MINT1& operator=(MINT1&&) = default;

  constexpr MINT1(int32 x) noexcept
    :
    x(x) {}

  bool operator==(const MINT1&) const = default;
  auto operator<=>(const MINT1&) const = default;
};

struct alignas(16) MINT1A : public MINT1
{
  using MINT1::MINT1;
};

struct MINT2
{
  int32 x;
  int32 y;

  MINT2() = default;

  MINT2(const MINT2&) = default;
  MINT2& operator=(const MINT2&) = default;

  MINT2(MINT2&&) = default;
  MINT2& operator=(MINT2&&) = default;

  constexpr MINT2(int32 x, int32 y) noexcept
    :
    x(x), y(y) {}

  bool operator==(const MINT2&) const = default;
  auto operator<=>(const MINT2&) const = default;
};

struct alignas(16) MINT2A : public MINT2
{
  using MINT2::MINT2;
};

struct MINT3
{
  int32 x;
  int32 y;
  int32 z;

  MINT3() = default;

  MINT3(const MINT3&) = default;
  MINT3& operator=(const MINT3&) = default;

  MINT3(MINT3&&) = default;
  MINT3& operator=(MINT3&&) = default;

  constexpr MINT3(int32 x, int32 y, int32 z) noexcept
    :
    x(x), y(y), z(z) {}

  bool operator==(const MINT3&) const = default;
  auto operator<=>(const MINT3&) const = default;
};

struct alignas(16) MINT3A : public MINT3
{
  using MINT3::MINT3;
};

struct MINT4
{
  int32 x;
  int32 y;
  int32 z;
  int32 w;

  MINT4() = default;

  MINT4(const MINT4&) = default;
  MINT4& operator=(const MINT4&) = default;

  MINT4(MINT4&&) = default;
  MINT4& operator=(MINT4&&) = default;

  constexpr MINT4(int32 x, int32 y, int32 z, int32 w) noexcept
    :
    x(x), y(y), z(z), w(w) {}

  bool operator==(const MINT4&) const = default;
  auto operator<=>(const MINT4&) const = default;
};

struct alignas(16) MINT4A : public MINT4
{
  using MINT4::MINT4;
};

struct MUINT1
{
  uint32 x;

  MUINT1() = default;

  MUINT1(const MUINT1&) = default;
  MUINT1& operator=(const MUINT1&) = default;

  MUINT1(MUINT1&&) = default;
  MUINT1& operator=(MUINT1&&) = default;

  constexpr MUINT1(uint32 x) noexcept
    :
    x(x) {}

  bool operator==(const MUINT1&) const = default;
  auto operator<=>(const MUINT1&) const = default;
};

struct alignas(16) MUINT1A : public MUINT1
{
  using MUINT1::MUINT1;
};

struct MUINT2
{
  uint32 x;
  uint32 y;

  MUINT2() = default;

  MUINT2(const MUINT2&) = default;
  MUINT2& operator=(const MUINT2&) = default;

  MUINT2(MUINT2&&) = default;
  MUINT2& operator=(MUINT2&&) = default;

  constexpr MUINT2(uint32 x, uint32 y) noexcept
    :
    x(x), y(y) {}

  bool operator==(const MUINT2&) const = default;
  auto operator<=>(const MUINT2&) const = default;
};

struct alignas(16) MUINT2A : public MUINT2
{
  using MUINT2::MUINT2;
};

struct MUINT3
{
  uint32 x;
  uint32 y;
  uint32 z;

  MUINT3() = default;

  MUINT3(const MUINT3&) = default;
  MUINT3& operator=(const MUINT3&) = default;

  MUINT3(MUINT3&&) = default;
  MUINT3& operator=(MUINT3&&) = default;

  constexpr MUINT3(uint32 x, uint32 y, uint32 z) noexcept
    :
    x(x), y(y), z(z) {}

  bool operator==(const MUINT3&) const = default;
  auto operator<=>(const MUINT3&) const = default;
};

struct alignas(16) MUINT3A : public MUINT3
{
  using MUINT3::MUINT3;
};

struct MUINT4
{
  uint32 x;
  uint32 y;
  uint32 z;
  uint32 w;

  MUINT4() = default;

  MUINT4(const MUINT4&) = default;
  MUINT4& operator=(const MUINT4&) = default;

  MUINT4(MUINT4&&) = default;
  MUINT4& operator=(MUINT4&&) = default;

  constexpr MUINT4(uint32 x, uint32 y, uint32 z, uint32 w) noexcept
    :
    x(x), y(y), z(z), w(w) {}

  bool operator==(const MUINT4&) const = default;
  auto operator<=>(const MUINT4&) const = default;
};

struct alignas(16) MUINT4A : public MUINT4
{
  using MUINT4::MUINT4;
};

}