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

Vector Vector::zero() noexcept
{
  VectorF32 vResult = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
  return vResult;
}

Vector Vector::set(float32 x, float32 y, float32 z, float32 w) noexcept
{
  VectorF32 vResult = { { { x, y, z, w } } };
  return vResult.v;
}

Vector Vector::setInt(uint32 x, uint32 y, uint32 z, uint32 w) noexcept
{
  VectorU32 vResult = { { { x, y, z, w } } };
  return vResult;
}

Vector Vector::fill(float32 value) noexcept
{
  VectorF32 vResult = { { { value, value, value, value } } };
  return vResult.v;
}

Vector Vector::fillInt(uint32 value) noexcept
{
  VectorU32 vResult = { { { value, value, value, value } } };
  return vResult.v;
}

Vector Vector::trueInt() noexcept
{
  VectorU32 vResult = { { { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU } } };
  return vResult.v;
}

Vector Vector::falseInt() noexcept
{
  VectorU32 vResult = { { { 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U } } };
  return vResult.v;
}

Vector Vector::splatX(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[0];
  return vResult;
}

Vector Vector::splatY(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[1];
  return vResult;
}

Vector Vector::splatZ(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[2];
  return vResult;
}

Vector Vector::splatW(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[3];
  return vResult;
}

Vector Vector::splatOne() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = 1.0f;
  return vResult;
}

Vector Vector::splatInfinity() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = 0x7F800000;
  return vResult;
}

Vector Vector::splatQNaN() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = 0x7FC00000;
  return vResult;
}

Vector Vector::splatEpsilon() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] =
    vResult.vector4_f32[1] =
    vResult.vector4_f32[2] =
    vResult.vector4_f32[3] = 0x34000000;
  return vResult;

}

float32 Vector::getX(CVector v) noexcept
{
  return v.vector4_f32[0];
}

float32 Vector::getY(CVector v) noexcept
{
  return v.vector4_f32[1];
}

float32 Vector::getZ(CVector v) noexcept
{
  return v.vector4_f32[2];
}

float32 Vector::getW(CVector v) noexcept
{
  return v.vector4_f32[3];
}

uint32 Vector::getIntX(CVector v) noexcept
{
  return v.vector4_u32[0];
}

uint32 Vector::getIntY(CVector v) noexcept
{
  return v.vector4_u32[1];
}

uint32 Vector::getIntZ(CVector v) noexcept
{
  return v.vector4_u32[2];
}

uint32 Vector::getIntW(CVector v) noexcept
{
  return v.vector4_u32[3];
}

Vector Vector::setX(CVector v, float32 x) noexcept
{
  VectorF32 vResult = { { {
    x,
    v.vector4_f32[1],
    v.vector4_f32[2],
    v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::setY(CVector v, float32 y) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0],
    y,
    v.vector4_f32[2],
    v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::setZ(CVector v, float32 z) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0],
    v.vector4_f32[1],
    z,
    v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::setW(CVector v, float32 w) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0],
    v.vector4_f32[1],
    v.vector4_f32[2],
    w
  } } };
  return vResult.v;
}

