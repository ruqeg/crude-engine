#include <directxmath/DirectXMath.h>
#include <flecs.h>

module crude.scene.transform;

import crude.scene.node;

namespace crude::scene
{

Transform::Transform(core::shared_ptr<Node> node)
  : m_node(node)
{}

void Transform::setTranslation(const DirectX::XMFLOAT3& translationFloat3)
{
  invalidateNodeToWorld();
  m_translationFloat3 = translationFloat3;
}

void Transform::setTranslation(DirectX::FXMVECTOR translation)
{
  invalidateNodeToWorld();
  DirectX::XMStoreFloat3(&m_translationFloat3, translation);
}

void Transform::setTranslation(core::float32 x, core::float32 y, core::float32 z)
{
  invalidateNodeToWorld();
  m_translationFloat3.x = x;
  m_translationFloat3.y = y;
  m_translationFloat3.z = z;
}

void Transform::setScale(const DirectX::XMFLOAT3& scaleFloat3)
{
  invalidateNodeToWorld();
  m_scaleFloat3 = scaleFloat3;
}

void Transform::setScale(DirectX::FXMVECTOR scale)
{
  invalidateNodeToWorld();
  DirectX::XMStoreFloat3(&m_scaleFloat3, scale);
}

void Transform::setScale(core::float32 x, core::float32 y, core::float32 z)
{
  invalidateNodeToWorld();
  DirectX::XMVECTOR scale = getScaleVector();
  m_scaleFloat3.x = x;
  m_scaleFloat3.y = y;
  m_scaleFloat3.z = z;
}

void Transform::setRotation(const DirectX::XMFLOAT3& rotationFloat3)
{
  invalidateNodeToWorld();
  DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationRollPitchYaw(rotationFloat3.x, rotationFloat3.y, rotationFloat3.z);
  DirectX::XMStoreFloat4(&m_rotationFloat4, rotation);
}

void Transform::setRotation(DirectX::FXMVECTOR rotation)
{
  invalidateNodeToWorld();
  DirectX::XMStoreFloat4(&m_rotationFloat4, DirectX::XMQuaternionRotationRollPitchYawFromVector(rotation));
}

void Transform::setRotation(core::float32 x, core::float32 y, core::float32 z)
{
  invalidateNodeToWorld();
  DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);
  DirectX::XMStoreFloat4(&m_rotationFloat4, rotation);
}

void Transform::setNodeToParent(const DirectX::XMFLOAT4X4& nodeToParent)
{
  invalidateNodeToWorld();
  decomposeNodeToParent(DirectX::XMLoadFloat4x4(&nodeToParent));
}

void Transform::setNodeToParent(DirectX::FXMMATRIX nodeToParent)
{
  invalidateNodeToWorld();
  decomposeNodeToParent(nodeToParent);
}

DirectX::XMMATRIX Transform::getNodeToParentMatrix() const
{
  return DirectX::XMMatrixAffineTransformation(getScaleVector(), DirectX::XMVectorZero(), getRotationVector(), getTranslationVector());
}

DirectX::XMMATRIX Transform::getNodeToWorldMatrix()
{
  return DirectX::XMLoadFloat4x4(&getNodeToWorldFloat4x4());
}

const DirectX::XMFLOAT4X4& Transform::getNodeToWorldFloat4x4()
{
  updateNodeToWorld();
  return m_nodeToWorldFloat4x4;
}

core::shared_ptr<Node> Transform::getNode()
{
  return m_node;
}

core::shared_ptr<const Node> Transform::getNode() const
{
  return m_node;
}

void Transform::decomposeNodeToParent(DirectX::FXMMATRIX nodeToParent)
{
  DirectX::XMVECTOR translation, rotation, scale;
  DirectX::XMMatrixDecompose(&scale, &rotation, &translation, nodeToParent);
  DirectX::XMStoreFloat3(&m_scaleFloat3, scale);
  DirectX::XMStoreFloat4(&m_rotationFloat4, rotation);
  DirectX::XMStoreFloat3(&m_translationFloat3, translation);
}

void Transform::updateNodeToWorld()
{
  if (!m_updateNodeToWorld)
    return;

  core::shared_ptr<Node> parent = m_node->getParent();
  if (parent)
  {
    auto transform = parent->getEntity().get_ref<Transform>();
    const DirectX::XMMATRIX nodeToWorld = DirectX::XMMatrixMultiply(transform->getNodeToWorldMatrix(), getNodeToParentMatrix());
    DirectX::XMStoreFloat4x4(&m_nodeToWorldFloat4x4, nodeToWorld);
  }
  else
  {
    const DirectX::XMMATRIX nodeToWorld = getNodeToParentMatrix();
    DirectX::XMStoreFloat4x4(&m_nodeToWorldFloat4x4, nodeToWorld);
  }

  m_updateNodeToWorld = false;
}

}