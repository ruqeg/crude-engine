#include <DirectXMath.h>

module crude.scene.camera;

import crude.scene.transform;

namespace crude::scene
{

void Camera::calculateViewToClipMatrix(core::float32 fovRadians, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ)
{
  m_aspectRatio = m_aspectRatio;
  m_farZ = farZ;
  m_nearZ = nearZ;
  m_fovRadians = fovRadians;

  DirectX::XMMATRIX viewToClipMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
  DirectX::XMMATRIX clipToViewMatrix = DirectX::XMMatrixInverse(nullptr, viewToClipMatrix);
  DirectX::XMStoreFloat4x4(&m_viewToClipFloat4x4, viewToClipMatrix);
  DirectX::XMStoreFloat4x4(&m_clipToViewFloat4x4, clipToViewMatrix);
}

}