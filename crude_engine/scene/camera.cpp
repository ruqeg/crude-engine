#include <directxmath/DirectXMath.h>
#include <flecs.h>

module crude.scene.camera;
import crude.scene.transform;
import crude.scene.node;

namespace crude::scene
{

Camera::Camera(core::shared_ptr<Node> node) : m_node(node) {}

core::shared_ptr<const Node> Camera::getNode() const { return m_node; }
core::shared_ptr<Node> Camera::getNode() { return m_node; }

void Camera::calculateViewToClipMatrix(core::float32 fovRadians, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ)
{
  m_farZ = farZ;
  m_nearZ = nearZ;
  m_fovRadians = fovRadians;

  DirectX::XMMATRIX viewToClipMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
  DirectX::XMMATRIX clipToViewMatrix = DirectX::XMMatrixInverse(nullptr, viewToClipMatrix);
  DirectX::XMStoreFloat4x4(&m_viewToClipFloat4x4, viewToClipMatrix);
  DirectX::XMStoreFloat4x4(&m_clipToViewFloat4x4, clipToViewMatrix);
}

}