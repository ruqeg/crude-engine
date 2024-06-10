#include <algorithm>

module crude.math.matrix;

import crude.math.scalar;
import crude.core.assert;

namespace crude::math
{

constexpr Matrix::Matrix(const Vector& r0, const Vector& r1, const Vector& r2, const Vector& r3) noexcept
  :
  r{r0, r1, r2, r3}
{}

Matrix::Matrix(core::float32 m00, core::float32 m01, core::float32 m02, core::float32 m03,
               core::float32 m10, core::float32 m11, core::float32 m12, core::float32 m13,
               core::float32 m20, core::float32 m21, core::float32 m22, core::float32 m23,
               core::float32 m30, core::float32 m31, core::float32 m32, core::float32 m33) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
  m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
  m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
  m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
#endif
}

bool smatrix::isIdentity(CMatrix m) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  return (m.m[0][0] == 1) && (m.m[1][1] == 1) && (m.m[2][2] == 1) && (m.m[3][3] == 1);
#endif
}

Matrix smatrix::multiply(CMatrix m1, CMatrix m2) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  Matrix mResult;

  core::float32 x = m1.m[0][0];
  core::float32 y = m1.m[0][1];
  core::float32 z = m1.m[0][2];
  core::float32 w = m1.m[0][3];

  mResult.m[0][0] = (m2.m[0][0] * x) + (m2.m[1][0] * y) + (m2.m[2][0] * z) + (m2.m[3][0] * w);
  mResult.m[0][1] = (m2.m[0][1] * x) + (m2.m[1][1] * y) + (m2.m[2][1] * z) + (m2.m[3][1] * w);
  mResult.m[0][2] = (m2.m[0][2] * x) + (m2.m[1][2] * y) + (m2.m[2][2] * z) + (m2.m[3][2] * w);
  mResult.m[0][3] = (m2.m[0][3] * x) + (m2.m[1][3] * y) + (m2.m[2][3] * z) + (m2.m[3][3] * w);
  
  x = m1.m[1][0];
  y = m1.m[1][1];
  z = m1.m[1][2];
  w = m1.m[1][3];
  mResult.m[1][0] = (m2.m[0][0] * x) + (m2.m[1][0] * y) + (m2.m[2][0] * z) + (m2.m[3][0] * w);
  mResult.m[1][1] = (m2.m[0][1] * x) + (m2.m[1][1] * y) + (m2.m[2][1] * z) + (m2.m[3][1] * w);
  mResult.m[1][2] = (m2.m[0][2] * x) + (m2.m[1][2] * y) + (m2.m[2][2] * z) + (m2.m[3][2] * w);

  mResult.m[1][3] = (m2.m[0][3] * x) + (m2.m[1][3] * y) + (m2.m[2][3] * z) + (m2.m[3][3] * w);
  x = m1.m[2][0];
  y = m1.m[2][1];
  z = m1.m[2][2];
  w = m1.m[2][3];
  mResult.m[2][0] = (m2.m[0][0] * x) + (m2.m[1][0] * y) + (m2.m[2][0] * z) + (m2.m[3][0] * w);
  mResult.m[2][1] = (m2.m[0][1] * x) + (m2.m[1][1] * y) + (m2.m[2][1] * z) + (m2.m[3][1] * w);
  mResult.m[2][2] = (m2.m[0][2] * x) + (m2.m[1][2] * y) + (m2.m[2][2] * z) + (m2.m[3][2] * w);
  mResult.m[2][3] = (m2.m[0][3] * x) + (m2.m[1][3] * y) + (m2.m[2][3] * z) + (m2.m[3][3] * w);

  x = m1.m[3][0];
  y = m1.m[3][1];
  z = m1.m[3][2];
  w = m1.m[3][3];
  mResult.m[3][0] = (m2.m[0][0] * x) + (m2.m[1][0] * y) + (m2.m[2][0] * z) + (m2.m[3][0] * w);
  mResult.m[3][1] = (m2.m[0][1] * x) + (m2.m[1][1] * y) + (m2.m[2][1] * z) + (m2.m[3][1] * w);
  mResult.m[3][2] = (m2.m[0][2] * x) + (m2.m[1][2] * y) + (m2.m[2][2] * z) + (m2.m[3][2] * w);
  mResult.m[3][3] = (m2.m[0][3] * x) + (m2.m[1][3] * y) + (m2.m[2][3] * z) + (m2.m[3][3] * w);

  return mResult;
