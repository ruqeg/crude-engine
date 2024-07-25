module;

#include <directxmath/DirectXMath.h>

export module crude.scene.camera;

export import crude.core.alias;
export import crude.core.std_containers_heap;

export namespace crude::scene
{

class Camera
{
public:
  Camera();
public:
  void calculateViewToClipMatrix(core::float32 fovRadians, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ);
  void calculateWorldToViewMatrix();
public:
  void setPosition(DirectX::XMFLOAT3 newPosition);
  void setPosition(DirectX::FXMVECTOR newPosition);
  void setPosition(core::float32 x, core::float32 y, core::float32 z);
  void addPosition(DirectX::FXMVECTOR newPosition);
  void addPosition(core::float32 x, core::float32 y, core::float32 z);
  void setRotation(DirectX::XMFLOAT3 newRotation);
  void setRotation(DirectX::FXMVECTOR newRotation);
  void setRotation(core::float32 x, core::float32 y, core::float32 z);
  void addRotation(DirectX::FXMVECTOR addedRotation);
  void addRotation(core::float32 x, core::float32 y, core::float32 z);
public:
  DirectX::XMMATRIX getWorldToClipMatrix() const { return DirectX::XMLoadFloat4x4(&m_worldToClipFloat4x4); }
  DirectX::XMMATRIX getClipToWorldMatrix() const { return DirectX::XMLoadFloat4x4(&m_clipToWorldFloat4x4); }
  DirectX::XMMATRIX getWorldToViewMatrix() const { return DirectX::XMLoadFloat4x4(&m_worldToViewFloat4x4); }
  DirectX::XMMATRIX getViewToWorldMatrix() const { return DirectX::XMLoadFloat4x4(&m_viewToWorldFloat4x4); }
  DirectX::XMMATRIX getViewToClipMatrix() const { return DirectX::XMLoadFloat4x4(&m_viewToClipFloat4x4); }
  DirectX::XMMATRIX getClipToViewMatrix() const { return DirectX::XMLoadFloat4x4(&m_clipToViewFloat4x4); }
  const DirectX::XMFLOAT4X4& getWorldToClipFloat4x4() const { return m_worldToClipFloat4x4; }
  const DirectX::XMFLOAT4X4& getClipToWorldFloat4x4() const { return m_clipToWorldFloat4x4; }
  const DirectX::XMFLOAT4X4& getWorldToViewFloat4x4() const { return m_worldToViewFloat4x4; }
  const DirectX::XMFLOAT4X4& getViewToWorldFloat4x4() const { return m_viewToWorldFloat4x4; }
  const DirectX::XMFLOAT4X4& getViewToClipFloat4x4() const { return m_viewToClipFloat4x4; }
  const DirectX::XMFLOAT4X4& getClipToViewFloat4x4() const { return m_clipToViewFloat4x4; }
  DirectX::XMVECTOR getPositionVector() const { return DirectX::XMLoadFloat3(&m_positionFloat3); }
  const DirectX::XMFLOAT3& getPositionFloat3() const { return m_positionFloat3; }
  DirectX::XMVECTOR getRotationVector() const { return DirectX::XMLoadFloat3(&m_rotationFloat3); }
  const DirectX::XMFLOAT3& getRotationFloat3() const { return m_rotationFloat3; }
  DirectX::XMVECTOR getRightVector() const { return DirectX::XMLoadFloat3(&m_rightFloat3); }
  DirectX::XMVECTOR getTopVector() const { return DirectX::XMLoadFloat3(&m_topFloat3); }
  DirectX::XMVECTOR getForwardVector() const { return DirectX::XMLoadFloat3(&m_forwardFloat3); }
  core::float32 getFovRadians() const { return m_fovRadians; }
  core::float32 getNearZ() const { return m_nearZ;  }
  core::float32 getFarZ() const { return m_farZ; }
private:
  DirectX::XMFLOAT3    m_positionFloat3;
  DirectX::XMFLOAT3    m_rotationFloat3;
  DirectX::XMFLOAT3    m_forwardFloat3;
  DirectX::XMFLOAT3    m_rightFloat3;
  DirectX::XMFLOAT3    m_topFloat3;
  DirectX::XMFLOAT4X4  m_worldToClipFloat4x4;
  DirectX::XMFLOAT4X4  m_clipToWorldFloat4x4;
  DirectX::XMFLOAT4X4  m_worldToViewFloat4x4;
  DirectX::XMFLOAT4X4  m_viewToWorldFloat4x4;
  DirectX::XMFLOAT4X4  m_viewToClipFloat4x4;
  DirectX::XMFLOAT4X4  m_clipToViewFloat4x4;
  core::float32        m_fovRadians;
  core::float32        m_nearZ;
  core::float32        m_farZ;
};
}
