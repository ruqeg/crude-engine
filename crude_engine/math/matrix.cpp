#include <math/matrix.hpp>
#include <core/assert.hpp>

namespace crude_engine
{

constexpr Matrix::Matrix(const Vector& r0, const Vector& r1, const Vector& r2, const Vector& r3) noexcept
  :
  r{r0, r1, r2, r3}
{}

Matrix::Matrix(float32 m00, float32 m01, float32 m02, float32 m03,
               float32 m10, float32 m11, float32 m12, float32 m13,
               float32 m20, float32 m21, float32 m22, float32 m23,
               float32 m30, float32 m31, float32 m32, float32 m33) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
  m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
  m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
  m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
#endif
}

bool SysMatrix::isIdentity(CMatrix m) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  return (m.m[0][0] == 1) && (m.m[1][1] == 1) && (m.m[2][2] == 1) && (m.m[3][3] == 1);
#endif
}

Matrix SysMatrix::multiply(CMatrix m1, CMatrix m2) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  Matrix mResult;

  float32 x = m1.m[0][0];
  float32 y = m1.m[0][1];
  float32 z = m1.m[0][2];
  float32 w = m1.m[0][3];

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

Matrix SysMatrix::multiplyTranspose(CMatrix m1, CMatrix m2) noexcept
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

Matrix SysMatrix::transpose(CMatrix m) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::inverse(Vector* pDeterminant, CMatrix m) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Vector SysMatrix::determinant(CMatrix m) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Vector();
}

Matrix SysMatrix::identity() noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  Matrix m;
  m.r[0] = SysVector::set(1.f, 0.f, 0.f, 0.f);
  m.r[1] = SysVector::set(0.f, 1.f, 0.f, 0.f);
  m.r[2] = SysVector::set(0.f, 0.f, 1.f, 0.f);
  m.r[3] = SysVector::set(0.f, 0.f, 0.f, 1.f);
  return m;
#endif
}

Matrix SysMatrix::set(float32 m00, float32 m01, float32 m02, float32 m03, float32 m10, float32 m11, float32 m12, float32 m13, float32 m20, float32 m21, float32 m22, float32 m23, float32 m30, float32 m31, float32 m32, float32 m33) noexcept
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

Matrix SysMatrix::translation(float32 offsetX, float32 offsetY, float32 offsetZ) noexcept
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

Matrix SysMatrix::translationFromVector(CVector offset) noexcept
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

Matrix SysMatrix::scaling(float32 scaleX, float32 scaleY, float32 scaleZ) noexcept
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

Matrix SysMatrix::scalingFromVector(CVector scale) noexcept
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

Matrix SysMatrix::rotationX(float32 angle) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  float32 fSinAngle = Scalar::sin(angle);
  float32 fCosAngle = Scalar::cos(angle);
  
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

Matrix SysMatrix::rotationY(float32 angle) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  float32 fSinAngle = Scalar::sin(angle);
  float32 fCosAngle = Scalar::cos(angle);

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

Matrix SysMatrix::rotationZ(float32 angle) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  float32 fSinAngle = Scalar::sin(angle);
  float32 fCosAngle = Scalar::cos(angle);

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

Matrix SysMatrix::rotationRollPitchYaw(float32 pitch, float32 yaw, float32 roll) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  float32 cp = Scalar::cos(pitch);
  float32 sp = Scalar::sin(pitch);

  float32 cy = Scalar::cos(yaw);
  float32 sy = Scalar::sin(yaw);

  float32 cr = Scalar::cos(roll);
  float32 sr = Scalar::sin(roll);

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

Matrix SysMatrix::rotationRollPitchYawFromVector(CVector angles) noexcept
{
#if defined(_CRUDE_NO_INTRINSICS)
  float32 cp = Scalar::cos(angles.vector4_f32[0]);
  float32 sp = Scalar::sin(angles.vector4_f32[0]);

  float32 cy = Scalar::cos(angles.vector4_f32[1]);
  float32 sy = Scalar::sin(angles.vector4_f32[1]);

  float32 cr = Scalar::cos(angles.vector4_f32[2]);
  float32 sr = Scalar::sin(angles.vector4_f32[2]);

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

Matrix SysMatrix::rotationNormal(CVector normalAxis, float32 angle) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::rotationAxis(CVector axis, float32 angle) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::reflect(CVector reflectionPlane) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::lookAtLH(CVector eyePosition, CVector focusPosition, CVector upDirection) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::lookAtRH(CVector eyePosition, CVector focusPosition, CVector upDirection) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::lookToLH(CVector eyePosition, CVector eyeDirection, CVector upDirection) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::lookToRH(CVector eyePosition, CVector eyeDirection, CVector upDirection) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::perspectiveLH(float32 viewWidth, float32 viewHeight, float32 nearZ, float32 farZ) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::perspectiveRH(float32 viewWidth, float32 viewHeight, float32 nearZ, float32 farZ) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::perspectiveFovLH(float32 fovAngleY, float32 aspectRatio, float32 nearZ, float32 farZ) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::perspectiveFovRH(float32 fovAngleY, float32 aspectRatio, float32 nearZ, float32 farZ) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::perspectiveOffCenterLH(float32 viewLeft, float32 viewRight, float32 viewBottom, float32 viewTop, float32 nearZ, float32 farZ) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::perspectiveOffCenterRH(float32 viewLeft, float32 viewRight, float32 viewBottom, float32 viewTop, float32 nearZ, float32 farZ) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::orthographicLH(float32 viewWidth, float32 viewHeight, float32 nearZ, float32 farZ) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::orthographicRH(float32 viewWidth, float32 viewHeight, float32 nearZ, float32 farZ) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::orthographicOffCenterLH(float32 viewLeft, float32 viewRight, float32 viewBottom, float32 viewTop, float32 nearZ, float32 farZ) noexcept
{
  CRUDE_ASSERT(false && "!TODO");
  return Matrix();
}

Matrix SysMatrix::orthographicOffCenterRH(float32 viewLeft, float32 viewRight, float32 viewBottom, float32 viewTop, float32 nearZ, float32 farZ) noexcept
{
  return Matrix();
}

}