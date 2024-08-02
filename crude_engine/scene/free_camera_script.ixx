module;

#include <directxmath/DirectXMath.h>
#include <SDL3/SDL.h>
#include <flecs.h>

export module crude.scene.free_camera_script;

export import crude.core.std_containers_heap;
export import crude.scene.transform;

export namespace crude::scene::script
{

struct Free_Camera_Component
{
  DirectX::XMFLOAT3  movingSpeedMultiplier{ 7.f, -7.f, 7.f };
  DirectX::XMFLOAT2  rotatingSpeedMultiplier{ -0.15f, 0.15f };
  DirectX::XMFLOAT2  rotatingRel{};
  DirectX::XMFLOAT3  movingDirection[2] { };
  bool               rotatingLocked = true;
};

void freeCameraUpdateSystemEach(flecs::iter& it, size_t index, Free_Camera_Component& freeCamera, Transform& transform);
void freeCameraUpdateEventSystemEach(flecs::iter& it, size_t index, Free_Camera_Component& freeCamera);

}
