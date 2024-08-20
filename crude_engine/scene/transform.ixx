module;

#include <DirectXMath.h>
#include <flecs.h>

export module crude.scene.transform;

export import crude.core.alias;
export import crude.core.std_containers_heap;

export namespace crude::scene
{

class Transform
{
public:
  Transform() = default;
  Transform(flecs::entity_view node);
  void setTranslation(const DirectX::XMFLOAT3& translatio);
  void setTranslation(DirectX::FXMVECTOR translation);
  void setTranslation(core::float32 x, core::float32 y, core::float32 z);
  void addTranslation(const DirectX::XMFLOAT3& translatio);
  void addTranslation(DirectX::FXMVECTOR translation);
  void addTranslation(core::float32 x, core::float32 y, core::float32 z);
  void setScale(const DirectX::XMFLOAT3& scale);
  void setScale(DirectX::FXMVECTOR scale);
  void setScale(core::float32 x, core::float32 y, core::float32 z);
  void addScale(const DirectX::XMFLOAT3& scale);
  void addScale(DirectX::FXMVECTOR scale);
  void addScale(core::float32 x, core::float32 y, core::float32 z);
  void setRotation(const DirectX::XMFLOAT3& rotation);
  void setRotation(DirectX::FXMVECTOR rotation);
  void setRotation(core::float32 x, core::float32 y, core::float32 z);
  void setRotationQuaternion(DirectX::FXMVECTOR quaternion);
  void addRotation(const DirectX::XMFLOAT3& rotation);
  void addRotation(DirectX::FXMVECTOR rotation);
  void addRotation(core::float32 x, core::float32 y, core::float32 z);
  void addRotationAxis(DirectX::FXMVECTOR vector, core::float32 rotation);
  void setNodeToParent(const DirectX::XMFLOAT4X4& nodeToParent);
  void setNodeToParent(DirectX::FXMMATRIX nodeToParent);
  DirectX::XMMATRIX getNodeToParentMatrix() const;
  DirectX::XMMATRIX getNodeToWorldMatrix();
  DirectX::XMMATRIX getWorldToNodeMatrix();
  const DirectX::XMFLOAT4X4& getNodeToWorldFloat4x4();
  const DirectX::XMFLOAT4X4& getWorldToNodeFloat4x4();
  const DirectX::XMFLOAT3& getTranslationFloat3() const { return m_translationFloat3; }
  const DirectX::XMFLOAT4& getRotationFloat3() const { return m_rotationFloat4; }
  const DirectX::XMFLOAT3& getScaleFloat4() const { return m_scaleFloat3; }
  DirectX::XMVECTOR getTranslationVector() const { return DirectX::XMLoadFloat3(&m_translationFloat3); }
  DirectX::XMVECTOR getRotationQuaternion() const { return DirectX::XMLoadFloat4(&m_rotationFloat4); }
  DirectX::XMVECTOR getScaleVector() const { return DirectX::XMLoadFloat3(&m_scaleFloat3); }
  DirectX::XMVECTOR getWorldBasisUpVector();
  DirectX::XMVECTOR getWorldBasisRightVector();
  DirectX::XMVECTOR getWorldBasisForwardVector();
  static DirectX::XMVECTOR getDefaultBasisUpVector();
  static DirectX::XMVECTOR getDefaultBasisRightVector();
  static DirectX::XMVECTOR getDefaultBasisForwardVector();
  void invalidateNodeToWorld() { m_updateNodeToWorld = true; }
private:
  void decomposeNodeToParent(DirectX::FXMMATRIX nodeToParent);
  void updateNodeToWorld();
private:
  flecs::entity_view   m_node;
  DirectX::XMFLOAT4X4  m_nodeToWorldFloat4x4;
  DirectX::XMFLOAT4X4  m_worldToNodeFloat4x4;
  DirectX::XMFLOAT3    m_translationFloat3;
  DirectX::XMFLOAT4    m_rotationFloat4;
  DirectX::XMFLOAT3    m_scaleFloat3;
  bool                 m_updateNodeToWorld = false;
};

}