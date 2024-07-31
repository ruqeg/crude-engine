#include <directxmath/DirectXMath.h>
#include <flecs.h>

module crude.scene.transform;

namespace crude::scene
{

Transform::Transform(flecs::entity_view node)
  : m_node(node) {}

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

void Transform::addTranslation(const DirectX::XMFLOAT3& translatio)
{
  invalidateNodeToWorld();
  m_translationFloat3.x += translatio.x;
  m_translationFloat3.y += translatio.y;
  m_translationFloat3.z += translatio.z;
}

void Transform::addTranslation(DirectX::FXMVECTOR translation)
{
  invalidateNodeToWorld();
  DirectX::XMStoreFloat3(&m_translationFloat3, DirectX::XMVectorAdd(getTranslationVector(), translation));
}

void Transform::addTranslation(core::float32 x, core::float32 y, core::float32 z)
{
  invalidateNodeToWorld();
  m_translationFloat3.x += x;
  m_translationFloat3.y += y;
  m_translationFloat3.z += z;
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

void Transform::addScale(const DirectX::XMFLOAT3& scale)
{
  invalidateNodeToWorld();
  m_scaleFloat3.x += scale.x;
  m_scaleFloat3.y += scale.y;
  m_scaleFloat3.z += scale.z;
}

void Transform::addScale(DirectX::FXMVECTOR scale)
{
  invalidateNodeToWorld();
  DirectX::XMStoreFloat3(&m_scaleFloat3, DirectX::XMVectorAdd(getScaleVector(), scale));
}

void Transform::addScale(core::float32 x, core::float32 y, core::float32 z)
{
  invalidateNodeToWorld();
  m_scaleFloat3.x += x;
  m_scaleFloat3.y += y;
  m_scaleFloat3.z += z;
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

void Transform::addRotation(const DirectX::XMFLOAT3& rotation)
{
  invalidateNodeToWorld();
  DirectX::XMVECTOR newRotation = DirectX::XMQuaternionMultiply(getRotationQuaternion(), DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));
  DirectX::XMStoreFloat4(&m_rotationFloat4, newRotation);
}

void Transform::addRotation(DirectX::FXMVECTOR rotation)
{
  invalidateNodeToWorld();
  DirectX::XMVECTOR newRotation = DirectX::XMQuaternionMultiply(getRotationQuaternion(), DirectX::XMQuaternionRotationRollPitchYawFromVector(rotation));
  DirectX::XMStoreFloat4(&m_rotationFloat4, newRotation);
}

void Transform::addRotation(core::float32 x, core::float32 y, core::float32 z)
{
  invalidateNodeToWorld();
  DirectX::XMVECTOR newRotation = DirectX::XMQuaternionMultiply(getRotationQuaternion(), DirectX::XMQuaternionRotationRollPitchYaw(x, y, z));
  DirectX::XMStoreFloat4(&m_rotationFloat4, newRotation);
}

void Transform::addRotationAxis(DirectX::FXMVECTOR vector, core::float32 rotation)
{
  invalidateNodeToWorld();
  DirectX::XMVECTOR newRotation = DirectX::XMQuaternionMultiply(getRotationQuaternion(), DirectX::XMQuaternionRotationAxis(vector, rotation));
  DirectX::XMStoreFloat4(&m_rotationFloat4, newRotation);
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
  return DirectX::XMMatrixAffineTransformation(getScaleVector(), DirectX::XMVectorZero(), getRotationQuaternion(), getTranslationVector());
}

DirectX::XMMATRIX Transform::getNodeToWorldMatrix()
{
  return DirectX::XMLoadFloat4x4(&getNodeToWorldFloat4x4());
}

DirectX::XMMATRIX Transform::getWorldToNodeMatrix()
{
  return DirectX::XMLoadFloat4x4(&getWorldToNodeFloat4x4());
}

const DirectX::XMFLOAT4X4& Transform::getNodeToWorldFloat4x4()
{
  updateNodeToWorld();
  return m_nodeToWorldFloat4x4;
}

const DirectX::XMFLOAT4X4& Transform::getWorldToNodeFloat4x4()
{
  updateNodeToWorld();
  return m_worldToNodeFloat4x4;
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

  flecs::entity parent = m_node.parent();
  if (parent.is_valid() && parent.has<scene::Transform>())
  {
    Transform* transform = parent.get_mut<Transform>();
    const DirectX::XMMATRIX nodeToWorld = DirectX::XMMatrixMultiply(transform->getNodeToWorldMatrix(), getNodeToParentMatrix());
    DirectX::XMStoreFloat4x4(&m_nodeToWorldFloat4x4, nodeToWorld);
  }
  else
  {
    const DirectX::XMMATRIX nodeToWorld = getNodeToParentMatrix();
    DirectX::XMStoreFloat4x4(&m_nodeToWorldFloat4x4, nodeToWorld);
  }

  DirectX::XMStoreFloat4x4(&m_worldToNodeFloat4x4, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&m_nodeToWorldFloat4x4)));

  m_updateNodeToWorld = false;
}

}