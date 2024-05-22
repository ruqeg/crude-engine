module;

#include <xmmintrin.h>
#include <emmintrin.h>

export module crude.math.vector;

import crude.core.alias;
import crude.math.scalar;
import crude.math.constants;

namespace crude::math
{

struct __vector4
{
  union
  {
    core::float32   vector4_f32[4];
    core::uint32    vector4_u32[4];
  };
};

}

export namespace crude::math
{
// !TODO complete SIMD (in the next life) or just switch to DirectXMath
#if defined(_CRUDE_SSE_INTRINSICS) && !defined(_CRUDE_NO_INTRINSICS)
#error "!TODO"
using Vector = __m128;
#elif defined(_CRUDE_ARM_NEON_INTRINSICS) && !defined(_CRUDE_NO_INTRINSICS)
#error "!TODO"
using Vector = float32x4_t;
#else
using Vector = __vector4;
#endif

class SysVector
{
public:
  using CVector = const Vector&;

public:
  SysVector() = delete;

public:
  static Vector setInt(core::uint32 x, core::uint32 y, core::uint32 z, core::uint32 w) noexcept;
  static Vector fillInt(core::uint32 value) noexcept;
  static Vector trueInt() noexcept;
  static Vector falseInt() noexcept;

  static core::uint32 getIntX(CVector v) noexcept;
  static core::uint32 getIntY(CVector v) noexcept;
  static core::uint32 getIntZ(CVector v) noexcept;
  static core::uint32 getIntW(CVector v) noexcept;

  static Vector setIntX(CVector v, core::uint32 x) noexcept;
  static Vector setIntY(CVector v, core::uint32 y) noexcept;
  static Vector setIntZ(CVector v, core::uint32 z) noexcept;
  static Vector setIntW(CVector v, core::uint32 w) noexcept;

  static Vector equalInt(CVector v1, CVector v2) noexcept;
  static Vector notEqualInt(CVector v1, CVector v2) noexcept;

  static Vector andInt(CVector v1, CVector v2) noexcept;
  static Vector orInt(CVector v1, CVector v2) noexcept;
  static Vector norInt(CVector v1, CVector v2) noexcept;
  static Vector xorInt(CVector v1, CVector v2) noexcept;

  static Vector select(CVector v1, CVector v2, CVector control) noexcept;

  static Vector zero() noexcept;
  static Vector set(core::float32 x, core::float32 y, core::float32 z, core::float32 w) noexcept;
  static Vector fill(core::float32 value) noexcept;
  static Vector splatX(CVector v) noexcept;
  static Vector splatY(CVector v) noexcept;
  static Vector splatZ(CVector v) noexcept;
  static Vector splatW(CVector v) noexcept;
  static Vector splatOne() noexcept;
  static Vector splatInfinity() noexcept;
  static Vector splatQNaN() noexcept;
  static Vector splatEpsilon() noexcept;

  static core::float32 getX(CVector v) noexcept;
  static core::float32 getY(CVector v) noexcept;
  static core::float32 getZ(CVector v) noexcept;
  static core::float32 getW(CVector v) noexcept;

  static Vector setX(CVector v, core::float32 x) noexcept;
  static Vector setY(CVector v, core::float32 y) noexcept;
  static Vector setZ(CVector v, core::float32 z) noexcept;
  static Vector setW(CVector v, core::float32 w) noexcept;

  static Vector equal(CVector v1, CVector v2) noexcept;
  static Vector nearEqual(CVector v1, CVector v2, CVector vepsilon) noexcept;
  static Vector notEqual(CVector v1, CVector v2) noexcept;
  static Vector greater(CVector v1, CVector v2) noexcept;
  static Vector greaterOrEqual(CVector v1, CVector v2) noexcept;
  static Vector less(CVector v1, CVector v2) noexcept;
  static Vector lessOrEqual(CVector v1, CVector v2) noexcept;
  static Vector inBounds(CVector v, CVector vbounds) noexcept;

  static Vector isNaN(CVector v) noexcept;
  static Vector isInfinite(CVector v) noexcept;

  static Vector min(CVector v1, CVector v2) noexcept;
  static Vector max(CVector v1, CVector v2) noexcept;
  static Vector round(CVector v) noexcept;
  static Vector floor(CVector v) noexcept;
  static Vector ceil(CVector v) noexcept;
  static Vector trunc(CVector v) noexcept;
  static Vector clamp(CVector v, CVector vmin, CVector vmax) noexcept;

