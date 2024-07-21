#include <directxmath/DirectXMath.h>

module crude.scene.camera;

namespace crude::scene
{

Camera::Camera()
{
  m_positionFloat3 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
  m_rotationFloat3 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
  calculateWorldToViewMatrix();
}

void Camera::calculateViewToClipMatrix(core::float32 fovRadians, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ)
{
  m_farZ       = farZ;
  m_nearZ      = nearZ;
  m_fovRadians = fovRadians;

  DirectX::XMMATRIX viewToClipMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
  DirectX::XMMATRIX clipToViewMatrix = DirectX::XMMatrixInverse(nullptr, viewToClipMatrix);
  DirectX::XMStoreFloat4x4(&m_viewToClipFloat4x4, viewToClipMatrix);
  DirectX::XMStoreFloat4x4(&m_clipToViewFloat4x4, clipToViewMatrix);
}

void Camera::calculateWorldToViewMatrix()
{
  DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&m_positionFloat3);
  DirectX::XMVECTOR rotation = DirectX::XMLoadFloat3(&m_rotationFloat3);
  DirectX::XMMATRIX cameraRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
  DirectX::XMVECTOR cameraDirection = DirectX::XMVector3TransformCoord(cDefaultCameraForward, cameraRotation);
  DirectX::XMVECTOR cameraTarget = DirectX::XMVectorAdd(position, cameraDirection);
  DirectX::XMVECTOR upDir = DirectX::XMVector3TransformCoord(cDefaultCameraTop, cameraRotation);
  DirectX::XMMATRIX worldToView = DirectX::XMMatrixLookAtLH(position, cameraTarget, upDir);
  DirectX::XMMATRIX viewToWorld = DirectX::XMMatrixInverse(nullptr, worldToView);
  DirectX::XMStoreFloat4x4(&m_worldToViewFloat4x4, worldToView);
  DirectX::XMStoreFloat4x4(&m_viewToWorldFloat4x4, viewToWorld);

  DirectX::XMStoreFloat3(&m_forwardFloat3, DirectX::XMVector3TransformCoord(cDefaultCameraForward, cameraRotation));
  DirectX::XMStoreFloat3(&m_topFloat3, DirectX::XMVector3TransformCoord(cDefaultCameraTop, cameraRotation));
  DirectX::XMStoreFloat3(&m_rightFloat3, DirectX::XMVector3TransformCoord(cDefaultCameraRight, cameraRotation));

  DirectX::XMMATRIX worldToClip = DirectX::XMMatrixMultiply(worldToView, DirectX::XMLoadFloat4x4(&m_viewToClipFloat4x4));
  DirectX::XMMATRIX clipToWorld = DirectX::XMMatrixInverse(nullptr, worldToClip);
  DirectX::XMStoreFloat4x4(&m_worldToClipFloat4x4, worldToClip);
  DirectX::XMStoreFloat4x4(&m_clipToWorldFloat4x4, clipToWorld);
}

void Camera::setPosition(DirectX::XMFLOAT3 newPosition)
{
  m_positionFloat3 = newPosition;
}

void Camera::setPosition(DirectX::FXMVECTOR newPosition)
{
  DirectX::XMStoreFloat3(&m_positionFloat3, newPosition);
}

void Camera::setPosition(core::float32 x, core::float32 y, core::float32 z)
{
  m_positionFloat3 = DirectX::XMFLOAT3(x, y, z);
}

void Camera::addPosition(DirectX::FXMVECTOR addedPosition)
{
  DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&m_positionFloat3);
  DirectX::XMStoreFloat3(&m_positionFloat3, DirectX::XMVectorAdd(position, addedPosition));
}

void Camera::addPosition(core::float32 x, core::float32 y, core::float32 z)
{
  m_positionFloat3.x += x;
  m_positionFloat3.y += y;
  m_positionFloat3.z += z;
}

void Camera::setRotation(DirectX::XMFLOAT3 newRotation)
{
  m_rotationFloat3 = newRotation;
}

void Camera::setRotation(DirectX::FXMVECTOR newRotation)
{
  DirectX::XMStoreFloat3(&m_rotationFloat3, newRotation);
}

void Camera::setRotation(core::float32 x, core::float32 y, core::float32 z)
{
  m_rotationFloat3 = DirectX::XMFLOAT3(x, y, z);
}

void Camera::addRotation(DirectX::FXMVECTOR addedRotation)
{
  DirectX::XMVECTOR rotation = DirectX::XMLoadFloat3(&m_rotationFloat3);
  DirectX::XMStoreFloat3(&m_rotationFloat3, DirectX::XMVectorAdd(rotation, addedRotation));
}

void Camera::addRotation(core::float32 x, core::float32 y, core::float32 z)
{
  m_rotationFloat3.x += x;
  m_rotationFloat3.y += y;
  m_rotationFloat3.z += z;
}

}