module;

#include <directxmath/DirectXMath.h>
#include <SDL3/SDL.h>
#include <flecs.h>

export module crude.scene.free_camera;

export import crude.core.std_containers_heap;
export import crude.scene.transform;

export namespace crude::scene
{

class Scene;

struct Free_Camera_Component
{
  DirectX::XMFLOAT3  movingSpeedMultiplier{ 7.f, 7.f, 7.f };
  DirectX::XMFLOAT2  rotatingSpeedMultiplier{ 0.15f, -0.15f };
  DirectX::XMFLOAT2  rotatingRel;
  DirectX::XMINT3    movingDirection;
  bool               rotatingLocked = true;
};

void freeCameraUpdate(flecs::iter& it, size_t index, Free_Camera_Component& freeCamera, scene::Transform& transform);
void freeCameraUpdateEvent(flecs::iter& it, size_t index, Free_Camera_Component& freeCamera);

}