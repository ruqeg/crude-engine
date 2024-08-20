module;

#include <DirectXMath.h>

export module crude.scene.camera;

export import crude.core.std_containers_heap;

export namespace crude::scene
{

class Camera
{
public:
  void calculateViewToClipMatrix(core::float32 fovRadians, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ);
public:
  DirectX::XMMATRIX getViewToClipMatrix() const { return DirectX::XMLoadFloat4x4(&m_viewToClipFloat4x4); }
  DirectX::XMMATRIX getClipToViewMatrix() const { return DirectX::XMLoadFloat4x4(&m_clipToViewFloat4x4); }
  const DirectX::XMFLOAT4X4& getViewToClipFloat4x4() const { return m_viewToClipFloat4x4; }
  const DirectX::XMFLOAT4X4& getClipToViewFloat4x4() const { return m_clipToViewFloat4x4; }
  core::float32 getFovRadians() const { return m_fovRadians; }
  core::float32 getNearZ() const { return m_nearZ;  }
  core::float32 getFarZ() const { return m_farZ; }
private:
  DirectX::XMFLOAT4X4     m_viewToClipFloat4x4;
  DirectX::XMFLOAT4X4     m_clipToViewFloat4x4;
  core::float32           m_fovRadians;
  core::float32           m_nearZ;
  core::float32           m_farZ;
};
}
