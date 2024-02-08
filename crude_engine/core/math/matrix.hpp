#pragma once

#include <core/math/vector.hpp>

namespace crude_engine
{

#ifdef _CRUDE_NO_INTRINSICS
struct Matrix
#else
//! TODO
#endif
{
#ifdef _CRUDE_NO_INTRINSICS
  union
  {
    Vector r[4];
    struct
    {
      float _11, _12, _13, _14;
      float _21, _22, _23, _24;
      float _31, _32, _33, _34;
      float _41, _42, _43, _44;
    };
    float m[4][4];
  };
#else
// !TODO
#endif

  Matrix() = default;

  Matrix(const Matrix& other) noexcept = default;
  Matrix& operator= (const Matrix& other) noexcept = default;

  Matrix(Matrix&& other) noexcept = default;
  Matrix& operator=(Matrix&& other) noexcept = default;

  constexpr Matrix(const Vector& r0, const Vector& r1, const Vector& r2, const Vector& r3) noexcept;

  Matrix(float32 m00, float32 m01, float32 m02, float32 m03,
         float32 m10, float32 m11, float32 m12, float32 m13,
         float32 m20, float32 m21, float32 m22, float32 m23,
         float32 m30, float32 m31, float32 m32, float32 m33) noexcept;
};

class SysMatrix
{
public:
  using CMatrix = const Matrix&;
  using CVector = const Vector&;

public:
  bool isIdentity(CMatrix m) noexcept;

  Matrix multiply(CMatrix m1, CMatrix m2) noexcept;
  Matrix multiplyTranspose(CMatrix m1, CMatrix m2) noexcept;
  Matrix transpose(CMatrix m) noexcept;
  Matrix inverse(Vector* pDeterminant, CMatrix m) noexcept;
  Vector determinant(CMatrix m) noexcept;

  Matrix identity() noexcept;
  Matrix set(float32 m00, float32 m01, float32 m02, float32 m03,
             float32 m10, float32 m11, float32 m12, float32 m13,
             float32 m20, float32 m21, float32 m22, float32 m23,
             float32 m30, float32 m31, float32 m32, float32 m33) noexcept;
  Matrix translation(float32 offsetX, float32 offsetY, float32 offsetZ) noexcept;
  Matrix translationFromVector(CVector offset) noexcept;
  Matrix scaling(float32 scaleX, float32 scaleY, float32 scaleZ) noexcept;
  Matrix scalingFromVector(CVector scale) noexcept;
  Matrix rotationX(float32 angle) noexcept;
  Matrix rotationY(float32 angle) noexcept;
  Matrix rotationZ(float32 angle) noexcept;

  Matrix rotationRollPitchYaw(float32 pitch, float32 yaw, float32 roll) noexcept;

  Matrix rotationRollPitchYawFromVector(CVector angles) noexcept;

  Matrix rotationNormal(CVector normalAxis, float32 angle) noexcept;
  Matrix rotationAxis(CVector axis, float32 angle) noexcept;
  Matrix reflect(CVector reflectionPlane) noexcept;

  Matrix lookAtLH(CVector eyePosition, CVector focusPosition, CVector upDirection) noexcept;
  Matrix lookAtRH(CVector eyePosition, CVector focusPosition, CVector upDirection) noexcept;
  Matrix lookToLH(CVector eyePosition, CVector eyeDirection, CVector upDirection) noexcept;
  Matrix lookToRH(CVector eyePosition, CVector eyeDirection, CVector upDirection) noexcept;
  Matrix perspectiveLH(float32 viewWidth, float32 viewHeight, float32 nearZ, float32 farZ) noexcept;
  Matrix perspectiveRH(float32 viewWidth, float32 viewHeight, float32 nearZ, float32 farZ) noexcept;
  Matrix perspectiveFovLH(float32 fovAngleY, float32 aspectRatio, float32 nearZ, float32 farZ) noexcept;
  Matrix perspectiveFovRH(float32 fovAngleY, float32 aspectRatio, float32 nearZ, float32 farZ) noexcept;
  Matrix perspectiveOffCenterLH(float32 viewLeft, float32 viewRight, float32 viewBottom, float32 viewTop, float32 nearZ, float32 farZ) noexcept;
  Matrix perspectiveOffCenterRH(float32 viewLeft, float32 viewRight, float32 viewBottom, float32 viewTop, float32 nearZ, float32 farZ) noexcept;
  Matrix orthographicLH(float32 viewWidth, float32 viewHeight, float32 nearZ, float32 farZ) noexcept;
  Matrix orthographicRH(float32 viewWidth, float32 viewHeight, float32 nearZ, float32 farZ) noexcept;
  Matrix orthographicOffCenterLH(float32 viewLeft, float32 viewRight, float32 viewBottom, float32 viewTop, float32 nearZ, float32 farZ) noexcept;
  Matrix orthographicOffCenterRH(float32 viewLeft, float32 viewRight, float32 viewBottom, float32 viewTop, float32 nearZ, float32 farZ) noexcept;

};

}