#endif
}

Matrix smatrix::multiplyTranspose(CMatrix m1, CMatrix m2) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)

  Matrix mResult;

  float x = m2.m[0][0];
  float y = m2.m[1][0];
  float z = m2.m[2][0];
  float w = m2.m[3][0];
  mResult.m[0][0] = (m1.m[0][0] * x) + (m1.m[0][1] * y) + (m1.m[0][2] * z) + (m1.m[0][3] * w);
  mResult.m[0][1] = (m1.m[1][0] * x) + (m1.m[1][1] * y) + (m1.m[1][2] * z) + (m1.m[1][3] * w);
  mResult.m[0][2] = (m1.m[2][0] * x) + (m1.m[2][1] * y) + (m1.m[2][2] * z) + (m1.m[2][3] * w);
  mResult.m[0][3] = (m1.m[3][0] * x) + (m1.m[3][1] * y) + (m1.m[3][2] * z) + (m1.m[3][3] * w);
  
  x = m2.m[0][1];
  y = m2.m[1][1];
  z = m2.m[2][1];
  w = m2.m[3][1];
  mResult.m[1][0] = (m1.m[0][0] * x) + (m1.m[0][1] * y) + (m1.m[0][2] * z) + (m1.m[0][3] * w);
  mResult.m[1][1] = (m1.m[1][0] * x) + (m1.m[1][1] * y) + (m1.m[1][2] * z) + (m1.m[1][3] * w);
  mResult.m[1][2] = (m1.m[2][0] * x) + (m1.m[2][1] * y) + (m1.m[2][2] * z) + (m1.m[2][3] * w);
  mResult.m[1][3] = (m1.m[3][0] * x) + (m1.m[3][1] * y) + (m1.m[3][2] * z) + (m1.m[3][3] * w);
  
  x = m2.m[0][2];
  y = m2.m[1][2];
  z = m2.m[2][2];
  w = m2.m[3][2];
  mResult.m[2][0] = (m1.m[0][0] * x) + (m1.m[0][1] * y) + (m1.m[0][2] * z) + (m1.m[0][3] * w);
  mResult.m[2][1] = (m1.m[1][0] * x) + (m1.m[1][1] * y) + (m1.m[1][2] * z) + (m1.m[1][3] * w);
  mResult.m[2][2] = (m1.m[2][0] * x) + (m1.m[2][1] * y) + (m1.m[2][2] * z) + (m1.m[2][3] * w);
  mResult.m[2][3] = (m1.m[3][0] * x) + (m1.m[3][1] * y) + (m1.m[3][2] * z) + (m1.m[3][3] * w);
  
  x = m2.m[0][3];
  y = m2.m[1][3];
  z = m2.m[2][3];
  w = m2.m[3][3];
  mResult.m[3][0] = (m1.m[0][0] * x) + (m1.m[0][1] * y) + (m1.m[0][2] * z) + (m1.m[0][3] * w);
  mResult.m[3][1] = (m1.m[1][0] * x) + (m1.m[1][1] * y) + (m1.m[1][2] * z) + (m1.m[1][3] * w);
  mResult.m[3][2] = (m1.m[2][0] * x) + (m1.m[2][1] * y) + (m1.m[2][2] * z) + (m1.m[2][3] * w);
  mResult.m[3][3] = (m1.m[3][0] * x) + (m1.m[3][1] * y) + (m1.m[3][2] * z) + (m1.m[3][3] * w);

  return mResult;
#endif
}

Matrix smatrix::transpose(CMatrix m) noexcept
{
  // !TODO optimize
  Vector r0 = svector::set(m._11, m._21, m._31, m._41);
  Vector r1 = svector::set(m._12, m._22, m._32, m._42);
  Vector r2 = svector::set(m._13, m._23, m._33, m._43);
  Vector r3 = svector::set(m._14, m._24, m._34, m._44);
  return Matrix(r0, r1, r2, r3);
}