  static Vector negate(CVector v) noexcept;
  static Vector add(CVector v1, CVector v2) noexcept;
  static Vector subtract(CVector v1, CVector v2) noexcept;
  static Vector multiply(CVector v1, CVector v2) noexcept;
  static Vector multiplyAdd(CVector v1, CVector v2, CVector v3) noexcept;
  static Vector divide(CVector v1, CVector v2) noexcept;
  static Vector divideAdd(CVector v1, CVector v2, CVector v3) noexcept;
  static Vector scale(CVector v, core::float32 s) noexcept;
  static Vector sqrt(CVector v) noexcept;
  static Vector exp2(CVector v) noexcept;
  static Vector exp(CVector v) noexcept;
  static Vector log2(CVector v) noexcept;
  static Vector log10(CVector v) noexcept;
  static Vector log(CVector v) noexcept;
  static Vector pow(CVector v1, CVector v2) noexcept;
  static Vector abs(CVector v) noexcept;
  static Vector cos(CVector v) noexcept;
  static Vector sin(CVector v) noexcept;
  static Vector tan(CVector v) noexcept;
  static Vector arcCos(CVector v) noexcept;
  static Vector arcSin(CVector v) noexcept;
  static Vector arcTan(CVector v) noexcept;

  static Vector covector1(CVector v, CVector e1) noexcept;
  static Vector dot1(CVector v1, CVector v2, CVector e1, Vector* cov = nullptr) noexcept;
  static Vector dot1(CVector v1, CVector v2) noexcept;
  static Vector length1(CVector v) noexcept;
  static Vector lengthSq1(CVector v) noexcept;
  static Vector normalize1(CVector v) noexcept;
  static Vector cos1(CVector v1, CVector v2) noexcept;
  static Vector project1(CVector v1, CVector v2) noexcept;
  static Vector reject1(CVector v1, CVector v2) noexcept;

  static Vector covector2(CVector v, CVector e1, CVector e2) noexcept;
  static Vector dot2(CVector v1, CVector v2, CVector e1, CVector e2, Vector* cov = nullptr) noexcept;
  static Vector dot2(CVector v1, CVector v2) noexcept;
  static Vector length2(CVector v) noexcept;
  static Vector lengthSq2(CVector v) noexcept;
  static Vector normalize2(CVector v) noexcept;
  static Vector cos2(CVector v1, CVector v2) noexcept;
  static Vector project2(CVector v1, CVector v2) noexcept;
  static Vector reject2(CVector v1, CVector v2) noexcept;

  static Vector covector3(CVector v, CVector e1, CVector e2, CVector e3) noexcept;
  static Vector dot3(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3, Vector* cov = nullptr) noexcept;
  static Vector dot3(CVector v1, CVector v2) noexcept;
  static Vector length3(CVector v) noexcept;
  static Vector lengthSq3(CVector v) noexcept;
  static Vector normalize3(CVector v) noexcept;
  static Vector cos3(CVector v1, CVector v2) noexcept;
  static Vector tripleProduct3(CVector v1, CVector v2, CVector v3, CVector e1, CVector e2, CVector e3) noexcept;
  static Vector cross3(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3) noexcept;
  static Vector tripleProduct3(CVector v1, CVector v2, CVector v3) noexcept;
  static Vector cross3(CVector v1, CVector v2) noexcept;
  static Vector project3(CVector v1, CVector v2) noexcept;
  static Vector reject3(CVector v1, CVector v2) noexcept;

  static Vector covector4(CVector v, CVector e1, CVector e2, CVector e3, CVector e4) noexcept;
  static Vector dot4(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3, CVector e4, Vector* cov = nullptr) noexcept;
  static Vector dot4(CVector v1, CVector v2) noexcept;
  static Vector length4(CVector v) noexcept;
  static Vector lengthSq4(CVector v) noexcept;
  static Vector normalize4(CVector v) noexcept;
  static Vector cos4(CVector v1, CVector v2) noexcept;
  static Vector project4(CVector v1, CVector v2) noexcept;
  static Vector reject4(CVector v1, CVector v2) noexcept;
};

}