Vector Vector::setIntX(CVector v, uint32 x) noexcept
{
  VectorU32 vResult = { { {
    x,
    v.vector4_u32[1],
    v.vector4_u32[2],
    v.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector Vector::setIntY(CVector v, uint32 y) noexcept
{
  VectorU32 vResult = { { {
    v.vector4_u32[0],
    y,
    v.vector4_u32[2],
    v.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector Vector::setIntZ(CVector v, uint32 z) noexcept
{
  VectorU32 vResult = { { {
    v.vector4_u32[0],
    v.vector4_u32[1],
    z,
    v.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector Vector::setIntW(CVector v, uint32 w) noexcept
{
  VectorU32 vResult = { { {
    v.vector4_u32[0],
    v.vector4_u32[1],
    v.vector4_u32[2],
    w
  } } };
  return vResult.v;
}

Vector Vector::equal(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] == v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] == v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] == v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] == v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector Vector::equalInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_u32[0] == v2.vector4_u32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[1] == v2.vector4_u32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[2] == v2.vector4_u32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[3] == v2.vector4_u32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector Vector::nearEqual(CVector v1, CVector v2, CVector epsilon) noexcept
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

Vector Vector::notEqual(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] != v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] != v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] != v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] != v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector Vector::notEqualInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_u32[0] != v2.vector4_u32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[1] != v2.vector4_u32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[2] != v2.vector4_u32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[3] != v2.vector4_u32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector Vector::greater(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] > v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] > v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] > v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] > v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector Vector::greaterOrEqual(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] >= v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] >= v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] >= v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] >= v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector Vector::less(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] < v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] < v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] < v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] < v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector Vector::lessOrEqual(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] <= v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] <= v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] <= v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] <= v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector Vector::inBounds(CVector v, CVector bounds) noexcept
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

Vector Vector::isNaN(CVector v) noexcept
{
  VectorU32 vResult = { { {
    (v.vector4_f32[0] == 0x7FC00000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[1] == 0x7FC00000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[2] == 0x7FC00000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[3] == 0x7FC00000) ? 0xFFFFFFFFU : 0
  } } };
  return vResult.v;
}

Vector Vector::isInfinite(CVector v) noexcept
{
  VectorU32 vResult = { { {
    (v.vector4_f32[0] == 0x7F800000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[1] == 0x7F800000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[2] == 0x7F800000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[3] == 0x7F800000) ? 0xFFFFFFFFU : 0
  } } };
  return vResult.v;
}