Matrix smatrix::inverse(CMatrix m) noexcept
{
  // !TODO optimize it
  const Vector a = m.r[0];
  const Vector b = m.r[1];
  const Vector c = m.r[2];
  const Vector d = m.r[3];
  
  const core::float32& x = m._41; 
  const core::float32& y = m._42; 
  const core::float32& z = m._43; 
  const core::float32& w = m._44; 
  
  Vector s = svector::cross3(a, b);
  Vector t = svector::cross3(c, d);
  Vector u = svector::subtract(svector::scale(a, y), svector::scale(b, x));
  Vector v = svector::subtract(svector::scale(c, w), svector::scale(d, z));
  
  core::float32 invDet = 1.0f / svector::getX(svector::add(svector::dot3(s, v), svector::dot3(t, u)));
  s = svector::scale(s, invDet);
  t = svector::scale(t, invDet);
  u = svector::scale(u, invDet);
  v = svector::scale(v, invDet);
  
  Vector r0 = svector::add(svector::cross3(b, v), svector::scale(t, y));
  Vector r1 = svector::subtract(svector::cross3(v, a), svector::scale(t, x));
  Vector r2 = svector::add(svector::cross3(d, u), svector::scale(s, w));
  Vector r3 = svector::subtract(svector::cross3(u, c), svector::scale(s, z));
  
  return (smatrix::set(svector::getX(r0), svector::getY(r0), svector::getZ(r0), -svector::getX(svector::dot3(b, t)),
                       svector::getX(r1), svector::getY(r1), svector::getZ(r1), svector::getX(svector::dot3(a, t)),
                       svector::getX(r2), svector::getY(r2), svector::getZ(r2), -svector::getX(svector::dot3(d, s)),
                       svector::getX(r3), svector::getY(r3), svector::getZ(r3), svector::getX(svector::dot3(c, s))));
}

Vector smatrix::determinant(CMatrix m) noexcept
{
  core::assert(false && "!TODO");
  return Vector();
}

Matrix smatrix::identity() noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  Matrix m;
  m.r[0] = svector::set(1.f, 0.f, 0.f, 0.f);
  m.r[1] = svector::set(0.f, 1.f, 0.f, 0.f);
  m.r[2] = svector::set(0.f, 0.f, 1.f, 0.f);
  m.r[3] = svector::set(0.f, 0.f, 0.f, 1.f);
  return m;
#endif
}

Matrix smatrix::set(core::float32 m00, core::float32 m01, core::float32 m02, core::float32 m03, core::float32 m10, core::float32 m11, core::float32 m12, core::float32 m13, core::float32 m20, core::float32 m21, core::float32 m22, core::float32 m23, core::float32 m30, core::float32 m31, core::float32 m32, core::float32 m33) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  Matrix m;
  m.m[0][0] = m00; m.m[0][1] = m01; m.m[0][2] = m02; m.m[0][3] = m03;
  m.m[1][0] = m10; m.m[1][1] = m11; m.m[1][2] = m12; m.m[1][3] = m13;
  m.m[2][0] = m20; m.m[2][1] = m21; m.m[2][2] = m22; m.m[2][3] = m23;
  m.m[3][0] = m30; m.m[3][1] = m31; m.m[3][2] = m32; m.m[3][3] = m33;
  return m;
#endif
}

Matrix smatrix::translation(core::float32 offsetX, core::float32 offsetY, core::float32 offsetZ) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  Matrix m;
  m.m[0][0] = 1.0f;
  m.m[0][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[0][3] = 0.0f;

  m.m[1][0] = 0.0f;
  m.m[1][1] = 1.0f;
  m.m[1][2] = 0.0f;
  m.m[1][3] = 0.0f;

  m.m[2][0] = 0.0f;
  m.m[2][1] = 0.0f;
  m.m[2][2] = 1.0f;
  m.m[2][3] = 0.0f;

  m.m[3][0] = offsetX;
  m.m[3][1] = offsetY;
  m.m[3][2] = offsetZ;
  m.m[3][3] = 1.0f;
  return m;
#endif
}

