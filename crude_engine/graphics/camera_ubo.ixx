module;

#include <DirectXMath.h>
#include <flecs.h>

export module crude.gfx.camera_ubo;

export import crude.scene.camera;
export import crude.scene.transform;

export namespace crude::gfx
{

struct Camera_UBO
{
  Camera_UBO() = default;
  explicit Camera_UBO(flecs::entity_view cameraNode)
  {
    if (cameraNode.has<scene::Camera>())
    {
      const scene::Camera* camera = cameraNode.get<scene::Camera>();
      viewToClip = camera->getViewToClipFloat4x4();
      clipToView = camera->getClipToViewFloat4x4();
    }
    if (cameraNode.has<scene::Transform>())
    {
      scene::Transform* transform = cameraNode.get_mut<scene::Transform>();
      worldToView = transform->getWorldToNodeFloat4x4();
      viewToWorld = transform->getNodeToWorldFloat4x4();
      DirectX::XMStoreFloat3A(&position, DirectX::XMLoadFloat3(&transform->getTranslationFloat3()));
    }
  }
  DirectX::XMFLOAT3A  position;
  DirectX::XMFLOAT4X4 worldToView;
  DirectX::XMFLOAT4X4 viewToClip;
  DirectX::XMFLOAT4X4 clipToView;
  DirectX::XMFLOAT4X4 viewToWorld;
};

}
