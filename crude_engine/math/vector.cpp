module crude.math.vector;

#if defined(_CRUDE_AVX_INTRINSICS) && defined(_CRUDE_FAVOR_INTEL)
#define CRUDE_PERMUTE_PS( v, c ) _mm_permute_ps((v), c )
#else
#define CRUDE_PERMUTE_PS( v, c ) _mm_shuffle_ps((v), (v), c )
#endif

namespace crude::math
{

Vector svector::zero() noexcept
{
  VectorF32 vResult = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
  return vResult;
}

Vector svector::set(core::float32 x, core::float32 y, core::float32 z, core::float32 w) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  VectorF32 vResult = { { { x, y, z, w } } };
  return vResult.v;
#elif defined(_CRUDE_SSE_INTRINSICS)
  return _mm_setzero_ps();
#endif
}

Vector svector::setInt(core::uint32 x, core::uint32 y, core::uint32 z, core::uint32 w) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  VectorU32 vResult = { { { x, y, z, w } } };
  return vResult;
#elif defined(_CRUDE_SSE_INTRINSICS)
  __m128i v = _mm_set_epi32(static_cast<int32>(w), static_cast<int32>(z), static_cast<int32>(y), static_cast<int32>(x));
  return _mm_castsi128_ps(v);
#endif
}

Vector svector::fill(core::float32 value) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  VectorF32 vResult = { { { value, value, value, value } } };
  return vResult.v;
#elif defined(_CRUDE_SSE_INTRINSICS)
  return _mm_set_ps1(value);
#endif

}

Vector svector::fillInt(core::uint32 value) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  VectorU32 vResult = { { { value, value, value, value } } };
  return vResult.v;
#elif defined(_CRUDE_SSE_INTRINSICS)
  __m128i v = _mm_set1_epi32(static_cast<int32>(value));
  return _mm_castsi128_ps(v);
#endif
}

Vector svector::trueInt() noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  VectorU32 vResult = { { { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU } } };
  return vResult.v;
#elif defined(_CRUDE_SSE_INTRINSICS)
  __m128i v = _mm_set1_epi32(-1);
  return _mm_castsi128_ps(v);
#endif
}

Vector svector::falseInt() noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  VectorU32 vResult = { { { 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U } } };
  return vResult.v;
#elif defined(_CRUDE_SSE_INTRINSICS)
  return _mm_set_ps1(0);
#endif
}

Vector svector::splatX(CVector v) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[0];
  return vResult;
#elif defined(_CRUDE_SSE_INTRINSICS)
  return CRUDE_PERMUTE_PS(v, _MM_SHUFFLE(0, 0, 0, 0));
#endif
}

Vector svector::splatY(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[1];
  return vResult;
}

Vector svector::splatZ(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[2];
  return vResult;
}

Vector svector::splatW(CVector v) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = v.vector4_f32[3];
  return vResult;
}

Vector svector::splatOne() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = 1.0f;
  return vResult;
}

Vector svector::splatInfinity() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = 0x7F800000;
  return vResult;
}

Vector svector::splatQNaN() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0]
    = vResult.vector4_f32[1]
    = vResult.vector4_f32[2]
    = vResult.vector4_f32[3]
    = 0x7FC00000;
  return vResult;
}

Vector svector::splatEpsilon() noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] =
    vResult.vector4_f32[1] =
    vResult.vector4_f32[2] =
    vResult.vector4_f32[3] = 0x34000000;
  return vResult;

}

core::float32 svector::getX(CVector v) noexcept
{
  return v.vector4_f32[0];
}

core::float32 svector::getY(CVector v) noexcept
{
  return v.vector4_f32[1];
}

core::float32 svector::getZ(CVector v) noexcept
{
  return v.vector4_f32[2];
}

core::float32 svector::getW(CVector v) noexcept
{
  return v.vector4_f32[3];
}

core::uint32 svector::getIntX(CVector v) noexcept
{
  return v.vector4_u32[0];
}

core::uint32 svector::getIntY(CVector v) noexcept
{
  return v.vector4_u32[1];
}

core::uint32 svector::getIntZ(CVector v) noexcept
{
  return v.vector4_u32[2];
}

core::uint32 svector::getIntW(CVector v) noexcept
{
  return v.vector4_u32[3];
}

