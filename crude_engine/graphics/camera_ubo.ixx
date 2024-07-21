module;

#include <directxmath/DirectXMath.h>

export module crude.graphics.camera_ubo;

export import crude.scene.camera;

export namespace crude::graphics
{

struct Camera_UBO
{
  Camera_UBO() = default;
  explicit Camera_UBO(const scene::Camera& camera)
    : worldToClip(camera.getWorldToClipFloat4x4())
    , worldToView(camera.getWorldToViewFloat4x4())
    , viewToClip(camera.getViewToClipFloat4x4())
    , clipToView(camera.getClipToViewFloat4x4())
    , viewToWorld(camera.getViewToWorldFloat4x4())
    , clipToWorld(camera.getClipToWorldFloat4x4())
  {}
  DirectX::XMFLOAT4X4 worldToClip;
  DirectX::XMFLOAT4X4 worldToView;
  DirectX::XMFLOAT4X4 viewToClip;
  DirectX::XMFLOAT4X4 clipToView;
  DirectX::XMFLOAT4X4 viewToWorld;
  DirectX::XMFLOAT4X4 clipToWorld;
};

}