Matrix smatrix::translationFromVector(CVector offset) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  Matrix m;
  m.m[0][0] = 1.0f;
  m.m[0][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[0][3] = 0.0f;

  m.m[1][0] = 0.0f;
  m.m[1][1] = 1.0f;
  m.m[1][2] = 0.0f;
  m.m[1][3] = 0.0f;

  m.m[2][0] = 0.0f;
  m.m[2][1] = 0.0f;
  m.m[2][2] = 1.0f;
  m.m[2][3] = 0.0f;

  m.m[3][0] = offset.vector4_f32[0];
  m.m[3][1] = offset.vector4_f32[1];
  m.m[3][2] = offset.vector4_f32[2];
  m.m[3][3] = 1.0f;
  return m;
#endif
}

Matrix smatrix::scaling(core::float32 scaleX, core::float32 scaleY, core::float32 scaleZ) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  Matrix m;
  m.m[0][0] = scaleX;
  m.m[0][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[0][3] = 0.0f;

  m.m[1][0] = 0.0f;
  m.m[1][1] = scaleY;
  m.m[1][2] = 0.0f;
  m.m[1][3] = 0.0f;

  m.m[2][0] = 0.0f;
  m.m[2][1] = 0.0f;
  m.m[2][2] = scaleZ;
  m.m[2][3] = 0.0f;

  m.m[3][0] = 0.f;
  m.m[3][1] = 0.f;
  m.m[3][2] = 0.f;
  m.m[3][3] = 1.0f;
  return m;
#endif
}

Matrix smatrix::scalingFromVector(CVector scale) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  Matrix m;
  m.m[0][0] = scale.vector4_f32[0];
  m.m[0][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[0][3] = 0.0f;

  m.m[1][0] = 0.0f;
  m.m[1][1] = scale.vector4_f32[1];
  m.m[1][2] = 0.0f;
  m.m[1][3] = 0.0f;

  m.m[2][0] = 0.0f;
  m.m[2][1] = 0.0f;
  m.m[2][2] = scale.vector4_f32[2];
  m.m[2][3] = 0.0f;

  m.m[3][0] = 0.f;
  m.m[3][1] = 0.f;
  m.m[3][2] = 0.f;
  m.m[3][3] = 1.0f;
  return m;
#endif
}

Matrix smatrix::rotationX(core::float32 angle) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  core::float32 fSinAngle = scalar::sin(angle);
  core::float32 fCosAngle = scalar::cos(angle);
  
  Matrix m;
  m.m[0][0] = 1.0f;
  m.m[0][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[0][3] = 0.0f;

  m.m[1][0] = 0.0f;
  m.m[1][1] = fCosAngle;
  m.m[1][2] = fSinAngle;
  m.m[1][3] = 0.0f;

  m.m[2][0] = 0.0f;
  m.m[2][1] = -fSinAngle;
  m.m[2][2] = fCosAngle;
  m.m[2][3] = 0.0f;

  m.m[3][0] = 0.0f;
  m.m[3][1] = 0.0f;
  m.m[3][2] = 0.0f;
  m.m[3][3] = 1.0f;
  return m;
#endif
}

Matrix smatrix::rotationY(core::float32 angle) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  core::float32 fSinAngle = scalar::sin(angle);
  core::float32 fCosAngle = scalar::cos(angle);

  Matrix m;
  m.m[0][0] = fCosAngle;
  m.m[0][1] = 0.0f;
  m.m[0][2] = -fSinAngle;
  m.m[0][3] = 0.0f;

  m.m[1][0] = 0.0f;
  m.m[1][1] = 1.0f;
  m.m[1][2] = 0.0f;
  m.m[1][3] = 0.0f;

  m.m[2][0] = fSinAngle;
  m.m[2][1] = 0.f;
  m.m[2][2] = fCosAngle;
  m.m[2][3] = 0.0f;

  m.m[3][0] = 0.0f;
  m.m[3][1] = 0.0f;
  m.m[3][2] = 0.0f;
  m.m[3][3] = 1.0f;
  return m;
#endif
}

Matrix smatrix::rotationZ(core::float32 angle) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  core::float32 fSinAngle = scalar::sin(angle);
  core::float32 fCosAngle = scalar::cos(angle);

  Matrix m;
  m.m[0][0] = fCosAngle;
  m.m[0][1] = fSinAngle;
  m.m[0][2] = 0.0f;
  m.m[0][3] = 0.0f;

  m.m[1][0] = -fSinAngle;
  m.m[1][1] = fCosAngle;
  m.m[1][2] = 0.0f;
  m.m[1][3] = 0.0f;

  m.m[2][0] = 0.0f;
  m.m[2][1] = 0.0f;
  m.m[2][2] = 1.0f;
  m.m[2][3] = 0.0f;

  m.m[3][0] = 0.0f;
  m.m[3][1] = 0.0f;
  m.m[3][2] = 0.0f;
  m.m[3][3] = 1.0f;
  return m;