Vector Vector::min(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    (v1.vector4_f32[0] < v2.vector4_f32[0]) ? v1.vector4_f32[0] : v2.vector4_f32[0],
    (v1.vector4_f32[1] < v2.vector4_f32[1]) ? v1.vector4_f32[1] : v2.vector4_f32[1],
    (v1.vector4_f32[2] < v2.vector4_f32[2]) ? v1.vector4_f32[2] : v2.vector4_f32[2],
    (v1.vector4_f32[3] < v2.vector4_f32[3]) ? v1.vector4_f32[3] : v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::max(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    (v1.vector4_f32[0] > v2.vector4_f32[0]) ? v1.vector4_f32[0] : v2.vector4_f32[0],
    (v1.vector4_f32[1] > v2.vector4_f32[1]) ? v1.vector4_f32[1] : v2.vector4_f32[1],
    (v1.vector4_f32[2] > v2.vector4_f32[2]) ? v1.vector4_f32[2] : v2.vector4_f32[2],
    (v1.vector4_f32[3] > v2.vector4_f32[3]) ? v1.vector4_f32[3] : v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::round(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::round(v.vector4_f32[0]),
    Scalar::round(v.vector4_f32[1]),
    Scalar::round(v.vector4_f32[2]),
    Scalar::round(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::floor(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::floor(v.vector4_f32[0]),
    Scalar::floor(v.vector4_f32[1]),
    Scalar::floor(v.vector4_f32[2]),
    Scalar::floor(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::ceil(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::ceil(v.vector4_f32[0]),
    Scalar::ceil(v.vector4_f32[1]),
    Scalar::ceil(v.vector4_f32[2]),
    Scalar::ceil(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::trunc(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::trunc(v.vector4_f32[0]),
    Scalar::trunc(v.vector4_f32[1]),
    Scalar::trunc(v.vector4_f32[2]),
    Scalar::trunc(v.vector4_f32[3])
  } } };
  return vResult.v;
}


Vector Vector::clamp(CVector v, CVector vmin, CVector vmax) noexcept
{
  VectorF32 vResult = { { {
    Scalar::clamp(v.vector4_f32[0], vmin.vector4_f32[0], vmax.vector4_f32[0]),
    Scalar::clamp(v.vector4_f32[1], vmin.vector4_f32[1], vmax.vector4_f32[1]),
    Scalar::clamp(v.vector4_f32[2], vmin.vector4_f32[2], vmax.vector4_f32[2]),
    Scalar::clamp(v.vector4_f32[3], vmin.vector4_f32[3], vmax.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::andInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    v1.vector4_u32[0] & v2.vector4_u32[0],
    v1.vector4_u32[1] & v2.vector4_u32[1],
    v1.vector4_u32[2] & v2.vector4_u32[2],
    v1.vector4_u32[3] & v2.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector Vector::orInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    v1.vector4_u32[0] | v2.vector4_u32[0],
    v1.vector4_u32[1] | v2.vector4_u32[1],
    v1.vector4_u32[2] | v2.vector4_u32[2],
    v1.vector4_u32[3] | v2.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector Vector::norInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    ~(v1.vector4_u32[0] | v2.vector4_u32[0]),
    ~(v1.vector4_u32[1] | v2.vector4_u32[1]),
    ~(v1.vector4_u32[2] | v2.vector4_u32[2]),
    ~(v1.vector4_u32[3] | v2.vector4_u32[3])
  } } };
  return vResult.v;
}

Vector Vector::xorInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    v1.vector4_u32[0] ^ v2.vector4_u32[0],
    v1.vector4_u32[1] ^ v2.vector4_u32[1],
    v1.vector4_u32[2] ^ v2.vector4_u32[2],
    v1.vector4_u32[3] ^ v2.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector Vector::select(CVector v1, CVector v2, CVector control) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_u32[0] & control.vector4_u32[0]) | (v2.vector4_u32[0] & ~control.vector4_u32[0]),
    (v1.vector4_u32[1] & control.vector4_u32[1]) | (v2.vector4_u32[1] & ~control.vector4_u32[1]),
    (v1.vector4_u32[2] & control.vector4_u32[2]) | (v2.vector4_u32[2] & ~control.vector4_u32[2]),
    (v1.vector4_u32[3] & control.vector4_u32[3]) | (v2.vector4_u32[3] & ~control.vector4_u32[3])
  } } };
  return vResult.v;
}

Vector Vector::negate(CVector v) noexcept
{
  VectorF32 vResult = { { {
    -v.vector4_f32[0],
    -v.vector4_f32[1],
    -v.vector4_f32[2],
    -v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::add(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] + v2.vector4_f32[0],
    v1.vector4_f32[1] + v2.vector4_f32[1],
    v1.vector4_f32[2] + v2.vector4_f32[2],
    v1.vector4_f32[3] + v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::subtract(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] - v2.vector4_f32[0],
    v1.vector4_f32[1] - v2.vector4_f32[1],
    v1.vector4_f32[2] - v2.vector4_f32[2],
    v1.vector4_f32[3] - v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::multiply(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] * v2.vector4_f32[0],
    v1.vector4_f32[1] * v2.vector4_f32[1],
    v1.vector4_f32[2] * v2.vector4_f32[2],
    v1.vector4_f32[3] * v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::multiplyAdd(CVector v1, CVector v2, CVector v3) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] * v2.vector4_f32[0] + v3.vector4_f32[0],
    v1.vector4_f32[1] * v2.vector4_f32[1] + v3.vector4_f32[1],
    v1.vector4_f32[2] * v2.vector4_f32[2] + v3.vector4_f32[2],
    v1.vector4_f32[3] * v2.vector4_f32[3] + v3.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::divide(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] / v2.vector4_f32[0],
    v1.vector4_f32[1] / v2.vector4_f32[1],
    v1.vector4_f32[2] / v2.vector4_f32[2],
    v1.vector4_f32[3] / v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::divideAdd(CVector v1, CVector v2, CVector v3) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] / v2.vector4_f32[0] + v3.vector4_f32[0],
    v1.vector4_f32[1] / v2.vector4_f32[1] + v3.vector4_f32[1],
    v1.vector4_f32[2] / v2.vector4_f32[2] + v3.vector4_f32[2],
    v1.vector4_f32[3] / v2.vector4_f32[3] + v3.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector Vector::scale(CVector v, float32 s) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0] * s,
    v.vector4_f32[1] * s,
    v.vector4_f32[2] * s,
    v.vector4_f32[3] * s
  } } };
  return vResult.v;
}

