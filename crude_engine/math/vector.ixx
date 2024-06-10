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

// !TODO complete SIMD (in the next life) or just switch to DirectXMath
#if defined(_CRUDE_SSE_INTRINSICS) && !defined(_CRUDE_NO_INTRINSICS)
#error "!TODO"
using Vector = __m128;
#elif defined(_CRUDE_ARM_NEON_INTRINSICS) && !defined(_CRUDE_NO_INTRINSICS)
#error "!TODO"
using Vector = float32x4_t;
#else
export using Vector = __vector4;
#endif

export using CVector = const Vector&;

export namespace svector
{

Vector setInt(core::uint32 x, core::uint32 y, core::uint32 z, core::uint32 w) noexcept;
Vector fillInt(core::uint32 value) noexcept;
Vector trueInt() noexcept;
Vector falseInt() noexcept;

core::uint32 getIntX(CVector v) noexcept;
core::uint32 getIntY(CVector v) noexcept;
core::uint32 getIntZ(CVector v) noexcept;
core::uint32 getIntW(CVector v) noexcept;

Vector setIntX(CVector v, core::uint32 x) noexcept;
Vector setIntY(CVector v, core::uint32 y) noexcept;
Vector setIntZ(CVector v, core::uint32 z) noexcept;
Vector setIntW(CVector v, core::uint32 w) noexcept;

Vector equalInt(CVector v1, CVector v2) noexcept;
Vector notEqualInt(CVector v1, CVector v2) noexcept;

Vector andInt(CVector v1, CVector v2) noexcept;
Vector orInt(CVector v1, CVector v2) noexcept;
Vector norInt(CVector v1, CVector v2) noexcept;
Vector xorInt(CVector v1, CVector v2) noexcept;

Vector select(CVector v1, CVector v2, CVector control) noexcept;

Vector zero() noexcept;
Vector set(core::float32 x, core::float32 y, core::float32 z, core::float32 w) noexcept;
Vector fill(core::float32 value) noexcept;
Vector splatX(CVector v) noexcept;
Vector splatY(CVector v) noexcept;
Vector splatZ(CVector v) noexcept;
Vector splatW(CVector v) noexcept;
Vector splatOne() noexcept;
Vector splatInfinity() noexcept;
Vector splatQNaN() noexcept;
Vector splatEpsilon() noexcept;

core::float32 getX(CVector v) noexcept;
core::float32 getY(CVector v) noexcept;
core::float32 getZ(CVector v) noexcept;
core::float32 getW(CVector v) noexcept;

Vector setX(CVector v, core::float32 x) noexcept;
Vector setY(CVector v, core::float32 y) noexcept;
Vector setZ(CVector v, core::float32 z) noexcept;
Vector setW(CVector v, core::float32 w) noexcept;

Vector equal(CVector v1, CVector v2) noexcept;
Vector nearEqual(CVector v1, CVector v2, CVector vepsilon) noexcept;
Vector notEqual(CVector v1, CVector v2) noexcept;
Vector greater(CVector v1, CVector v2) noexcept;
Vector greaterOrEqual(CVector v1, CVector v2) noexcept;
Vector less(CVector v1, CVector v2) noexcept;
Vector lessOrEqual(CVector v1, CVector v2) noexcept;
Vector inBounds(CVector v, CVector vbounds) noexcept;

Vector isNaN(CVector v) noexcept;
Vector isInfinite(CVector v) noexcept;

Vector min(CVector v1, CVector v2) noexcept;
Vector max(CVector v1, CVector v2) noexcept;
Vector round(CVector v) noexcept;
Vector floor(CVector v) noexcept;
Vector ceil(CVector v) noexcept;
Vector trunc(CVector v) noexcept;
Vector clamp(CVector v, CVector vmin, CVector vmax) noexcept;

Vector negate(CVector v) noexcept;
Vector add(CVector v1, CVector v2) noexcept;
Vector subtract(CVector v1, CVector v2) noexcept;
Vector multiply(CVector v1, CVector v2) noexcept;
Vector multiplyAdd(CVector v1, CVector v2, CVector v3) noexcept;
Vector divide(CVector v1, CVector v2) noexcept;
Vector divideAdd(CVector v1, CVector v2, CVector v3) noexcept;
Vector scale(CVector v, core::float32 s) noexcept;
Vector sqrt(CVector v) noexcept;
Vector exp2(CVector v) noexcept;
Vector exp(CVector v) noexcept;
Vector log2(CVector v) noexcept;
Vector log10(CVector v) noexcept;
Vector log(CVector v) noexcept;
Vector pow(CVector v1, CVector v2) noexcept;
Vector abs(CVector v) noexcept;
Vector cos(CVector v) noexcept;
Vector sin(CVector v) noexcept;
Vector tan(CVector v) noexcept;
Vector arcCos(CVector v) noexcept;
Vector arcSin(CVector v) noexcept;
Vector arcTan(CVector v) noexcept;

Vector covector1(CVector v, CVector e1) noexcept;
Vector dot1(CVector v1, CVector v2, CVector e1, Vector* cov = nullptr) noexcept;
Vector dot1(CVector v1, CVector v2) noexcept;
Vector length1(CVector v) noexcept;
Vector lengthSq1(CVector v) noexcept;
Vector normalize1(CVector v) noexcept;
Vector cos1(CVector v1, CVector v2) noexcept;
Vector project1(CVector v1, CVector v2) noexcept;
Vector reject1(CVector v1, CVector v2) noexcept;

Vector covector2(CVector v, CVector e1, CVector e2) noexcept;
Vector dot2(CVector v1, CVector v2, CVector e1, CVector e2, Vector* cov = nullptr) noexcept;
Vector dot2(CVector v1, CVector v2) noexcept;
Vector length2(CVector v) noexcept;
Vector lengthSq2(CVector v) noexcept;
Vector normalize2(CVector v) noexcept;
Vector cos2(CVector v1, CVector v2) noexcept;
Vector project2(CVector v1, CVector v2) noexcept;
Vector reject2(CVector v1, CVector v2) noexcept;

Vector covector3(CVector v, CVector e1, CVector e2, CVector e3) noexcept;
Vector dot3(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3, Vector* cov = nullptr) noexcept;
Vector dot3(CVector v1, CVector v2) noexcept;
Vector length3(CVector v) noexcept;
Vector lengthSq3(CVector v) noexcept;
Vector normalize3(CVector v) noexcept;
Vector cos3(CVector v1, CVector v2) noexcept;
Vector tripleProduct3(CVector v1, CVector v2, CVector v3, CVector e1, CVector e2, CVector e3) noexcept;
Vector cross3(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3) noexcept;
Vector tripleProduct3(CVector v1, CVector v2, CVector v3) noexcept;
Vector cross3(CVector v1, CVector v2) noexcept;
Vector project3(CVector v1, CVector v2) noexcept;
Vector reject3(CVector v1, CVector v2) noexcept;

Vector covector4(CVector v, CVector e1, CVector e2, CVector e3, CVector e4) noexcept;
Vector dot4(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3, CVector e4, Vector* cov = nullptr) noexcept;
Vector dot4(CVector v1, CVector v2) noexcept;
Vector length4(CVector v) noexcept;
Vector lengthSq4(CVector v) noexcept;
Vector normalize4(CVector v) noexcept;
Vector cos4(CVector v1, CVector v2) noexcept;
Vector project4(CVector v1, CVector v2) noexcept;
Vector reject4(CVector v1, CVector v2) noexcept;
}

struct alignas(16) VectorF32
{
  union
  {
    core::float32  f[4];
    Vector         v;
  };

  operator Vector() const noexcept { return v; }
};

struct alignas(16) VectorU32
{
  union
  {
    core::uint32  u[4];
    Vector  v;
  };

  operator Vector() const noexcept { return v; }
};

export constexpr core::uint32 gSelect0 = 0x00000000;
export constexpr core::uint32 gSelect1 = 0xFFFFFFFF;

export VectorU32 gVectorSelect1110 = { { { gSelect1, gSelect1, gSelect1, gSelect0 } } };

}