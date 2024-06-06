module;

export module crude.math.matrix;

export import crude.math.vector;

export namespace crude::math
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

  Matrix(core::float32 m00, core::float32 m01, core::float32 m02, core::float32 m03,
         core::float32 m10, core::float32 m11, core::float32 m12, core::float32 m13,
         core::float32 m20, core::float32 m21, core::float32 m22, core::float32 m23,
         core::float32 m30, core::float32 m31, core::float32 m32, core::float32 m33) noexcept;
};

using CMatrix = const Matrix&;
using CVector = const Vector&;

namespace smatrix
{

bool isIdentity(CMatrix m) noexcept;

Matrix multiply(CMatrix m1, CMatrix m2) noexcept;
Matrix multiplyTranspose(CMatrix m1, CMatrix m2) noexcept;
Matrix transpose(CMatrix m) noexcept;
Matrix inverse(CMatrix m) noexcept;
Vector determinant(CMatrix m) noexcept;

Matrix identity() noexcept;
Matrix set(core::float32 m00, core::float32 m01, core::float32 m02, core::float32 m03,
           core::float32 m10, core::float32 m11, core::float32 m12, core::float32 m13,
           core::float32 m20, core::float32 m21, core::float32 m22, core::float32 m23,
           core::float32 m30, core::float32 m31, core::float32 m32, core::float32 m33) noexcept;
Matrix translation(core::float32 offsetX, core::float32 offsetY, core::float32 offsetZ) noexcept;
Matrix translationFromVector(CVector offset) noexcept;
Matrix scaling(core::float32 scaleX, core::float32 scaleY, core::float32 scaleZ) noexcept;
Matrix scalingFromVector(CVector scale) noexcept;
Matrix rotationX(core::float32 angle) noexcept;
Matrix rotationY(core::float32 angle) noexcept;
Matrix rotationZ(core::float32 angle) noexcept;

Matrix rotationRollPitchYaw(core::float32 pitch, core::float32 yaw, core::float32 roll) noexcept;

Matrix rotationRollPitchYawFromVector(CVector angles) noexcept;

Matrix rotationNormal(CVector normalAxis, core::float32 angle) noexcept;
Matrix rotationAxis(CVector axis, core::float32 angle) noexcept;
Matrix reflect(CVector reflectionPlane) noexcept;

Matrix lookAtLH(CVector eyePosition, CVector focusPosition, CVector upDirection) noexcept;
Matrix lookAtRH(CVector eyePosition, CVector focusPosition, CVector upDirection) noexcept;
Matrix lookToLH(CVector eyePosition, CVector eyeDirection, CVector upDirection) noexcept;
Matrix lookToRH(CVector eyePosition, CVector eyeDirection, CVector upDirection) noexcept;
Matrix perspectiveLH(core::float32 viewWidth, core::float32 viewHeight, core::float32 nearZ, core::float32 farZ) noexcept;
Matrix perspectiveRH(core::float32 viewWidth, core::float32 viewHeight, core::float32 nearZ, core::float32 farZ) noexcept;
Matrix perspectiveFovLH(core::float32 fovAngleY, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ) noexcept;
Matrix perspectiveFovRH(core::float32 fovAngleY, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ) noexcept;
Matrix perspectiveOffCenterLH(core::float32 viewLeft, core::float32 viewRight, core::float32 viewBottom, core::float32 viewTop, core::float32 nearZ, core::float32 farZ) noexcept;
Matrix perspectiveOffCenterRH(core::float32 viewLeft, core::float32 viewRight, core::float32 viewBottom, core::float32 viewTop, core::float32 nearZ, core::float32 farZ) noexcept;
Matrix orthographicLH(core::float32 viewWidth, core::float32 viewHeight, core::float32 nearZ, core::float32 farZ) noexcept;
Matrix orthographicRH(core::float32 viewWidth, core::float32 viewHeight, core::float32 nearZ, core::float32 farZ) noexcept;
Matrix orthographicOffCenterLH(core::float32 viewLeft, core::float32 viewRight, core::float32 viewBottom, core::float32 viewTop, core::float32 nearZ, core::float32 farZ) noexcept;
Matrix orthographicOffCenterRH(core::float32 viewLeft, core::float32 viewRight, core::float32 viewBottom, core::float32 viewTop, core::float32 nearZ, core::float32 farZ) noexcept;

}

// !TODO it looks weird
namespace svector
{

Vector transformCoord3(CVector v, CMatrix m) noexcept;

}

}