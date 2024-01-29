#include <core/math/vector.hpp>

namespace crude_engine
{

struct VectorF32
{
  union
  {
    float32  f[4];
    Vector   v;
  };

  operator Vector() const noexcept;
};

struct VectorU32
{
  union
  {
    uint32  u[4];
    Vector  v;
  };

  operator Vector() const noexcept;
};

VectorF32::operator Vector() const noexcept
{
  return v;
}

VectorU32::operator Vector() const noexcept
{
  return v;
}

Vector SysVector::zero() noexcept
{
  VectorF32 vResult = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
  return vResult;
}

Vector SysVector::set(float32 x, float32 y, float32 z, float32 w) noexcept
{
  VectorF32 vResult = { { { x, y, z, w } } };
  return vResult.v;
}

Vector SysVector::setInt(uint32 x, uint32 y, uint32 z, uint32 w) noexcept
{
  VectorU32 vResult = { { { x, y, z, w } } };
  return vResult;
}

Vector SysVector::fill(float32 value) noexcept
{
  VectorF32 vResult = { { { value, value, value, value } } };
  return vResult.v;
}

Vector SysVector::fillInt(uint32 value) noexcept
{
  VectorU32 vResult = { { { value, value, value, value } } };
  return vResult.v;
}

Vector SysVector::trueInt() noexcept
{
  VectorU32 vResult = { { { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU } } };
  return vResult.v;
}

Vector SysVector::falseInt() noexcept
{
  VectorU32 vResult = { { { 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U } } };
  return vResult.v;
}

Vector SysVector::splatX(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[0];
  return vResult;
}

Vector SysVector::splatY(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[1];
  return vResult;
}

Vector SysVector::splatZ(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[2];
  return vResult;
}

Vector SysVector::splatW(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[3];
  return vResult;
}

Vector SysVector::splatOne() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = 1.0f;
  return vResult;
}

Vector SysVector::splatInfinity() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = 0x7F800000;
  return vResult;
}

Vector SysVector::splatQNaN() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = 0x7FC00000;
  return vResult;
}

Vector SysVector::splatEpsilon() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] =
    vResult.vector4_f32[1] =
    vResult.vector4_f32[2] =
    vResult.vector4_f32[3] = 0x34000000;
  return vResult;

}

float32 SysVector::getX(CVector v) noexcept
{
  return v.vector4_f32[0];
}

float32 SysVector::getY(CVector v) noexcept
{
  return v.vector4_f32[1];
}

float32 SysVector::getZ(CVector v) noexcept
{
  return v.vector4_f32[2];
}

float32 SysVector::getW(CVector v) noexcept
{
  return v.vector4_f32[3];
}

uint32 SysVector::getIntX(CVector v) noexcept
{
  return v.vector4_u32[0];
}

uint32 SysVector::getIntY(CVector v) noexcept
{
  return v.vector4_u32[1];
}

uint32 SysVector::getIntZ(CVector v) noexcept
{
  return v.vector4_u32[2];
}

uint32 SysVector::getIntW(CVector v) noexcept
{
  return v.vector4_u32[3];
}

Vector SysVector::setX(CVector v, float32 x) noexcept
{
  VectorF32 vResult = { { {
    x,
    v.vector4_f32[1],
    v.vector4_f32[2],
    v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::setY(CVector v, float32 y) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0],
    y,
    v.vector4_f32[2],
    v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::setZ(CVector v, float32 z) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0],
    v.vector4_f32[1],
    z,
    v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::setW(CVector v, float32 w) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0],
    v.vector4_f32[1],
    v.vector4_f32[2],
    w
  } } };
  return vResult.v;
}

