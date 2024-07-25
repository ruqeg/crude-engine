#include <directxmath/DirectXMath.h>
#include <flecs.h>

module crude.scene.transform;

import crude.scene.node;

namespace crude::scene
{

Transform::Transform(core::shared_ptr<Node2> node)
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
  m_scaleFloat3.x = x;
  m_scaleFloat3.y = y;
  m_scaleFloat3.z = z;
}

void Transform::setRotation(const DirectX::XMFLOAT3& rotationFloat3)
{
  invalidateNodeToWorld();
  m_rotationFloat3 = rotationFloat3;
}

void Transform::setRotation(DirectX::FXMVECTOR rotation)
{
  invalidateNodeToWorld();
  DirectX::XMStoreFloat3(&m_rotationFloat3, rotation);
}

void Transform::setRotation(core::float32 x, core::float32 y, core::float32 z)
{
  invalidateNodeToWorld();
  m_rotationFloat3.x = x;
  m_rotationFloat3.y = y;
  m_rotationFloat3.z = z;
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
  return DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(
    DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_translationFloat3)),
    DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat3(&m_rotationFloat3))),
    DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_scaleFloat3)));
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

core::shared_ptr<Node2> Transform::getNode()
{
  return m_node;
}

core::shared_ptr<const Node2> Transform::getNode() const
{
  return m_node;
}

void Transform::decomposeNodeToParent(DirectX::FXMMATRIX nodeToParent)
{
  DirectX::XMVECTOR translation, rotation, scale;
  DirectX::XMMatrixDecompose(&scale, &rotation, &translation, nodeToParent);
  DirectX::XMStoreFloat3(&m_scaleFloat3, scale);
  DirectX::XMStoreFloat3(&m_rotationFloat3, rotation);
  DirectX::XMStoreFloat3(&m_translationFloat3, translation);
}

void Transform::updateNodeToWorld()
{
  if (!m_updateNodeToWorld)
    return;

  core::shared_ptr<Node2> parent = m_node->getParent();
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