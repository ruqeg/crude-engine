module;

#include <DirectXMath.h>
#include <SDL3/SDL.h>
#include <flecs.h>

export module crude.scripts.free_camera_script;

export import crude.core.std_containers_heap;
export import crude.scene.transform;

export namespace crude::scripts
{

struct Free_Camera_Component
{
  DirectX::XMFLOAT3  movingSpeedMultiplier{ 7.f, 7.f, 7.f };
  DirectX::XMFLOAT2  rotatingSpeedMultiplier{ -0.15f, -0.15f };
  DirectX::XMFLOAT2  rotatingRel{};
  DirectX::XMFLOAT3  movingDirection[2] { };
  bool               rotatingLocked = true;
};

// 0 component - Free_Camera_Component
// 1 component - Transform
void freeCameraUpdateSystemProcess(flecs::iter& it);

// 0 component - Free_Camera_Component
void freeCameraUpdateEventSystemProcess(flecs::iter& it);

}
