#include <SDL3/SDL.h>
#include <directxmath/DirectXMath.h>
#include <flecs.h>

module crude.scene.free_camera;

import crude.scene.transform;
import crude.core.logger;
import crude.system.input_system;

namespace crude::scene
{

void freeCameraUpdateSystemProcess(flecs::iter& it, size_t index, Free_Camera_Component& freeCamera, scene::Transform& transform)
{
  const core::float64 deltaTime = it.delta_time();
  const core::int32 movingForaward = freeCamera.movingDirection[0].z - freeCamera.movingDirection[1].z;
  const core::int32 movingUp = freeCamera.movingDirection[0].y - freeCamera.movingDirection[1].y;
  const core::int32 movingRight = freeCamera.movingDirection[0].x - freeCamera.movingDirection[1].x;

  if (movingRight)
    transform.addTranslation(DirectX::XMVectorScale(transform.getWorldBasisRightVector(), freeCamera.movingSpeedMultiplier.x * movingRight * deltaTime));
  if (movingForaward)
    transform.addTranslation(DirectX::XMVectorScale(transform.getWorldBasisForwardVector(), freeCamera.movingSpeedMultiplier.z * movingForaward * deltaTime));
  if (movingUp)
    transform.addTranslation(DirectX::XMVectorScale(transform.getWorldBasisUpVector(), freeCamera.movingSpeedMultiplier.y * movingUp * deltaTime));

  DirectX::XMVECTOR cameraUp = DirectX::XMVectorGetY(transform.getWorldBasisUpVector()) > 0.0f ? transform.getDefaultBasisUpVector() : DirectX::XMVectorNegate(transform.getDefaultBasisUpVector());
  transform.addRotationAxis(cameraUp, -freeCamera.rotatingSpeedMultiplier.x * freeCamera.rotatingRel.x * deltaTime);
  transform.addRotationAxis(transform.getWorldBasisRightVector(), -freeCamera.rotatingSpeedMultiplier.y * freeCamera.rotatingRel.y * deltaTime);
  freeCamera.rotatingRel = {};
}

void freeCameraUpdateEventSystemProcess(flecs::iter& it, size_t index, scene::Free_Camera_Component& freeCamera)
{
  const system::Input_System_Component* inputSystemComponent = it.world().get<system::Input_System_Component>();
  const SDL_Event& inputEvent = inputSystemComponent->inputEvent;
  
  switch (inputEvent.type)
  {
  case SDL_EVENT_KEY_DOWN:
    switch (inputEvent.key.keysym.scancode)
    {
    case SDL_SCANCODE_W: freeCamera.movingDirection[0].z = 1; break;
    case SDL_SCANCODE_S: freeCamera.movingDirection[1].z = 1; break;
    case SDL_SCANCODE_D: freeCamera.movingDirection[0].x = 1; break;
    case SDL_SCANCODE_A: freeCamera.movingDirection[1].x = 1; break;
    case SDL_SCANCODE_E: freeCamera.movingDirection[0].y = 1; break;
    case SDL_SCANCODE_Q: freeCamera.movingDirection[1].y = 1; break;
    }
    break;
  case SDL_EVENT_KEY_UP:
    switch (inputEvent.key.keysym.scancode)
    {
    case SDL_SCANCODE_W: freeCamera.movingDirection[0].z = 0; break;
    case SDL_SCANCODE_S: freeCamera.movingDirection[1].z = 0; break;
    case SDL_SCANCODE_D: freeCamera.movingDirection[0].x = 0; break;
    case SDL_SCANCODE_A: freeCamera.movingDirection[1].x = 0; break;
    case SDL_SCANCODE_E: freeCamera.movingDirection[0].y = 0; break;
    case SDL_SCANCODE_Q: freeCamera.movingDirection[1].y = 0; break;
    }
    break;
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    switch (inputEvent.button.button)
    {
    case SDL_BUTTON_RIGHT:
      freeCamera.rotatingLocked = false;
      break;
    }
    break;
  case SDL_EVENT_MOUSE_BUTTON_UP:
    switch (inputEvent.button.button)
    {
    case SDL_BUTTON_RIGHT:
      freeCamera.rotatingLocked = true;
      break;
    }
    break;
  case SDL_EVENT_MOUSE_MOTION:
    if (!freeCamera.rotatingLocked)
    {
      freeCamera.rotatingRel.x += inputEvent.motion.xrel;
      freeCamera.rotatingRel.y += inputEvent.motion.yrel;
    }
    break;
  };
}

}