#endif
}

Matrix smatrix::rotationRollPitchYaw(core::float32 pitch, core::float32 yaw, core::float32 roll) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  core::float32 cp = scalar::cos(pitch);
  core::float32 sp = scalar::sin(pitch);

  core::float32 cy = scalar::cos(yaw);
  core::float32 sy = scalar::sin(yaw);

  core::float32 cr = scalar::cos(roll);
  core::float32 sr = scalar::sin(roll);

  Matrix m;
  m.m[0][0] = cr * cy + sr * sp * sy;
  m.m[0][1] = sr * cp;
  m.m[0][2] = sr * sp * cy - cr * sy;
  m.m[0][3] = 0.0f;

  m.m[1][0] = cr * sp * sy - sr * cy;
  m.m[1][1] = cr * cp;
  m.m[1][2] = sr * sy + cr * sp * cy;
  m.m[1][3] = 0.0f;

  m.m[2][0] = cp * sy;
  m.m[2][1] = -sp;
  m.m[2][2] = cp * cy;
  m.m[2][3] = 0.0f;

  m.m[3][0] = 0.0f;
  m.m[3][1] = 0.0f;
  m.m[3][2] = 0.0f;
  m.m[3][3] = 1.0f;
  return m;
#endif
}

Matrix smatrix::rotationRollPitchYawFromVector(CVector angles) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  core::float32 cp = scalar::cos(angles.vector4_f32[0]);
  core::float32 sp = scalar::sin(angles.vector4_f32[0]);

  core::float32 cy = scalar::cos(angles.vector4_f32[1]);
  core::float32 sy = scalar::sin(angles.vector4_f32[1]);

  core::float32 cr = scalar::cos(angles.vector4_f32[2]);
  core::float32 sr = scalar::sin(angles.vector4_f32[2]);

  Matrix m;
  m.m[0][0] = cr * cy + sr * sp * sy;
  m.m[0][1] = sr * cp;
  m.m[0][2] = sr * sp * cy - cr * sy;
  m.m[0][3] = 0.0f;

  m.m[1][0] = cr * sp * sy - sr * cy;
  m.m[1][1] = cr * cp;
  m.m[1][2] = sr * sy + cr * sp * cy;
  m.m[1][3] = 0.0f;

  m.m[2][0] = cp * sy;
  m.m[2][1] = -sp;
  m.m[2][2] = cp * cy;
  m.m[2][3] = 0.0f;

  m.m[3][0] = 0.0f;
  m.m[3][1] = 0.0f;
  m.m[3][2] = 0.0f;
  m.m[3][3] = 1.0f;
  return m;
#endif
}

Matrix smatrix::rotationNormal(CVector normalAxis, core::float32 angle) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::rotationAxis(CVector axis, core::float32 angle) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::reflect(CVector reflectionPlane) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::lookAtLH(CVector eyePosition, CVector focusPosition, CVector upDirection) noexcept
{
  Vector eyeDirection = svector::subtract(focusPosition, eyePosition);
  return lookToLH(eyePosition, eyeDirection, upDirection);
}

Matrix smatrix::lookAtRH(CVector eyePosition, CVector focusPosition, CVector upDirection) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::lookToLH(CVector eyePosition, CVector eyeDirection, CVector upDirection) noexcept
{
  Vector r2 = svector::normalize3(eyeDirection);

  Vector r0 = svector::cross3(upDirection, r2);
  r0 = svector::normalize3(r0);

  Vector r1 = svector::cross3(r2, r0);

  Vector negEyePosition = svector::negate(eyePosition);

  Vector d0 = svector::dot3(r0, negEyePosition);
  Vector d1 = svector::dot3(r1, negEyePosition);
  Vector d2 = svector::dot3(r2, negEyePosition);

  Matrix m;
  m.r[0] = svector::select(d0, r0, gVectorSelect1110);
  m.r[1] = svector::select(d1, r1, gVectorSelect1110);
  m.r[2] = svector::select(d2, r2, gVectorSelect1110);
  m.r[3] = svector::set(0.f, 0.f, 0.f, 1.0f);

  m = transpose(m);

  return m;
}

