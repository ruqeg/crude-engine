module;

#include <directxmath/DirectXMath.h>
#include <flecs.h>

export module crude.graphics.camera_ubo;

export import crude.scene.camera;
export import crude.scene.node;
export import crude.scene.transform;

export namespace crude::graphics
{

struct Camera_UBO
{
  Camera_UBO() = default;
  explicit Camera_UBO(const scene::Camera& camera)
  {
    viewToClip = camera.getViewToClipFloat4x4();
    clipToView = camera.getClipToViewFloat4x4();
    
    core::shared_ptr<const scene::Node> cameraNode = camera.getNode();
    if (cameraNode && cameraNode->getEntity().has<scene::Transform>())
    {
      auto transform = cameraNode->getEntity().get_mut<scene::Transform>();
      worldToView = transform->getNodeToWorldFloat4x4();
      viewToWorld = transform->getWorldToNodeFloat4x4();
    }
  }
  DirectX::XMFLOAT4X4 worldToView;
  DirectX::XMFLOAT4X4 viewToClip;
  DirectX::XMFLOAT4X4 clipToView;
  DirectX::XMFLOAT4X4 viewToWorld;
};

}
