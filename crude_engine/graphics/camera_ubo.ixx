module;

export module crude.graphics.camera_ubo;

export import crude.math.fuicont;
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
  math::Float4x4 worldToClip;
  math::Float4x4 worldToView;
  math::Float4x4 viewToClip;
  math::Float4x4 clipToView;
  math::Float4x4 viewToWorld;
  math::Float4x4 clipToWorld;
};

}
