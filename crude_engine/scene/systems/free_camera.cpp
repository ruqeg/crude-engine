#include <SDL3/SDL.h>
#include <directxmath/DirectXMath.h>
#include <flecs.h>

module crude.scene.free_camera;

import crude.scene.transform;
import crude.core.logger;

namespace crude::scene
{

void freeCameraUpdate(flecs::iter& it, size_t index, Free_Camera_Component& freeCamera, scene::Transform& transform)
{
  const core::float64 deltaTime = *it.world().get<core::float64>();
  const core::int32 movingForaward = freeCamera.movingDirection[0].z - freeCamera.movingDirection[1].z;
  const core::int32 movingUp = freeCamera.movingDirection[0].y - freeCamera.movingDirection[1].y;
  const core::int32 movingRight = freeCamera.movingDirection[0].x - freeCamera.movingDirection[1].x;

  const DirectX::XMVECTOR movingSpeedMultiplier = DirectX::XMLoadFloat3(&freeCamera.movingSpeedMultiplier);

  if (movingRight)
    transform.addTranslation(DirectX::XMVectorScale(DirectX::XMVectorMultiply(transform.getWorldBasisRightVector(), movingSpeedMultiplier), movingRight * deltaTime));
  if (movingForaward)
    transform.addTranslation(DirectX::XMVectorScale(DirectX::XMVectorMultiply(transform.getWorldBasisForwardVector(), movingSpeedMultiplier), movingForaward * deltaTime));
  if (movingUp)
    transform.addTranslation(DirectX::XMVectorScale(DirectX::XMVectorMultiply(transform.getWorldBasisUpVector(), movingSpeedMultiplier), movingUp * deltaTime));

  transform.addRotationAxis(transform.getDefaultBasisUpVector(), -freeCamera.rotatingSpeedMultiplier.x * freeCamera.rotatingRel.x * deltaTime);
  transform.addRotationAxis(transform.getWorldBasisRightVector(), -freeCamera.rotatingSpeedMultiplier.y * freeCamera.rotatingRel.y * deltaTime);
  freeCamera.rotatingRel = {};
}

void freeCameraUpdateEvent(flecs::iter& it, size_t index, Free_Camera_Component& freeCamera)
{
  const SDL_Event* inputEvent = it.world().get<SDL_Event>();
  switch (inputEvent->type)
  {
  case SDL_EVENT_KEY_DOWN:
    switch (inputEvent->key.keysym.scancode)
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
    switch (inputEvent->key.keysym.scancode)
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
    switch (inputEvent->button.button)
    {
    case SDL_BUTTON_RIGHT:
      freeCamera.rotatingLocked = false;
      break;
    }
    break;
  case SDL_EVENT_MOUSE_BUTTON_UP:
    switch (inputEvent->button.button)
    {
    case SDL_BUTTON_RIGHT:
      freeCamera.rotatingLocked = true;
      break;
    }
    break;
  case SDL_EVENT_MOUSE_MOTION:
    if (!freeCamera.rotatingLocked)
    {
      freeCamera.rotatingRel.x += inputEvent->motion.xrel;
      freeCamera.rotatingRel.y += inputEvent->motion.yrel;
    }
    break;
  }
}

}