Vector SysVector::setIntX(CVector v, uint32 x) noexcept
{
  VectorU32 vResult = { { {
    x,
    v.vector4_u32[1],
    v.vector4_u32[2],
    v.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector SysVector::setIntY(CVector v, uint32 y) noexcept
{
  VectorU32 vResult = { { {
    v.vector4_u32[0],
    y,
    v.vector4_u32[2],
    v.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector SysVector::setIntZ(CVector v, uint32 z) noexcept
{
  VectorU32 vResult = { { {
    v.vector4_u32[0],
    v.vector4_u32[1],
    z,
    v.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector SysVector::setIntW(CVector v, uint32 w) noexcept
{
  VectorU32 vResult = { { {
    v.vector4_u32[0],
    v.vector4_u32[1],
    v.vector4_u32[2],
    w
  } } };
  return vResult.v;
}

Vector SysVector::equal(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] == v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] == v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] == v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] == v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector SysVector::equalInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_u32[0] == v2.vector4_u32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[1] == v2.vector4_u32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[2] == v2.vector4_u32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[3] == v2.vector4_u32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector SysVector::nearEqual(CVector v1, CVector v2, CVector epsilon) noexcept
{
  float32 fDeltaX = v1.vector4_f32[0] - v2.vector4_f32[0];
  float32 fDeltaY = v1.vector4_f32[1] - v2.vector4_f32[1];
  float32 fDeltaZ = v1.vector4_f32[2] - v2.vector4_f32[2];
  float32 fDeltaW = v1.vector4_f32[3] - v2.vector4_f32[3];

  if (fDeltaX < 0) fDeltaX = -fDeltaX;
  if (fDeltaY < 0) fDeltaY = -fDeltaY;
  if (fDeltaZ < 0) fDeltaZ = -fDeltaZ;
  if (fDeltaW < 0) fDeltaW = -fDeltaW;

  VectorU32 vResult = { { {
    (fDeltaX <= epsilon.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (fDeltaY <= epsilon.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (fDeltaZ <= epsilon.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (fDeltaW <= epsilon.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector SysVector::notEqual(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] != v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] != v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] != v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] != v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector SysVector::notEqualInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_u32[0] != v2.vector4_u32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[1] != v2.vector4_u32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[2] != v2.vector4_u32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[3] != v2.vector4_u32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector SysVector::greater(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] > v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] > v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] > v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] > v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector SysVector::greaterOrEqual(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] >= v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] >= v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] >= v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] >= v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector SysVector::less(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] < v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] < v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] < v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] < v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector SysVector::lessOrEqual(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] <= v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] <= v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] <= v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] <= v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector SysVector::inBounds(CVector v, CVector bounds) noexcept
{
  float32 absBoundsX = bounds.vector4_f32[0] > 0.0f ? bounds.vector4_f32[0] : -bounds.vector4_f32[0];
  float32 absBoundsY = bounds.vector4_f32[1] > 0.0f ? bounds.vector4_f32[1] : -bounds.vector4_f32[1];
  float32 absBoundsZ = bounds.vector4_f32[2] > 0.0f ? bounds.vector4_f32[2] : -bounds.vector4_f32[2];
  float32 absBoundsW = bounds.vector4_f32[3] > 0.0f ? bounds.vector4_f32[3] : -bounds.vector4_f32[3];

  VectorU32 vResult = { { {
    (v.vector4_f32[0] <= absBoundsX && v.vector4_f32[0] >= -absBoundsX) ? 0xFFFFFFFF : 0,
    (v.vector4_f32[1] <= absBoundsY && v.vector4_f32[1] >= -absBoundsY) ? 0xFFFFFFFF : 0,
    (v.vector4_f32[2] <= absBoundsZ && v.vector4_f32[2] >= -absBoundsZ) ? 0xFFFFFFFF : 0,
    (v.vector4_f32[3] <= absBoundsW && v.vector4_f32[3] >= -absBoundsW) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector SysVector::isNaN(CVector v) noexcept
{
  VectorU32 vResult = { { {
    (v.vector4_f32[0] == 0x7FC00000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[1] == 0x7FC00000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[2] == 0x7FC00000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[3] == 0x7FC00000) ? 0xFFFFFFFFU : 0
  } } };
  return vResult.v;
}

Vector SysVector::isInfinite(CVector v) noexcept
{
  VectorU32 vResult = { { {
    (v.vector4_f32[0] == 0x7F800000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[1] == 0x7F800000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[2] == 0x7F800000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[3] == 0x7F800000) ? 0xFFFFFFFFU : 0
  } } };
  return vResult.v;
}

Vector SysVector::min(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    (v1.vector4_f32[0] < v2.vector4_f32[0]) ? v1.vector4_f32[0] : v2.vector4_f32[0],
    (v1.vector4_f32[1] < v2.vector4_f32[1]) ? v1.vector4_f32[1] : v2.vector4_f32[1],
    (v1.vector4_f32[2] < v2.vector4_f32[2]) ? v1.vector4_f32[2] : v2.vector4_f32[2],
    (v1.vector4_f32[3] < v2.vector4_f32[3]) ? v1.vector4_f32[3] : v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::max(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    (v1.vector4_f32[0] > v2.vector4_f32[0]) ? v1.vector4_f32[0] : v2.vector4_f32[0],
    (v1.vector4_f32[1] > v2.vector4_f32[1]) ? v1.vector4_f32[1] : v2.vector4_f32[1],
    (v1.vector4_f32[2] > v2.vector4_f32[2]) ? v1.vector4_f32[2] : v2.vector4_f32[2],
    (v1.vector4_f32[3] > v2.vector4_f32[3]) ? v1.vector4_f32[3] : v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::round(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::round(v.vector4_f32[0]),
    Scalar::round(v.vector4_f32[1]),
    Scalar::round(v.vector4_f32[2]),
    Scalar::round(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::floor(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::floor(v.vector4_f32[0]),
    Scalar::floor(v.vector4_f32[1]),
    Scalar::floor(v.vector4_f32[2]),
    Scalar::floor(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::ceil(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::ceil(v.vector4_f32[0]),
    Scalar::ceil(v.vector4_f32[1]),
    Scalar::ceil(v.vector4_f32[2]),
    Scalar::ceil(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::trunc(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::trunc(v.vector4_f32[0]),
    Scalar::trunc(v.vector4_f32[1]),
    Scalar::trunc(v.vector4_f32[2]),
    Scalar::trunc(v.vector4_f32[3])
  } } };
  return vResult.v;
}


Vector SysVector::clamp(CVector v, CVector vmin, CVector vmax) noexcept
{
  VectorF32 vResult = { { {
    Scalar::clamp(v.vector4_f32[0], vmin.vector4_f32[0], vmax.vector4_f32[0]),
    Scalar::clamp(v.vector4_f32[1], vmin.vector4_f32[1], vmax.vector4_f32[1]),
    Scalar::clamp(v.vector4_f32[2], vmin.vector4_f32[2], vmax.vector4_f32[2]),
    Scalar::clamp(v.vector4_f32[3], vmin.vector4_f32[3], vmax.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::andInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    v1.vector4_u32[0] & v2.vector4_u32[0],
    v1.vector4_u32[1] & v2.vector4_u32[1],
    v1.vector4_u32[2] & v2.vector4_u32[2],
    v1.vector4_u32[3] & v2.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector SysVector::orInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    v1.vector4_u32[0] | v2.vector4_u32[0],
    v1.vector4_u32[1] | v2.vector4_u32[1],
    v1.vector4_u32[2] | v2.vector4_u32[2],
    v1.vector4_u32[3] | v2.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector SysVector::norInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    ~(v1.vector4_u32[0] | v2.vector4_u32[0]),
    ~(v1.vector4_u32[1] | v2.vector4_u32[1]),
    ~(v1.vector4_u32[2] | v2.vector4_u32[2]),
    ~(v1.vector4_u32[3] | v2.vector4_u32[3])
  } } };
  return vResult.v;
}

Vector SysVector::xorInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    v1.vector4_u32[0] ^ v2.vector4_u32[0],
    v1.vector4_u32[1] ^ v2.vector4_u32[1],
    v1.vector4_u32[2] ^ v2.vector4_u32[2],
    v1.vector4_u32[3] ^ v2.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector SysVector::select(CVector v1, CVector v2, CVector control) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_u32[0] & control.vector4_u32[0]) | (v2.vector4_u32[0] & ~control.vector4_u32[0]),
    (v1.vector4_u32[1] & control.vector4_u32[1]) | (v2.vector4_u32[1] & ~control.vector4_u32[1]),
    (v1.vector4_u32[2] & control.vector4_u32[2]) | (v2.vector4_u32[2] & ~control.vector4_u32[2]),
    (v1.vector4_u32[3] & control.vector4_u32[3]) | (v2.vector4_u32[3] & ~control.vector4_u32[3])
  } } };
  return vResult.v;
}

Vector SysVector::negate(CVector v) noexcept
{
  VectorF32 vResult = { { {
    -v.vector4_f32[0],
    -v.vector4_f32[1],
    -v.vector4_f32[2],
    -v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::add(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] + v2.vector4_f32[0],
    v1.vector4_f32[1] + v2.vector4_f32[1],
    v1.vector4_f32[2] + v2.vector4_f32[2],
    v1.vector4_f32[3] + v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::subtract(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] - v2.vector4_f32[0],
    v1.vector4_f32[1] - v2.vector4_f32[1],
    v1.vector4_f32[2] - v2.vector4_f32[2],
    v1.vector4_f32[3] - v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::multiply(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] * v2.vector4_f32[0],
    v1.vector4_f32[1] * v2.vector4_f32[1],
    v1.vector4_f32[2] * v2.vector4_f32[2],
    v1.vector4_f32[3] * v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::multiplyAdd(CVector v1, CVector v2, CVector v3) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] * v2.vector4_f32[0] + v3.vector4_f32[0],
    v1.vector4_f32[1] * v2.vector4_f32[1] + v3.vector4_f32[1],
    v1.vector4_f32[2] * v2.vector4_f32[2] + v3.vector4_f32[2],
    v1.vector4_f32[3] * v2.vector4_f32[3] + v3.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::divide(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] / v2.vector4_f32[0],
    v1.vector4_f32[1] / v2.vector4_f32[1],
    v1.vector4_f32[2] / v2.vector4_f32[2],
    v1.vector4_f32[3] / v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::divideAdd(CVector v1, CVector v2, CVector v3) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] / v2.vector4_f32[0] + v3.vector4_f32[0],
    v1.vector4_f32[1] / v2.vector4_f32[1] + v3.vector4_f32[1],
    v1.vector4_f32[2] / v2.vector4_f32[2] + v3.vector4_f32[2],
    v1.vector4_f32[3] / v2.vector4_f32[3] + v3.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector SysVector::scale(CVector v, float32 s) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0] * s,
    v.vector4_f32[1] * s,
    v.vector4_f32[2] * s,
    v.vector4_f32[3] * s
  } } };
  return vResult.v;
}

Vector SysVector::sqrt(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::sqrt(v.vector4_f32[0]),
    Scalar::sqrt(v.vector4_f32[1]),
    Scalar::sqrt(v.vector4_f32[2]),
    Scalar::sqrt(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::exp2(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::exp2(v.vector4_f32[0]),
    Scalar::exp2(v.vector4_f32[1]),
    Scalar::exp2(v.vector4_f32[2]),
    Scalar::exp2(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::exp(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::exp(v.vector4_f32[0]),
    Scalar::exp(v.vector4_f32[1]),
    Scalar::exp(v.vector4_f32[2]),
    Scalar::exp(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::log2(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::log2(v.vector4_f32[0]),
    Scalar::log2(v.vector4_f32[1]),
    Scalar::log2(v.vector4_f32[2]),
    Scalar::log2(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::log10(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::log10(v.vector4_f32[0]),
    Scalar::log10(v.vector4_f32[1]),
    Scalar::log10(v.vector4_f32[2]),
    Scalar::log10(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::log(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::log(v.vector4_f32[0]),
    Scalar::log(v.vector4_f32[1]),
    Scalar::log(v.vector4_f32[2]),
    Scalar::log(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::pow(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    Scalar::pow(v1.vector4_f32[0], v2.vector4_f32[0]),
    Scalar::pow(v1.vector4_f32[1], v2.vector4_f32[1]),
    Scalar::pow(v1.vector4_f32[2], v2.vector4_f32[2]),
    Scalar::pow(v1.vector4_f32[3], v2.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::abs(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::abs(v.vector4_f32[0]),
    Scalar::abs(v.vector4_f32[1]),
    Scalar::abs(v.vector4_f32[2]),
    Scalar::abs(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::cos(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::cos(v.vector4_f32[0]),
    Scalar::cos(v.vector4_f32[1]),
    Scalar::cos(v.vector4_f32[2]),
    Scalar::cos(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::sin(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::sin(v.vector4_f32[0]),
    Scalar::sin(v.vector4_f32[1]),
    Scalar::sin(v.vector4_f32[2]),
    Scalar::sin(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::tan(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::tan(v.vector4_f32[0]),
    Scalar::tan(v.vector4_f32[1]),
    Scalar::tan(v.vector4_f32[2]),
    Scalar::tan(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::arcCos(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::arcCos(v.vector4_f32[0]),
    Scalar::arcCos(v.vector4_f32[1]),
    Scalar::arcCos(v.vector4_f32[2]),
    Scalar::arcCos(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::arcSin(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::arcSin(v.vector4_f32[0]),
    Scalar::arcSin(v.vector4_f32[1]),
    Scalar::arcSin(v.vector4_f32[2]),
    Scalar::arcSin(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::arcTan(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::arcTan(v.vector4_f32[0]),
    Scalar::arcTan(v.vector4_f32[1]),
    Scalar::arcTan(v.vector4_f32[2]),
    Scalar::arcTan(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector SysVector::covector1(CVector v, CVector e1) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = SysVector::dot1(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = 1.0f;
  vResult.vector4_f32[2] = 1.0f;
  vResult.vector4_f32[3] = 1.0f;
  return vResult;
}

Vector SysVector::dot1(CVector v1, CVector v2, CVector e1, Vector* cov) noexcept
{
  Vector covector = SysVector::covector1(v2, e1);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = SysVector::dot1(v1, covector);
  return vResult;
}

Vector SysVector::dot1(CVector v1, CVector v2) noexcept
{
  float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0];

  Vector vResult = SysVector::fill(fDot);
  return vResult;
}

Vector SysVector::length1(CVector v) noexcept
{
  Vector vOrthoDotResult = SysVector::dot1(v, v);
  float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  float32 fOrthoNormLength = Scalar::sqrt(fOrthoDotResult);
  Vector vResult = SysVector::fill(fOrthoNormLength);
  return vResult;
}

Vector SysVector::lengthSq1(CVector v) noexcept
{
  Vector vResult = SysVector::dot1(v, v);
  return vResult;
}

Vector SysVector::normalize1(CVector v) noexcept
{
  Vector vLength = SysVector::lengthSq1(v);
  float32 fLength = vLength.vector4_f32[0];
  float32 fInvLength;

  if (fLength > 0)
  {
    fInvLength = 1.0f / fLength;
  }
  else
  {
    fInvLength = Constans::CMAXF32;
  }

  Vector vResult;
  vResult.vector4_f32[0] = v.vector4_f32[0] * fInvLength;
  vResult.vector4_f32[1] = v.vector4_f32[1];
  vResult.vector4_f32[2] = v.vector4_f32[2];
  vResult.vector4_f32[3] = v.vector4_f32[3];

  return vResult;
}

Vector SysVector::cos1(CVector v1, CVector v2) noexcept
{
  Vector vDot = SysVector::dot1(v1, v2);
  Vector vLength1 = SysVector::lengthSq1(v1);
  Vector vLength2 = SysVector::lengthSq1(v2);
  float32 fDot = vDot.vector4_f32[0];
  float32 fLength1 = vLength1.vector4_f32[0];
  float32 fLength2 = vLength2.vector4_f32[0];
  float32 fMul = fLength1 * fLength2;
  float32 fSqrtMul = Scalar::sqrt(fMul);
  float32 fCos = fDot / fSqrtMul;
  Vector vResult = SysVector::fill(fCos);
  return vResult;
}

Vector SysVector::covector2(CVector v, CVector e1, CVector e2) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = SysVector::dot2(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = SysVector::dot2(v, e2).vector4_f32[0];
  vResult.vector4_f32[2] = 1.0f;
  vResult.vector4_f32[3] = 1.0f;

  return vResult;
}

Vector SysVector::dot2(CVector v1, CVector v2, CVector e1, CVector e2, Vector* cov) noexcept
{
  Vector covector = SysVector::covector2(v2, e1, e2);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = SysVector::dot2(v1, covector);
  return vResult;
}

Vector SysVector::dot2(CVector v1, CVector v2) noexcept
{
  float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0] +
    v1.vector4_f32[1] * v2.vector4_f32[1];

  Vector vResult = SysVector::fill(fDot);
  return vResult;
}

Vector SysVector::length2(CVector v) noexcept
{
  Vector vOrthoDotResult = SysVector::dot2(v, v);
  float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  float32 fOrthoNormLength = Scalar::sqrt(fOrthoDotResult);
  Vector vResult = SysVector::fill(fOrthoNormLength);
  return vResult;
}

Vector SysVector::lengthSq2(CVector v) noexcept
{
  Vector vResult = SysVector::dot2(v, v);
  return vResult;
}

Vector SysVector::normalize2(CVector v) noexcept
{
  Vector vLength = SysVector::lengthSq2(v);
  float32 fLength = vLength.vector4_f32[0];
  float32 fInvLength;

  if (fLength > 0.0f)
  {
    fInvLength = 1.0f / fLength;
  }
  else
  {
    fInvLength = Constans::CMAXF32;
  }

  Vector vResult;
  vResult.vector4_f32[0] = v.vector4_f32[0] * fInvLength;
  vResult.vector4_f32[1] = v.vector4_f32[1] * fInvLength;
  vResult.vector4_f32[2] = v.vector4_f32[2];
  vResult.vector4_f32[3] = v.vector4_f32[3];

  return vResult;
}

Vector
SysVector::cos2(CVector v1, CVector v2) noexcept
{
  Vector vDot = SysVector::dot2(v1, v2);
  Vector vLength1 = SysVector::lengthSq2(v1);
  Vector vLength2 = SysVector::lengthSq2(v2);
  float32 fDot = vDot.vector4_f32[0];
  float32 fLength1 = vLength1.vector4_f32[0];
  float32 fLength2 = vLength2.vector4_f32[0];
  float32 fMul = fLength1 * fLength2;
  float32 fSqrtMul = Scalar::sqrt(fMul);
  float32 fCos = fDot / fSqrtMul;
  Vector vResult = SysVector::fill(fCos);
  return vResult;
}

Vector SysVector::covector3(CVector v, CVector e1, CVector e2, CVector e3) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = SysVector::dot3(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = SysVector::dot3(v, e2).vector4_f32[0];
  vResult.vector4_f32[2] = SysVector::dot3(v, e3).vector4_f32[0];
  vResult.vector4_f32[3] = 1.0f;

  return vResult;
}

Vector SysVector::dot3(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3, Vector* cov) noexcept
{
  Vector covector = SysVector::covector3(v2, e1, e2, e3);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = SysVector::dot3(v1, covector);
  return vResult;
}

Vector SysVector::dot3(CVector v1, CVector v2) noexcept
{
  float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0] +
    v1.vector4_f32[1] * v2.vector4_f32[1] +
    v1.vector4_f32[2] * v2.vector4_f32[2];

  Vector vResult = SysVector::fill(fDot);
  return vResult;
}

Vector SysVector::length3(CVector v) noexcept
{
  Vector vOrthoDotResult = SysVector::dot3(v, v);
  float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  float32 fOrthoNormLength = Scalar::sqrt(fOrthoDotResult);
  Vector vResult = SysVector::fill(fOrthoNormLength);
  return vResult;
}

Vector SysVector::lengthSq3(CVector v) noexcept
{
  Vector vResult = SysVector::dot3(v, v);
  return vResult;
}

Vector SysVector::normalize3(CVector v) noexcept
{
  Vector vLength = SysVector::lengthSq3(v);
  float32 fLength = vLength.vector4_f32[0];
  float32 fInvLength;

  if (fLength > 0.0f)
  {
    fInvLength = 1.0f / fLength;
  }
  else
  {
    fInvLength = Constans::CMAXF32;
  }

  Vector vResult;
  vResult.vector4_f32[0] = v.vector4_f32[0] * fInvLength;
  vResult.vector4_f32[1] = v.vector4_f32[1] * fInvLength;
  vResult.vector4_f32[2] = v.vector4_f32[2] * fInvLength;
  vResult.vector4_f32[3] = v.vector4_f32[3];

  return vResult;
}

Vector SysVector::cos3(CVector v1, CVector v2) noexcept
{
  Vector vDot = SysVector::dot3(v1, v2);
  Vector vLength1 = SysVector::lengthSq3(v1);
  Vector vLength2 = SysVector::lengthSq3(v2);
  float32 fDot = vDot.vector4_f32[0];
  float32 fLength1 = vLength1.vector4_f32[0];
  float32 fLength2 = vLength2.vector4_f32[0];
  float32 fMul = fLength1 * fLength2;
  float32 fSqrtMul = Scalar::sqrt(fMul);
  float32 fCos = fDot / fSqrtMul;
  Vector vResult = SysVector::fill(fCos);
  return vResult;
}

Vector SysVector::covector4(CVector v, CVector e1, CVector e2, CVector e3, CVector e4) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = SysVector::dot4(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = SysVector::dot4(v, e2).vector4_f32[0];
  vResult.vector4_f32[2] = SysVector::dot4(v, e3).vector4_f32[0];
  vResult.vector4_f32[3] = SysVector::dot4(v, e4).vector4_f32[0];

  return vResult;
}

Vector SysVector::dot4(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3, CVector e4, Vector* cov) noexcept
{
  Vector covector = SysVector::covector4(v2, e1, e2, e3, e4);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = SysVector::dot4(v1, covector);
  return vResult;
}

Vector SysVector::dot4(CVector v1, CVector v2) noexcept
{
  float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0] +
    v1.vector4_f32[1] * v2.vector4_f32[1] +
    v1.vector4_f32[2] * v2.vector4_f32[2] +
    v1.vector4_f32[3] * v2.vector4_f32[3];

  Vector vResult = SysVector::fill(fDot);
  return vResult;
}

Vector SysVector::length4(CVector v) noexcept
{
  Vector vOrthoDotResult = SysVector::dot4(v, v);
  float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  float32 fOrthoNormLength = Scalar::sqrt(fOrthoDotResult);
  Vector vResult = SysVector::fill(fOrthoNormLength);
  return vResult;
}

Vector SysVector::lengthSq4(CVector v) noexcept
{
  Vector vResult = SysVector::dot4(v, v);
  return vResult;
}

Vector SysVector::normalize4(CVector v) noexcept
{
  Vector vLength = SysVector::lengthSq4(v);
  float32 fLength = vLength.vector4_f32[0];
  float32 fInvLength;

  if (fLength > 0)
  {
    fInvLength = 1.0f / fLength;
  }
  else
  {
    fInvLength = Constans::CMAXF32;
  }

  Vector vResult;
  vResult.vector4_f32[0] = v.vector4_f32[0] * fInvLength;
  vResult.vector4_f32[1] = v.vector4_f32[1] * fInvLength;
  vResult.vector4_f32[2] = v.vector4_f32[2] * fInvLength;
  vResult.vector4_f32[3] = v.vector4_f32[3] * fInvLength;

  return vResult;
}

Vector SysVector::cos4(CVector v1, CVector v2) noexcept
{
  Vector vDot = SysVector::dot4(v1, v2);
  Vector vLength1 = SysVector::lengthSq4(v1);
  Vector vLength2 = SysVector::lengthSq4(v2);
  float32 fDot = vDot.vector4_f32[0];
  float32 fLength1 = vLength1.vector4_f32[0];
  float32 fLength2 = vLength2.vector4_f32[0];
  float32 fMul = fLength1 * fLength2;
  float32 fSqrtMul = Scalar::sqrt(fMul);
  float32 fCos = fDot / fSqrtMul;
  Vector vResult = SysVector::fill(fCos);
  return vResult;
}

Vector SysVector::project1(CVector v1, CVector v2) noexcept
{
  float32 x = SysVector::getX(SysVector::dot1(v1, v2));
  float32 y = SysVector::getX(SysVector::lengthSq1(v2));
  Vector vResult = SysVector::scale(v2, x / y);
  return vResult;
}

Vector SysVector::reject1(CVector v1, CVector v2) noexcept
{
  Vector vProject = SysVector::project1(v1, v2);
  Vector vResult = SysVector::subtract(v1, vProject);
  return vResult;
}

Vector SysVector::project2(CVector v1, CVector v2) noexcept
{
  float32 x = SysVector::getX(SysVector::dot2(v1, v2));
  float32 y = SysVector::getX(SysVector::lengthSq2(v2));
  Vector vResult = SysVector::scale(v2, x / y);
  return vResult;
}

Vector SysVector::reject2(CVector v1, CVector v2) noexcept
{
  Vector vProject = SysVector::project2(v1, v2);
  Vector vResult = SysVector::subtract(v1, vProject);
  return vResult;
}

Vector SysVector::project3(CVector v1, CVector v2) noexcept
{
  float32 x = SysVector::getX(SysVector::dot3(v1, v2));
  float32 y = SysVector::getX(SysVector::lengthSq3(v2));
  Vector vResult = SysVector::scale(v2, x / y);
  return vResult;
}

Vector SysVector::reject3(CVector v1, CVector v2) noexcept
{
  Vector vProject = SysVector::project3(v1, v2);
  Vector vResult = SysVector::subtract(v1, vProject);
  return vResult;
}

Vector SysVector::project4(CVector v1, CVector v2) noexcept
{
  float32 x = SysVector::getX(SysVector::dot4(v1, v2));
  float32 y = SysVector::getX(SysVector::lengthSq4(v2));
  Vector vResult = SysVector::scale(v2, x / y);
  return vResult;
}

Vector SysVector::reject4(CVector v1, CVector v2) noexcept
{
  Vector vProject = SysVector::project4(v1, v2);
  Vector vResult = SysVector::subtract(v1, vProject);
  return vResult;
}

Vector SysVector::tripleProduct3(CVector v1, CVector v2, CVector v3, CVector e1, CVector e2, CVector e3) noexcept
{
  float32 x = SysVector::getX(SysVector::tripleProduct3(v1, v2, v3));
  float32 y = SysVector::getX(SysVector::tripleProduct3(e1, e2, e3));
  Vector vResult = SysVector::fill(x * y);
  return vResult;
}

Vector SysVector::cross3(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3) noexcept
{
  Vector vResult;
  {
    float32 s1 = v1.vector4_f32[1] * v2.vector4_f32[2] - v1.vector4_f32[2] * v2.vector4_f32[1];
    float32 s2 = v1.vector4_f32[2] * v2.vector4_f32[0] - v1.vector4_f32[0] * v2.vector4_f32[2];
    float32 s3 = v1.vector4_f32[0] * v2.vector4_f32[1] - v1.vector4_f32[1] * v2.vector4_f32[0];
    vResult = SysVector::scale(e1, s1);
    vResult = SysVector::add(vResult, SysVector::scale(e2, s2));
    vResult = SysVector::add(vResult, SysVector::scale(e3, s3));
  }
  return vResult;
}

Vector SysVector::tripleProduct3(CVector v1, CVector v2, CVector v3) noexcept
{
  float32 x;
  {
    x = v1.vector4_f32[0] * (v2.vector4_f32[1] * v3.vector4_f32[2] - v2.vector4_f32[2] * v3.vector4_f32[1]);
    x += v1.vector4_f32[1] * (v2.vector4_f32[2] * v3.vector4_f32[0] - v2.vector4_f32[0] * v3.vector4_f32[2]);
    x += v1.vector4_f32[2] * (v2.vector4_f32[0] * v3.vector4_f32[1] - v2.vector4_f32[1] * v3.vector4_f32[0]);
  }

  Vector vResult = SysVector::fill(x);
  return vResult;
}

Vector SysVector::cross3(CVector v1, CVector v2) noexcept
{
  Vector vResult;
  {
    vResult.vector4_f32[0] = v1.vector4_f32[1] * v2.vector4_f32[2] - v1.vector4_f32[2] * v2.vector4_f32[1];
    vResult.vector4_f32[1] = v1.vector4_f32[2] * v2.vector4_f32[0] - v1.vector4_f32[0] * v2.vector4_f32[2];
    vResult.vector4_f32[2] = v1.vector4_f32[0] * v2.vector4_f32[1] - v1.vector4_f32[1] * v2.vector4_f32[0];
    vResult.vector4_f32[3] = 0.0f;
  }
  return vResult;
}

__vector4::__vector4(const __vector4& other) noexcept
  :
  vector4_u32{ other.vector4_u32[0], vector4_u32 [1], vector4_u32 [2], vector4_u32 [3]}
{}

__vector4& __vector4::operator=(const __vector4& other) noexcept
{
  vector4_u32[0] = other.vector4_u32[0];
  vector4_u32[1] = other.vector4_u32[1];
  vector4_u32[2] = other.vector4_u32[2];
  vector4_u32[3] = other.vector4_u32[3];
}

__vector4::__vector4(__vector4&& other) noexcept
  :
  vector4_u32{ other.vector4_u32[0], vector4_u32[1], vector4_u32[2], vector4_u32[3] }
{}

__vector4& __vector4::operator=(__vector4&& other) noexcept
{
  vector4_u32[0] = other.vector4_u32[0];
  vector4_u32[1] = other.vector4_u32[1];
  vector4_u32[2] = other.vector4_u32[2];
  vector4_u32[3] = other.vector4_u32[3];
}

}