module;

#include <directxmath/DirectXMath.h>

export module crude.scene.transform;

export import crude.core.alias;
export import crude.core.std_containers_heap;

export namespace crude::scene
{

class Node;

class Transform
{
public:
  Transform(core::shared_ptr<Node> node);
  void setTranslation(const DirectX::XMFLOAT3& translatio);
  void setTranslation(DirectX::FXMVECTOR translation);
  void setTranslation(core::float32 x, core::float32 y, core::float32 z);
  void setScale(const DirectX::XMFLOAT3& scale);
  void setScale(DirectX::FXMVECTOR scale);
  void setScale(core::float32 x, core::float32 y, core::float32 z);
  void setRotation(const DirectX::XMFLOAT3& rotation);
  void setRotation(DirectX::FXMVECTOR rotation);
  void setRotation(core::float32 x, core::float32 y, core::float32 z);
  void setNodeToParent(const DirectX::XMFLOAT4X4& nodeToParent);
  void setNodeToParent(DirectX::FXMMATRIX nodeToParent);
  DirectX::XMMATRIX getNodeToParentMatrix() const;
  DirectX::XMMATRIX getNodeToWorldMatrix();
  const DirectX::XMFLOAT4X4& getNodeToWorldFloat4x4();
  core::shared_ptr<Node> getNode();
  core::shared_ptr<const Node> getNode() const;
  const DirectX::XMFLOAT3& getTranslationFloat3() const { return m_translationFloat3; }
  const DirectX::XMFLOAT3& getRotationFloat3() const { return m_rotationFloat3; }
  const DirectX::XMFLOAT3& getScaleFloat3() const { return m_scaleFloat3; }
  DirectX::XMVECTOR getTranslationVector() const { return DirectX::XMLoadFloat3(&m_translationFloat3); }
  DirectX::XMVECTOR getRotationVector() const { return DirectX::XMLoadFloat3(&m_rotationFloat3); }
  DirectX::XMVECTOR getScaleVector() const { return DirectX::XMLoadFloat3(&m_scaleFloat3); }
  void invalidateNodeToWorld() { m_updateNodeToWorld = true; }
private:
  void decomposeNodeToParent(DirectX::FXMMATRIX nodeToParent);
  void updateNodeToWorld();
private:
  core::shared_ptr<Node>  m_node;
  DirectX::XMFLOAT4X4     m_nodeToWorldFloat4x4;
  DirectX::XMFLOAT3       m_translationFloat3;
  DirectX::XMFLOAT3       m_rotationFloat3;
  DirectX::XMFLOAT3       m_scaleFloat3;
  bool                    m_updateNodeToWorld = false;
};

}