Vector svector::setX(CVector v, core::float32 x) noexcept
{
  VectorF32 vResult = { { {
    x,
    v.vector4_f32[1],
    v.vector4_f32[2],
    v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::setY(CVector v, core::float32 y) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0],
    y,
    v.vector4_f32[2],
    v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::setZ(CVector v, core::float32 z) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0],
    v.vector4_f32[1],
    z,
    v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::setW(CVector v, core::float32 w) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0],
    v.vector4_f32[1],
    v.vector4_f32[2],
    w
  } } };
  return vResult.v;
}

Vector svector::setIntX(CVector v, core::uint32 x) noexcept
{
  VectorU32 vResult = { { {
    x,
    v.vector4_u32[1],
    v.vector4_u32[2],
    v.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector svector::setIntY(CVector v, core::uint32 y) noexcept
{
  VectorU32 vResult = { { {
    v.vector4_u32[0],
    y,
    v.vector4_u32[2],
    v.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector svector::setIntZ(CVector v, core::uint32 z) noexcept
{
  VectorU32 vResult = { { {
    v.vector4_u32[0],
    v.vector4_u32[1],
    z,
    v.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector svector::setIntW(CVector v, core::uint32 w) noexcept
{
  VectorU32 vResult = { { {
    v.vector4_u32[0],
    v.vector4_u32[1],
    v.vector4_u32[2],
    w
  } } };
  return vResult.v;
}

Vector svector::equal(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] == v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] == v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] == v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] == v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector svector::equalInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_u32[0] == v2.vector4_u32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[1] == v2.vector4_u32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[2] == v2.vector4_u32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[3] == v2.vector4_u32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector svector::nearEqual(CVector v1, CVector v2, CVector epsilon) noexcept
{
  core::float32 fDeltaX = v1.vector4_f32[0] - v2.vector4_f32[0];
  core::float32 fDeltaY = v1.vector4_f32[1] - v2.vector4_f32[1];
  core::float32 fDeltaZ = v1.vector4_f32[2] - v2.vector4_f32[2];
  core::float32 fDeltaW = v1.vector4_f32[3] - v2.vector4_f32[3];

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

Vector svector::notEqual(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] != v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] != v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] != v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] != v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector svector::notEqualInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_u32[0] != v2.vector4_u32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[1] != v2.vector4_u32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[2] != v2.vector4_u32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_u32[3] != v2.vector4_u32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector svector::greater(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] > v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] > v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] > v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] > v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector svector::greaterOrEqual(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] >= v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] >= v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] >= v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] >= v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector svector::less(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] < v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] < v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] < v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] < v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector svector::lessOrEqual(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_f32[0] <= v2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[1] <= v2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[2] <= v2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
    (v1.vector4_f32[3] <= v2.vector4_f32[3]) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector svector::inBounds(CVector v, CVector bounds) noexcept
{
  core::float32 absBoundsX = bounds.vector4_f32[0] > 0.0f ? bounds.vector4_f32[0] : -bounds.vector4_f32[0];
  core::float32 absBoundsY = bounds.vector4_f32[1] > 0.0f ? bounds.vector4_f32[1] : -bounds.vector4_f32[1];
  core::float32 absBoundsZ = bounds.vector4_f32[2] > 0.0f ? bounds.vector4_f32[2] : -bounds.vector4_f32[2];
  core::float32 absBoundsW = bounds.vector4_f32[3] > 0.0f ? bounds.vector4_f32[3] : -bounds.vector4_f32[3];

  VectorU32 vResult = { { {
    (v.vector4_f32[0] <= absBoundsX && v.vector4_f32[0] >= -absBoundsX) ? 0xFFFFFFFF : 0,
    (v.vector4_f32[1] <= absBoundsY && v.vector4_f32[1] >= -absBoundsY) ? 0xFFFFFFFF : 0,
    (v.vector4_f32[2] <= absBoundsZ && v.vector4_f32[2] >= -absBoundsZ) ? 0xFFFFFFFF : 0,
    (v.vector4_f32[3] <= absBoundsW && v.vector4_f32[3] >= -absBoundsW) ? 0xFFFFFFFF : 0
  } } };
  return vResult.v;
}

Vector svector::isNaN(CVector v) noexcept
{
  VectorU32 vResult = { { {
    (v.vector4_f32[0] == 0x7FC00000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[1] == 0x7FC00000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[2] == 0x7FC00000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[3] == 0x7FC00000) ? 0xFFFFFFFFU : 0
  } } };
  return vResult.v;
}

Vector svector::isInfinite(CVector v) noexcept
{
  VectorU32 vResult = { { {
    (v.vector4_f32[0] == 0x7F800000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[1] == 0x7F800000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[2] == 0x7F800000) ? 0xFFFFFFFFU : 0,
    (v.vector4_f32[3] == 0x7F800000) ? 0xFFFFFFFFU : 0
  } } };
  return vResult.v;
}

Vector svector::min(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    (v1.vector4_f32[0] < v2.vector4_f32[0]) ? v1.vector4_f32[0] : v2.vector4_f32[0],
    (v1.vector4_f32[1] < v2.vector4_f32[1]) ? v1.vector4_f32[1] : v2.vector4_f32[1],
    (v1.vector4_f32[2] < v2.vector4_f32[2]) ? v1.vector4_f32[2] : v2.vector4_f32[2],
    (v1.vector4_f32[3] < v2.vector4_f32[3]) ? v1.vector4_f32[3] : v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::max(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    (v1.vector4_f32[0] > v2.vector4_f32[0]) ? v1.vector4_f32[0] : v2.vector4_f32[0],
    (v1.vector4_f32[1] > v2.vector4_f32[1]) ? v1.vector4_f32[1] : v2.vector4_f32[1],
    (v1.vector4_f32[2] > v2.vector4_f32[2]) ? v1.vector4_f32[2] : v2.vector4_f32[2],
    (v1.vector4_f32[3] > v2.vector4_f32[3]) ? v1.vector4_f32[3] : v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::round(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::round(v.vector4_f32[0]),
    scalar::round(v.vector4_f32[1]),
    scalar::round(v.vector4_f32[2]),
    scalar::round(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::floor(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::floor(v.vector4_f32[0]),
    scalar::floor(v.vector4_f32[1]),
    scalar::floor(v.vector4_f32[2]),
    scalar::floor(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::ceil(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::ceil(v.vector4_f32[0]),
    scalar::ceil(v.vector4_f32[1]),
    scalar::ceil(v.vector4_f32[2]),
    scalar::ceil(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::trunc(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::trunc(v.vector4_f32[0]),
    scalar::trunc(v.vector4_f32[1]),
    scalar::trunc(v.vector4_f32[2]),
    scalar::trunc(v.vector4_f32[3])
  } } };
  return vResult.v;
}


Vector svector::clamp(CVector v, CVector vmin, CVector vmax) noexcept
{
  VectorF32 vResult = { { {
    scalar::clamp(v.vector4_f32[0], vmin.vector4_f32[0], vmax.vector4_f32[0]),
    scalar::clamp(v.vector4_f32[1], vmin.vector4_f32[1], vmax.vector4_f32[1]),
    scalar::clamp(v.vector4_f32[2], vmin.vector4_f32[2], vmax.vector4_f32[2]),
    scalar::clamp(v.vector4_f32[3], vmin.vector4_f32[3], vmax.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::andInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    v1.vector4_u32[0] & v2.vector4_u32[0],
    v1.vector4_u32[1] & v2.vector4_u32[1],
    v1.vector4_u32[2] & v2.vector4_u32[2],
    v1.vector4_u32[3] & v2.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector svector::orInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    v1.vector4_u32[0] | v2.vector4_u32[0],
    v1.vector4_u32[1] | v2.vector4_u32[1],
    v1.vector4_u32[2] | v2.vector4_u32[2],
    v1.vector4_u32[3] | v2.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector svector::norInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    ~(v1.vector4_u32[0] | v2.vector4_u32[0]),
    ~(v1.vector4_u32[1] | v2.vector4_u32[1]),
    ~(v1.vector4_u32[2] | v2.vector4_u32[2]),
    ~(v1.vector4_u32[3] | v2.vector4_u32[3])
  } } };
  return vResult.v;
}

Vector svector::xorInt(CVector v1, CVector v2) noexcept
{
  VectorU32 vResult = { { {
    v1.vector4_u32[0] ^ v2.vector4_u32[0],
    v1.vector4_u32[1] ^ v2.vector4_u32[1],
    v1.vector4_u32[2] ^ v2.vector4_u32[2],
    v1.vector4_u32[3] ^ v2.vector4_u32[3]
  } } };
  return vResult.v;
}

Vector svector::select(CVector v1, CVector v2, CVector control) noexcept
{
  VectorU32 vResult = { { {
    (v1.vector4_u32[0] & ~control.vector4_u32[0]) | (v2.vector4_u32[0] & control.vector4_u32[0]),
    (v1.vector4_u32[1] & ~control.vector4_u32[1]) | (v2.vector4_u32[1] & control.vector4_u32[1]),
    (v1.vector4_u32[2] & ~control.vector4_u32[2]) | (v2.vector4_u32[2] & control.vector4_u32[2]),
    (v1.vector4_u32[3] & ~control.vector4_u32[3]) | (v2.vector4_u32[3] & control.vector4_u32[3])
  } } };
  return vResult.v;
}

Vector svector::negate(CVector v) noexcept
{
  VectorF32 vResult = { { {
    -v.vector4_f32[0],
    -v.vector4_f32[1],
    -v.vector4_f32[2],
    -v.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::add(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] + v2.vector4_f32[0],
    v1.vector4_f32[1] + v2.vector4_f32[1],
    v1.vector4_f32[2] + v2.vector4_f32[2],
    v1.vector4_f32[3] + v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::subtract(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] - v2.vector4_f32[0],
    v1.vector4_f32[1] - v2.vector4_f32[1],
    v1.vector4_f32[2] - v2.vector4_f32[2],
    v1.vector4_f32[3] - v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::multiply(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] * v2.vector4_f32[0],
    v1.vector4_f32[1] * v2.vector4_f32[1],
    v1.vector4_f32[2] * v2.vector4_f32[2],
    v1.vector4_f32[3] * v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::multiplyAdd(CVector v1, CVector v2, CVector v3) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] * v2.vector4_f32[0] + v3.vector4_f32[0],
    v1.vector4_f32[1] * v2.vector4_f32[1] + v3.vector4_f32[1],
    v1.vector4_f32[2] * v2.vector4_f32[2] + v3.vector4_f32[2],
    v1.vector4_f32[3] * v2.vector4_f32[3] + v3.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::divide(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] / v2.vector4_f32[0],
    v1.vector4_f32[1] / v2.vector4_f32[1],
    v1.vector4_f32[2] / v2.vector4_f32[2],
    v1.vector4_f32[3] / v2.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::divideAdd(CVector v1, CVector v2, CVector v3) noexcept
{
  VectorF32 vResult = { { {
    v1.vector4_f32[0] / v2.vector4_f32[0] + v3.vector4_f32[0],
    v1.vector4_f32[1] / v2.vector4_f32[1] + v3.vector4_f32[1],
    v1.vector4_f32[2] / v2.vector4_f32[2] + v3.vector4_f32[2],
    v1.vector4_f32[3] / v2.vector4_f32[3] + v3.vector4_f32[3]
  } } };
  return vResult.v;
}

Vector svector::scale(CVector v, core::float32 s) noexcept
{
  VectorF32 vResult = { { {
    v.vector4_f32[0] * s,
    v.vector4_f32[1] * s,
    v.vector4_f32[2] * s,
    v.vector4_f32[3] * s
  } } };
  return vResult.v;
}

Vector svector::sqrt(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::sqrt(v.vector4_f32[0]),
    scalar::sqrt(v.vector4_f32[1]),
    scalar::sqrt(v.vector4_f32[2]),
    scalar::sqrt(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::exp2(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::exp2(v.vector4_f32[0]),
    scalar::exp2(v.vector4_f32[1]),
    scalar::exp2(v.vector4_f32[2]),
    scalar::exp2(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::exp(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::exp(v.vector4_f32[0]),
    scalar::exp(v.vector4_f32[1]),
    scalar::exp(v.vector4_f32[2]),
    scalar::exp(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::log2(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::log2(v.vector4_f32[0]),
    scalar::log2(v.vector4_f32[1]),
    scalar::log2(v.vector4_f32[2]),
    scalar::log2(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::log10(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::log10(v.vector4_f32[0]),
    scalar::log10(v.vector4_f32[1]),
    scalar::log10(v.vector4_f32[2]),
    scalar::log10(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::log(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::log(v.vector4_f32[0]),
    scalar::log(v.vector4_f32[1]),
    scalar::log(v.vector4_f32[2]),
    scalar::log(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::pow(CVector v1, CVector v2) noexcept
{
  VectorF32 vResult = { { {
    scalar::pow(v1.vector4_f32[0], v2.vector4_f32[0]),
    scalar::pow(v1.vector4_f32[1], v2.vector4_f32[1]),
    scalar::pow(v1.vector4_f32[2], v2.vector4_f32[2]),
    scalar::pow(v1.vector4_f32[3], v2.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::abs(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::abs(v.vector4_f32[0]),
    scalar::abs(v.vector4_f32[1]),
    scalar::abs(v.vector4_f32[2]),
    scalar::abs(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::cos(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::cos(v.vector4_f32[0]),
    scalar::cos(v.vector4_f32[1]),
    scalar::cos(v.vector4_f32[2]),
    scalar::cos(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::sin(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::sin(v.vector4_f32[0]),
    scalar::sin(v.vector4_f32[1]),
    scalar::sin(v.vector4_f32[2]),
    scalar::sin(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::tan(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::tan(v.vector4_f32[0]),
    scalar::tan(v.vector4_f32[1]),
    scalar::tan(v.vector4_f32[2]),
    scalar::tan(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::arcCos(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::arcCos(v.vector4_f32[0]),
    scalar::arcCos(v.vector4_f32[1]),
    scalar::arcCos(v.vector4_f32[2]),
    scalar::arcCos(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::arcSin(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::arcSin(v.vector4_f32[0]),
    scalar::arcSin(v.vector4_f32[1]),
    scalar::arcSin(v.vector4_f32[2]),
    scalar::arcSin(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::arcTan(CVector v) noexcept
{
  VectorF32 vResult = { { {
    scalar::arcTan(v.vector4_f32[0]),
    scalar::arcTan(v.vector4_f32[1]),
    scalar::arcTan(v.vector4_f32[2]),
    scalar::arcTan(v.vector4_f32[3])
  } } };
  return vResult.v;
}

Vector svector::covector1(CVector v, CVector e1) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = svector::dot1(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = 1.0f;
  vResult.vector4_f32[2] = 1.0f;
  vResult.vector4_f32[3] = 1.0f;
  return vResult;
}

Vector svector::dot1(CVector v1, CVector v2, CVector e1, Vector* cov) noexcept
{
  Vector covector = svector::covector1(v2, e1);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = svector::dot1(v1, covector);
  return vResult;
}

Vector svector::dot1(CVector v1, CVector v2) noexcept
{
  core::float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0];

  Vector vResult = svector::fill(fDot);
  return vResult;
}

Vector svector::length1(CVector v) noexcept
{
  Vector vOrthoDotResult = svector::dot1(v, v);
  core::float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  core::float32 fOrthoNormLength = scalar::sqrt(fOrthoDotResult);
  Vector vResult = svector::fill(fOrthoNormLength);
  return vResult;
}

Vector svector::lengthSq1(CVector v) noexcept
{
  Vector vResult = svector::dot1(v, v);
  return vResult;
}

Vector svector::normalize1(CVector v) noexcept
{
  Vector vLength = svector::lengthSq1(v);
  core::float32 fLength = vLength.vector4_f32[0];
  core::float32 fInvLength;

  if (fLength > 0)
  {
    fInvLength = 1.0f / fLength;
  }
  else
  {
    fInvLength = CMAXF32;
  }

  Vector vResult;
  vResult.vector4_f32[0] = v.vector4_f32[0] * fInvLength;
  vResult.vector4_f32[1] = v.vector4_f32[1];
  vResult.vector4_f32[2] = v.vector4_f32[2];
  vResult.vector4_f32[3] = v.vector4_f32[3];

  return vResult;
}

Vector svector::cos1(CVector v1, CVector v2) noexcept
{
  Vector vDot = svector::dot1(v1, v2);
  Vector vLength1 = svector::lengthSq1(v1);
  Vector vLength2 = svector::lengthSq1(v2);
  core::float32 fDot = vDot.vector4_f32[0];
  core::float32 fLength1 = vLength1.vector4_f32[0];
  core::float32 fLength2 = vLength2.vector4_f32[0];
  core::float32 fMul = fLength1 * fLength2;
  core::float32 fSqrtMul = scalar::sqrt(fMul);
  core::float32 fCos = fDot / fSqrtMul;
  Vector vResult = svector::fill(fCos);
  return vResult;
}

Vector svector::covector2(CVector v, CVector e1, CVector e2) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = svector::dot2(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = svector::dot2(v, e2).vector4_f32[0];
  vResult.vector4_f32[2] = 1.0f;
  vResult.vector4_f32[3] = 1.0f;

  return vResult;
}

Vector svector::dot2(CVector v1, CVector v2, CVector e1, CVector e2, Vector* cov) noexcept
{
  Vector covector = svector::covector2(v2, e1, e2);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = svector::dot2(v1, covector);
  return vResult;
}

Vector svector::dot2(CVector v1, CVector v2) noexcept
{
  core::float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0] +
    v1.vector4_f32[1] * v2.vector4_f32[1];

  Vector vResult = svector::fill(fDot);
  return vResult;
}

Vector svector::length2(CVector v) noexcept
{
  Vector vOrthoDotResult = svector::dot2(v, v);
  core::float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  core::float32 fOrthoNormLength = scalar::sqrt(fOrthoDotResult);
  Vector vResult = svector::fill(fOrthoNormLength);
  return vResult;
}

Vector svector::lengthSq2(CVector v) noexcept
{
  Vector vResult = svector::dot2(v, v);
  return vResult;
}

Vector svector::normalize2(CVector v) noexcept
{
  Vector vLength = svector::lengthSq2(v);
  core::float32 fLength = vLength.vector4_f32[0];
  core::float32 fInvLength;

  if (fLength > 0.0f)
  {
    fInvLength = 1.0f / fLength;
  }
  else
  {
    fInvLength = CMAXF32;
  }

  Vector vResult;
  vResult.vector4_f32[0] = v.vector4_f32[0] * fInvLength;
  vResult.vector4_f32[1] = v.vector4_f32[1] * fInvLength;
  vResult.vector4_f32[2] = v.vector4_f32[2];
  vResult.vector4_f32[3] = v.vector4_f32[3];

  return vResult;
}

Vector
svector::cos2(CVector v1, CVector v2) noexcept
{
  Vector vDot = svector::dot2(v1, v2);
  Vector vLength1 = svector::lengthSq2(v1);
  Vector vLength2 = svector::lengthSq2(v2);
  core::float32 fDot = vDot.vector4_f32[0];
  core::float32 fLength1 = vLength1.vector4_f32[0];
  core::float32 fLength2 = vLength2.vector4_f32[0];
  core::float32 fMul = fLength1 * fLength2;
  core::float32 fSqrtMul = scalar::sqrt(fMul);
  core::float32 fCos = fDot / fSqrtMul;
  Vector vResult = svector::fill(fCos);
  return vResult;
}

Vector svector::covector3(CVector v, CVector e1, CVector e2, CVector e3) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = svector::dot3(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = svector::dot3(v, e2).vector4_f32[0];
  vResult.vector4_f32[2] = svector::dot3(v, e3).vector4_f32[0];
  vResult.vector4_f32[3] = 1.0f;

  return vResult;
}

Vector svector::dot3(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3, Vector* cov) noexcept
{
  Vector covector = svector::covector3(v2, e1, e2, e3);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = svector::dot3(v1, covector);
  return vResult;
}

Vector svector::dot3(CVector v1, CVector v2) noexcept
{
  core::float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0] +
    v1.vector4_f32[1] * v2.vector4_f32[1] +
    v1.vector4_f32[2] * v2.vector4_f32[2];

  Vector vResult = svector::fill(fDot);
  return vResult;
}

Vector svector::length3(CVector v) noexcept
{
  Vector vOrthoDotResult = svector::dot3(v, v);
  core::float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  core::float32 fOrthoNormLength = scalar::sqrt(fOrthoDotResult);
  Vector vResult = svector::fill(fOrthoNormLength);
  return vResult;
}

Vector svector::lengthSq3(CVector v) noexcept
{
  Vector vResult = svector::dot3(v, v);
  return vResult;
}

Vector svector::normalize3(CVector v) noexcept
{
  Vector vLength = svector::length3(v);
  core::float32 fLength = vLength.vector4_f32[0];
  core::float32 fInvLength;

  if (fLength > 0.0f)
  {
    fInvLength = 1.0f / fLength;
  }
  else
  {
    fInvLength = CMAXF32;
  }

  Vector vResult;
  vResult.vector4_f32[0] = v.vector4_f32[0] * fInvLength;
  vResult.vector4_f32[1] = v.vector4_f32[1] * fInvLength;
  vResult.vector4_f32[2] = v.vector4_f32[2] * fInvLength;
  vResult.vector4_f32[3] = v.vector4_f32[3];

  return vResult;
}

Vector svector::cos3(CVector v1, CVector v2) noexcept
{
  Vector vDot = svector::dot3(v1, v2);
  Vector vLength1 = svector::lengthSq3(v1);
  Vector vLength2 = svector::lengthSq3(v2);
  core::float32 fDot = vDot.vector4_f32[0];
  core::float32 fLength1 = vLength1.vector4_f32[0];
  core::float32 fLength2 = vLength2.vector4_f32[0];
  core::float32 fMul = fLength1 * fLength2;
  core::float32 fSqrtMul = scalar::sqrt(fMul);
  core::float32 fCos = fDot / fSqrtMul;
  Vector vResult = svector::fill(fCos);
  return vResult;
}

Vector svector::covector4(CVector v, CVector e1, CVector e2, CVector e3, CVector e4) noexcept
{
  Vector vResult;
  vResult.vector4_f32[0] = svector::dot4(v, e1).vector4_f32[0];
  vResult.vector4_f32[1] = svector::dot4(v, e2).vector4_f32[0];
  vResult.vector4_f32[2] = svector::dot4(v, e3).vector4_f32[0];
  vResult.vector4_f32[3] = svector::dot4(v, e4).vector4_f32[0];

  return vResult;
}

Vector svector::dot4(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3, CVector e4, Vector* cov) noexcept
{
  Vector covector = svector::covector4(v2, e1, e2, e3, e4);

  if (cov != nullptr)
  {
    *cov = covector;
  }

  Vector vResult = svector::dot4(v1, covector);
  return vResult;
}

Vector svector::dot4(CVector v1, CVector v2) noexcept
{
  core::float32 fDot = v1.vector4_f32[0] * v2.vector4_f32[0] +
    v1.vector4_f32[1] * v2.vector4_f32[1] +
    v1.vector4_f32[2] * v2.vector4_f32[2] +
    v1.vector4_f32[3] * v2.vector4_f32[3];

  Vector vResult = svector::fill(fDot);
  return vResult;
}

Vector svector::length4(CVector v) noexcept
{
  Vector vOrthoDotResult = svector::dot4(v, v);
  core::float32 fOrthoDotResult = vOrthoDotResult.vector4_f32[0];
  core::float32 fOrthoNormLength = scalar::sqrt(fOrthoDotResult);
  Vector vResult = svector::fill(fOrthoNormLength);
  return vResult;
}

Vector svector::lengthSq4(CVector v) noexcept
{
  Vector vResult = svector::dot4(v, v);
  return vResult;
}

Vector svector::normalize4(CVector v) noexcept
{
  Vector vLength = svector::lengthSq4(v);
  core::float32 fLength = vLength.vector4_f32[0];
  core::float32 fInvLength;

  if (fLength > 0)
  {
    fInvLength = 1.0f / fLength;
  }
  else
  {
    fInvLength = CMAXF32;
  }

  Vector vResult;
  vResult.vector4_f32[0] = v.vector4_f32[0] * fInvLength;
  vResult.vector4_f32[1] = v.vector4_f32[1] * fInvLength;
  vResult.vector4_f32[2] = v.vector4_f32[2] * fInvLength;
  vResult.vector4_f32[3] = v.vector4_f32[3] * fInvLength;

  return vResult;
}

Vector svector::cos4(CVector v1, CVector v2) noexcept
{
  Vector vDot = svector::dot4(v1, v2);
  Vector vLength1 = svector::lengthSq4(v1);
  Vector vLength2 = svector::lengthSq4(v2);
  core::float32 fDot = vDot.vector4_f32[0];
  core::float32 fLength1 = vLength1.vector4_f32[0];
  core::float32 fLength2 = vLength2.vector4_f32[0];
  core::float32 fMul = fLength1 * fLength2;
  core::float32 fSqrtMul = scalar::sqrt(fMul);
  core::float32 fCos = fDot / fSqrtMul;
  Vector vResult = svector::fill(fCos);
  return vResult;
}

Vector svector::project1(CVector v1, CVector v2) noexcept
{
  core::float32 x = svector::getX(svector::dot1(v1, v2));
  core::float32 y = svector::getX(svector::lengthSq1(v2));
  Vector vResult = svector::scale(v2, x / y);
  return vResult;
}

Vector svector::reject1(CVector v1, CVector v2) noexcept
{
  Vector vProject = svector::project1(v1, v2);
  Vector vResult = svector::subtract(v1, vProject);
  return vResult;
}

Vector svector::project2(CVector v1, CVector v2) noexcept
{
  core::float32 x = svector::getX(svector::dot2(v1, v2));
  core::float32 y = svector::getX(svector::lengthSq2(v2));
  Vector vResult = svector::scale(v2, x / y);
  return vResult;
}

Vector svector::reject2(CVector v1, CVector v2) noexcept
{
  Vector vProject = svector::project2(v1, v2);
  Vector vResult = svector::subtract(v1, vProject);
  return vResult;
}

Vector svector::project3(CVector v1, CVector v2) noexcept
{
  core::float32 x = svector::getX(svector::dot3(v1, v2));
  core::float32 y = svector::getX(svector::lengthSq3(v2));
  Vector vResult = svector::scale(v2, x / y);
  return vResult;
}

Vector svector::reject3(CVector v1, CVector v2) noexcept
{
  Vector vProject = svector::project3(v1, v2);
  Vector vResult = svector::subtract(v1, vProject);
  return vResult;
}

Vector svector::project4(CVector v1, CVector v2) noexcept
{
  core::float32 x = svector::getX(svector::dot4(v1, v2));
  core::float32 y = svector::getX(svector::lengthSq4(v2));
  Vector vResult = svector::scale(v2, x / y);
  return vResult;
}

Vector svector::reject4(CVector v1, CVector v2) noexcept
{
  Vector vProject = svector::project4(v1, v2);
  Vector vResult = svector::subtract(v1, vProject);
  return vResult;
}

Vector svector::tripleProduct3(CVector v1, CVector v2, CVector v3, CVector e1, CVector e2, CVector e3) noexcept
{
  core::float32 x = svector::getX(svector::tripleProduct3(v1, v2, v3));
  core::float32 y = svector::getX(svector::tripleProduct3(e1, e2, e3));
  Vector vResult = svector::fill(x * y);
  return vResult;
}

Vector svector::cross3(CVector v1, CVector v2, CVector e1, CVector e2, CVector e3) noexcept
{
  Vector vResult;
  {
    core::float32 s1 = v1.vector4_f32[1] * v2.vector4_f32[2] - v1.vector4_f32[2] * v2.vector4_f32[1];
    core::float32 s2 = v1.vector4_f32[2] * v2.vector4_f32[0] - v1.vector4_f32[0] * v2.vector4_f32[2];
    core::float32 s3 = v1.vector4_f32[0] * v2.vector4_f32[1] - v1.vector4_f32[1] * v2.vector4_f32[0];
    vResult = svector::scale(e1, s1);
    vResult = svector::add(vResult, svector::scale(e2, s2));
    vResult = svector::add(vResult, svector::scale(e3, s3));
  }
  return vResult;
}

Vector svector::tripleProduct3(CVector v1, CVector v2, CVector v3) noexcept
{
  core::float32 x;
  {
    x = v1.vector4_f32[0] * (v2.vector4_f32[1] * v3.vector4_f32[2] - v2.vector4_f32[2] * v3.vector4_f32[1]);
    x += v1.vector4_f32[1] * (v2.vector4_f32[2] * v3.vector4_f32[0] - v2.vector4_f32[0] * v3.vector4_f32[2]);
    x += v1.vector4_f32[2] * (v2.vector4_f32[0] * v3.vector4_f32[1] - v2.vector4_f32[1] * v3.vector4_f32[0]);
  }

  Vector vResult = svector::fill(x);
  return vResult;
}

Vector svector::cross3(CVector v1, CVector v2) noexcept
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