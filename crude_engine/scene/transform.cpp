#include <DirectXMath.h>
#include <flecs.h>

module crude.scene.transform;

namespace crude::scene
{

const DirectX::XMVECTOR cDefaultBasisRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR cDefaultBasisUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR cDefaultBasisForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

Transform::Transform(flecs::entity_view node)
  : m_node(node) 
{
  DirectX::XMStoreFloat4x4(&m_nodeToWorldFloat4x4, DirectX::XMMatrixIdentity());
  DirectX::XMStoreFloat4x4(&m_worldToNodeFloat4x4, DirectX::XMMatrixIdentity());
  DirectX::XMStoreFloat3(&m_translationFloat3, DirectX::XMVectorZero());
  DirectX::XMStoreFloat4(&m_rotationFloat4, DirectX::XMQuaternionIdentity());
  DirectX::XMStoreFloat3(&m_scaleFloat3, DirectX::XMVectorSplatOne());
}

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

void Transform::setRotationQuaternion(DirectX::FXMVECTOR quaternion)
{
  invalidateNodeToWorld();
  DirectX::XMStoreFloat4(&m_rotationFloat4, quaternion);
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

DirectX::XMMATRIX Transform::getParentToWorldMatrix()
{
  flecs::entity parent = m_node.parent();
  
  if (!parent.is_valid() || !parent.has<scene::Transform>())
  {
    return DirectX::XMMatrixIdentity();
  }

  Transform* transform = parent.get_mut<Transform>();
  return transform->getNodeToWorldMatrix();
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

DirectX::XMVECTOR Transform::getWorldTranslationVector()
{
  // !TODO
  return DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&m_translationFloat3), getParentToWorldMatrix());
}

DirectX::XMFLOAT4X4 Transform::getNodeToParentFloat4x4() const
{
  DirectX::XMFLOAT4X4 nodeToParent;
  DirectX::XMStoreFloat4x4(&nodeToParent, getNodeToParentMatrix());
  return nodeToParent;
}

// !TODO
DirectX::XMVECTOR Transform::getWorldBasisUpVector()
{
  return DirectX::XMVector3TransformNormal(cDefaultBasisUp, getNodeToWorldMatrix());
}

DirectX::XMVECTOR Transform::getWorldBasisRightVector()
{
  return DirectX::XMVector3TransformNormal(cDefaultBasisRight, getNodeToWorldMatrix());
}

DirectX::XMVECTOR Transform::getWorldBasisForwardVector()
{
  return DirectX::XMVector3TransformNormal(cDefaultBasisForward, getNodeToWorldMatrix());
}

DirectX::XMVECTOR Transform::getDefaultBasisUpVector()
{
  return cDefaultBasisUp;
}

DirectX::XMVECTOR Transform::getDefaultBasisRightVector()
{
  return cDefaultBasisRight;
}

DirectX::XMVECTOR Transform::getDefaultBasisForwardVector()
{
  return cDefaultBasisForward;
}

void Transform::invalidateNodeToWorld()
{
  auto invalidateNodeToWorldChildren = [](flecs::entity node) {
    if (node.is_valid() && node.has<scene::Transform>())
    {
      node.get_mut<scene::Transform>()->invalidateNodeToWorld();
    }
    };
  
  if (m_node.is_valid())
  {
    m_node.children(invalidateNodeToWorldChildren);
  }

  m_updateNodeToWorld = true;
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
  {
      return;
  }

  flecs::entity parent = m_node.parent();
  if (parent.is_valid() && parent.has<scene::Transform>())
  {
    Transform* transform = parent.get_mut<Transform>();
    const DirectX::XMMATRIX nodeToWorld = DirectX::XMMatrixMultiply(getNodeToParentMatrix(), transform->getNodeToWorldMatrix());
    DirectX::XMStoreFloat4x4(&m_nodeToWorldFloat4x4, nodeToWorld);
  }
  else
  {
    const DirectX::XMMATRIX nodeToWorld = getNodeToParentMatrix();
    DirectX::XMStoreFloat4x4(&m_nodeToWorldFloat4x4, nodeToWorld);
  }

  const DirectX::XMMATRIX nodeToWorld = DirectX::XMLoadFloat4x4(&m_nodeToWorldFloat4x4);
  DirectX::XMStoreFloat4x4(&m_worldToNodeFloat4x4, DirectX::XMMatrixInverse(nullptr, nodeToWorld));

  m_updateNodeToWorld = false;
}

}