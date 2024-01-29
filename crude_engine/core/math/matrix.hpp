#pragma once

#include <core/math/vector.hpp>

namespace crude_engine
{

#ifdef _CRUDE_NO_INTRINSICS_
struct Matrix
#else
//! TODO
#endif
{
#ifdef _CRUDE_NO_INTRINSICS_
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

  Matrix(const Matrix& other) noexcept;
  Matrix& operator= (const Matrix& other) noexcept;

  Matrix(Matrix&& other) noexcept;
  Matrix& operator=(Matrix&& other) noexcept;

  constexpr Matrix(const Vector& c0, const Vector& c1, const Vector& c2, const Vector& c3) noexcept;

  Matrix(float32 m00, float32 m01, float32 m02, float32 m03,
         float32 m10, float32 m11, float32 m12, float32 m13,
         float32 m20, float32 m21, float32 m22, float32 m23,
         float32 m30, float32 m31, float32 m32, float32 m33) noexcept;
};

}