Matrix smatrix::lookToRH(CVector eyePosition, CVector eyeDirection, CVector upDirection) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::perspectiveLH(core::float32 viewWidth, core::float32 viewHeight, core::float32 nearZ, core::float32 farZ) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::perspectiveRH(core::float32 viewWidth, core::float32 viewHeight, core::float32 nearZ, core::float32 farZ) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::perspectiveFovLH(core::float32 fovAngleY, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  core::float32 sinFov = scalar::sin(0.5f * fovAngleY);
  core::float32 cosFov = scalar::cos(0.5f * fovAngleY);

  core::float32 height = cosFov / sinFov;
  core::float32 width = height / aspectRatio;
  core::float32 fRange = farZ / (farZ - nearZ);

  Matrix m;
  m.m[0][0] = width;
  m.m[0][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[0][3] = 0.0f;

  m.m[1][0] = 0.0f;
  m.m[1][1] = height;
  m.m[1][2] = 0.0f;
  m.m[1][3] = 0.0f;

  m.m[2][0] = 0.0f;
  m.m[2][1] = 0.0f;
  m.m[2][2] = fRange;
  m.m[2][3] = 1.0f;

  m.m[3][0] = 0.0f;
  m.m[3][1] = 0.0f;
  m.m[3][2] = -fRange * nearZ;
  m.m[3][3] = 0.0f;
  return m;
#endif
}

Matrix smatrix::perspectiveFovRH(core::float32 fovAngleY, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  core::float32 sinFov = scalar::sin(0.5f * fovAngleY);
  core::float32 cosFov = scalar::cos(0.5f * fovAngleY);

  core::float32 height = cosFov / sinFov;
  core::float32 width = height / aspectRatio;
  core::float32 fRange = farZ / (nearZ - farZ);

  Matrix m;
  m.m[0][0] = width;
  m.m[0][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[0][3] = 0.0f;

  m.m[1][0] = 0.0f;
  m.m[1][1] = height;
  m.m[1][2] = 0.0f;
  m.m[1][3] = 0.0f;

  m.m[2][0] = 0.0f;
  m.m[2][1] = 0.0f;
  m.m[2][2] = fRange;
  m.m[2][3] = -1.0f;

  m.m[3][0] = 0.0f;
  m.m[3][1] = 0.0f;
  m.m[3][2] = fRange * nearZ;
  m.m[3][3] = 0.0f;
  return m;
#endif
}

Matrix smatrix::perspectiveOffCenterLH(core::float32 viewLeft, core::float32 viewRight, core::float32 viewBottom, core::float32 viewTop, core::float32 nearZ, core::float32 farZ) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::perspectiveOffCenterRH(core::float32 viewLeft, core::float32 viewRight, core::float32 viewBottom, core::float32 viewTop, core::float32 nearZ, core::float32 farZ) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::orthographicLH(core::float32 viewWidth, core::float32 viewHeight, core::float32 nearZ, core::float32 farZ) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::orthographicRH(core::float32 viewWidth, core::float32 viewHeight, core::float32 nearZ, core::float32 farZ) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::orthographicOffCenterLH(core::float32 viewLeft, core::float32 viewRight, core::float32 viewBottom, core::float32 viewTop, core::float32 nearZ, core::float32 farZ) noexcept
{
  core::assert(false && "!TODO");
  return Matrix();
}

Matrix smatrix::orthographicOffCenterRH(core::float32 viewLeft, core::float32 viewRight, core::float32 viewBottom, core::float32 viewTop, core::float32 nearZ, core::float32 farZ) noexcept
{
  return Matrix();
}

Vector svector::transformCoord3(CVector v, CMatrix m) noexcept
{
  Vector z = svector::splatZ(v);
  Vector y = svector::splatY(v);
  Vector x = svector::splatX(v);

  Vector result = svector::multiplyAdd(z, m.r[2], m.r[3]);
  result = svector::multiplyAdd(y, m.r[1], result);
  result = svector::multiplyAdd(x, m.r[0], result);

  Vector w = svector::splatW(result);
  return svector::divide(result, w);
}

}