Vector Vector::sqrt(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::sqrt(v.vector4_f32[0]),
    Scalar::sqrt(v.vector4_f32[1]),
    Scalar::sqrt(v.vector4_f32[2]),
    Scalar::sqrt(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::exp2(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::exp2(v.vector4_f32[0]),
    Scalar::exp2(v.vector4_f32[1]),
    Scalar::exp2(v.vector4_f32[2]),
    Scalar::exp2(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::exp(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::exp(v.vector4_f32[0]),
    Scalar::exp(v.vector4_f32[1]),
    Scalar::exp(v.vector4_f32[2]),
    Scalar::exp(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::log2(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::log2(v.vector4_f32[0]),
    Scalar::log2(v.vector4_f32[1]),
    Scalar::log2(v.vector4_f32[2]),
    Scalar::log2(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::log10(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::log10(v.vector4_f32[0]),
    Scalar::log10(v.vector4_f32[1]),
    Scalar::log10(v.vector4_f32[2]),
    Scalar::log10(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::log(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::log(v.vector4_f32[0]),
    Scalar::log(v.vector4_f32[1]),
    Scalar::log(v.vector4_f32[2]),
    Scalar::log(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::pow(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    Scalar::pow(v1.vector4_f32[0], v2.vector4_f32[0]),
    Scalar::pow(v1.vector4_f32[1], v2.vector4_f32[1]),
    Scalar::pow(v1.vector4_f32[2], v2.vector4_f32[2]),
    Scalar::pow(v1.vector4_f32[3], v2.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::abs(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::abs(v.vector4_f32[0]),
    Scalar::abs(v.vector4_f32[1]),
    Scalar::abs(v.vector4_f32[2]),
    Scalar::abs(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::cos(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::cos(v.vector4_f32[0]),
    Scalar::cos(v.vector4_f32[1]),
    Scalar::cos(v.vector4_f32[2]),
    Scalar::cos(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::sin(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::sin(v.vector4_f32[0]),
    Scalar::sin(v.vector4_f32[1]),
    Scalar::sin(v.vector4_f32[2]),
    Scalar::sin(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::tan(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::tan(v.vector4_f32[0]),
    Scalar::tan(v.vector4_f32[1]),
    Scalar::tan(v.vector4_f32[2]),
    Scalar::tan(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::arcCos(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::arcCos(v.vector4_f32[0]),
    Scalar::arcCos(v.vector4_f32[1]),
    Scalar::arcCos(v.vector4_f32[2]),
    Scalar::arcCos(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::arcSin(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::arcSin(v.vector4_f32[0]),
    Scalar::arcSin(v.vector4_f32[1]),
    Scalar::arcSin(v.vector4_f32[2]),
    Scalar::arcSin(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::arcTan(CVector v) noexcept
{
  VectorF32 vResult = { { {
    Scalar::arcTan(v.vector4_f32[0]),
    Scalar::arcTan(v.vector4_f32[1]),
    Scalar::arcTan(v.vector4_f32[2]),
    Scalar::arcTan(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector Vector::covector1(CVector v, CVector e1) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = Vector::dot1(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = 1.0f;
  vResult.vector4_f32[2] = 1.0f;
  vResult.vector4_f32[3] = 1.0f;
  return vResult;
}

Vector Vector::dot1(CVector v1, CVector v2, CVector e1, Vector* cov) noexcept
{
  Vector covector = Vector::covector1(v2, e1);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = Vector::dot1(v1, covector);
  return vResult;
}

Vector Vector::dot1(CVector v1, CVector v2) noexcept
{
  float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0];

  Vector vResult = Vector::fill(fDot);
  return vResult;
}

Vector Vector::length1(CVector v) noexcept
{
  Vector vOrthoDotResult = Vector::dot1(v, v);
  float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  float32 fOrthoNormLength = Scalar::sqrt(fOrthoDotResult);
  Vector vResult = Vector::fill(fOrthoNormLength);
  return vResult;
}

Vector Vector::lengthSq1(CVector v) noexcept
{
  Vector vResult = Vector::dot1(v, v);
  return vResult;
}

Vector Vector::normalize1(CVector v) noexcept
{
  Vector vLength = Vector::lengthSq1(v);
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

Vector Vector::cos1(CVector v1, CVector v2) noexcept
{
  Vector vDot = Vector::dot1(v1, v2);
  Vector vLength1 = Vector::lengthSq1(v1);
  Vector vLength2 = Vector::lengthSq1(v2);
  float32 fDot = vDot.vector4_f32[0];
  float32 fLength1 = vLength1.vector4_f32[0];
  float32 fLength2 = vLength2.vector4_f32[0];
  float32 fMul = fLength1 * fLength2;
  float32 fSqrtMul = Scalar::sqrt(fMul);
  float32 fCos = fDot / fSqrtMul;
  Vector vResult = Vector::fill(fCos);
  return vResult;
}

Vector Vector::covector2(CVector v, CVector e1, CVector e2) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = Vector::dot2(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = Vector::dot2(v, e2).vector4_f32[0];
  vResult.vector4_f32[2] = 1.0f;
  vResult.vector4_f32[3] = 1.0f;

  return vResult;
}

Vector Vector::dot2(CVector v1, CVector v2, CVector e1, CVector e2, Vector* cov) noexcept
{
  Vector covector = Vector::covector2(v2, e1, e2);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = Vector::dot2(v1, covector);
  return vResult;
}

Vector Vector::dot2(CVector v1, CVector v2) noexcept
{
  float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0] +
    v1.vector4_f32[1] * v2.vector4_f32[1];

  Vector vResult = Vector::fill(fDot);
  return vResult;
}

Vector Vector::length2(CVector v) noexcept
{
  Vector vOrthoDotResult = Vector::dot2(v, v);
  float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  float32 fOrthoNormLength = Scalar::sqrt(fOrthoDotResult);
  Vector vResult = Vector::fill(fOrthoNormLength);
  return vResult;
}

Vector Vector::lengthSq2(CVector v) noexcept
{
  Vector vResult = Vector::dot2(v, v);
  return vResult;
}

Vector Vector::normalize2(CVector v) noexcept
{
  Vector vLength = Vector::lengthSq2(v);
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
Vector::cos2(CVector v1, CVector v2) noexcept
{
  Vector vDot = Vector::dot2(v1, v2);
  Vector vLength1 = Vector::lengthSq2(v1);
  Vector vLength2 = Vector::lengthSq2(v2);
  float32 fDot = vDot.vector4_f32[0];
  float32 fLength1 = vLength1.vector4_f32[0];
  float32 fLength2 = vLength2.vector4_f32[0];
  float32 fMul = fLength1 * fLength2;
  float32 fSqrtMul = Scalar::sqrt(fMul);
  float32 fCos = fDot / fSqrtMul;
  Vector vResult = Vector::fill(fCos);
  return vResult;
}

Vector Vector::covector3(CVector v, CVector e1, CVector e2, CVector e3) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = Vector::dot3(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = Vector::dot3(v, e2).vector4_f32[0];
  vResult.vector4_f32[2] = Vector::dot3(v, e3).vector4_f32[0];
  vResult.vector4_f32[3] = 1.0f;

  return vResult;
}

Vector Vector::dot3(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3, Vector* cov) noexcept
{
  Vector covector = Vector::covector3(v2, e1, e2, e3);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = Vector::dot3(v1, covector);
  return vResult;
}

Vector Vector::dot3(CVector v1, CVector v2) noexcept
{
  float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0] +
    v1.vector4_f32[1] * v2.vector4_f32[1] +
    v1.vector4_f32[2] * v2.vector4_f32[2];

  Vector vResult = Vector::fill(fDot);
  return vResult;
}

Vector Vector::length3(CVector v) noexcept
{
  Vector vOrthoDotResult = Vector::dot3(v, v);
  float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  float32 fOrthoNormLength = Scalar::sqrt(fOrthoDotResult);
  Vector vResult = Vector::fill(fOrthoNormLength);
  return vResult;
}

Vector Vector::lengthSq3(CVector v) noexcept
{
  Vector vResult = Vector::dot3(v, v);
  return vResult;
}

Vector Vector::normalize3(CVector v) noexcept
{
  Vector vLength = Vector::lengthSq3(v);
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

Vector Vector::cos3(CVector v1, CVector v2) noexcept
{
  Vector vDot = Vector::dot3(v1, v2);
  Vector vLength1 = Vector::lengthSq3(v1);
  Vector vLength2 = Vector::lengthSq3(v2);
  float32 fDot = vDot.vector4_f32[0];
  float32 fLength1 = vLength1.vector4_f32[0];
  float32 fLength2 = vLength2.vector4_f32[0];
  float32 fMul = fLength1 * fLength2;
  float32 fSqrtMul = Scalar::sqrt(fMul);
  float32 fCos = fDot / fSqrtMul;
  Vector vResult = Vector::fill(fCos);
  return vResult;
}

Vector Vector::covector4(CVector v, CVector e1, CVector e2, CVector e3, CVector e4) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = Vector::dot4(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = Vector::dot4(v, e2).vector4_f32[0];
  vResult.vector4_f32[2] = Vector::dot4(v, e3).vector4_f32[0];
  vResult.vector4_f32[3] = Vector::dot4(v, e4).vector4_f32[0];

  return vResult;
}

Vector Vector::dot4(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3, CVector e4, Vector* cov) noexcept
{
  Vector covector = Vector::covector4(v2, e1, e2, e3, e4);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = Vector::dot4(v1, covector);
  return vResult;
}

Vector Vector::dot4(CVector v1, CVector v2) noexcept
{
  float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0] +
    v1.vector4_f32[1] * v2.vector4_f32[1] +
    v1.vector4_f32[2] * v2.vector4_f32[2] +
    v1.vector4_f32[3] * v2.vector4_f32[3];

  Vector vResult = Vector::fill(fDot);
  return vResult;
}

Vector Vector::length4(CVector v) noexcept
{
  Vector vOrthoDotResult = Vector::dot4(v, v);
  float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  float32 fOrthoNormLength = Scalar::sqrt(fOrthoDotResult);
  Vector vResult = Vector::fill(fOrthoNormLength);
  return vResult;
}

Vector Vector::lengthSq4(CVector v) noexcept
{
  Vector vResult = Vector::dot4(v, v);
  return vResult;
}

Vector Vector::normalize4(CVector v) noexcept
{
  Vector vLength = Vector::lengthSq4(v);
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

Vector Vector::cos4(CVector v1, CVector v2) noexcept
{
  Vector vDot = Vector::dot4(v1, v2);
  Vector vLength1 = Vector::lengthSq4(v1);
  Vector vLength2 = Vector::lengthSq4(v2);
  float32 fDot = vDot.vector4_f32[0];
  float32 fLength1 = vLength1.vector4_f32[0];
  float32 fLength2 = vLength2.vector4_f32[0];
  float32 fMul = fLength1 * fLength2;
  float32 fSqrtMul = Scalar::sqrt(fMul);
  float32 fCos = fDot / fSqrtMul;
  Vector vResult = Vector::fill(fCos);
  return vResult;
}

Vector Vector::project1(CVector v1, CVector v2) noexcept
{
  float32 x = Vector::getX(Vector::dot1(v1, v2));
  float32 y = Vector::getX(Vector::lengthSq1(v2));
  Vector vResult = Vector::scale(v2, x / y);
  return vResult;
}

Vector Vector::reject1(CVector v1, CVector v2) noexcept
{
  Vector vProject = Vector::project1(v1, v2);
  Vector vResult = Vector::subtract(v1, vProject);
  return vResult;
}

Vector Vector::project2(CVector v1, CVector v2) noexcept
{
  float32 x = Vector::getX(Vector::dot2(v1, v2));
  float32 y = Vector::getX(Vector::lengthSq2(v2));
  Vector vResult = Vector::scale(v2, x / y);
  return vResult;
}

Vector Vector::reject2(CVector v1, CVector v2) noexcept
{
  Vector vProject = Vector::project2(v1, v2);
  Vector vResult = Vector::subtract(v1, vProject);
  return vResult;
}

Vector Vector::project3(CVector v1, CVector v2) noexcept
{
  float32 x = Vector::getX(Vector::dot3(v1, v2));
  float32 y = Vector::getX(Vector::lengthSq3(v2));
  Vector vResult = Vector::scale(v2, x / y);
  return vResult;
}

Vector Vector::reject3(CVector v1, CVector v2) noexcept
{
  Vector vProject = Vector::project3(v1, v2);
  Vector vResult = Vector::subtract(v1, vProject);
  return vResult;
}

Vector Vector::project4(CVector v1, CVector v2) noexcept
{
  float32 x = Vector::getX(Vector::dot4(v1, v2));
  float32 y = Vector::getX(Vector::lengthSq4(v2));
  Vector vResult = Vector::scale(v2, x / y);
  return vResult;
}

Vector Vector::reject4(CVector v1, CVector v2) noexcept
{
  Vector vProject = Vector::project4(v1, v2);
  Vector vResult = Vector::subtract(v1, vProject);
  return vResult;
}

Vector Vector::tripleProduct3(CVector v1, CVector v2, CVector v3, CVector e1, CVector e2, CVector e3) noexcept
{
  float32 x = Vector::getX(Vector::tripleProduct3(v1, v2, v3));
  float32 y = Vector::getX(Vector::tripleProduct3(e1, e2, e3));
  Vector vResult = Vector::fill(x * y);
  return vResult;
}

Vector Vector::cross3(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3) noexcept
{
  Vector vResult;
  {
    float32 s1 = v1.vector4_f32[1] * v2.vector4_f32[2] - v1.vector4_f32[2] * v2.vector4_f32[1];
    float32 s2 = v1.vector4_f32[2] * v2.vector4_f32[0] - v1.vector4_f32[0] * v2.vector4_f32[2];
    float32 s3 = v1.vector4_f32[0] * v2.vector4_f32[1] - v1.vector4_f32[1] * v2.vector4_f32[0];
    vResult = Vector::scale(e1, s1);
    vResult = Vector::add(vResult, Vector::scale(e2, s2));
    vResult = Vector::add(vResult, Vector::scale(e3, s3));
  }
  return vResult;
}

Vector Vector::tripleProduct3(CVector v1, CVector v2, CVector v3) noexcept
{
  float32 x;
  {
    x = v1.vector4_f32[0] * (v2.vector4_f32[1] * v3.vector4_f32[2] - v2.vector4_f32[2] * v3.vector4_f32[1]);
    x += v1.vector4_f32[1] * (v2.vector4_f32[2] * v3.vector4_f32[0] - v2.vector4_f32[0] * v3.vector4_f32[2]);
    x += v1.vector4_f32[2] * (v2.vector4_f32[0] * v3.vector4_f32[1] - v2.vector4_f32[1] * v3.vector4_f32[0]);
  }

  Vector vResult = Vector::fill(x);
  return vResult;
}

Vector Vector::cross3(CVector v1, CVector v2) noexcept
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

}