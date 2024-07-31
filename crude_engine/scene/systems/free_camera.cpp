#include <SDL3/SDL.h>
#include <directxmath/DirectXMath.h>
#include <flecs.h>

module crude.scene.free_camera;

import crude.scene.transform;

namespace crude::scene
{

const DirectX::XMVECTOR cDefaultCameraRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR cDefaultCameraTop = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR cDefaultCameraForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

void freeCameraUpdate(flecs::iter& it, size_t index, Free_Camera_Component& freeCamera, scene::Transform& transform)
{
  const core::float64 deltaTime = *it.world().get<core::float64>();
  transform.addTranslation(DirectX::XMVectorScale(cDefaultCameraRight, freeCamera.movingSpeedMultiplier.x * freeCamera.movingDirection.x * deltaTime));
  transform.addTranslation(DirectX::XMVectorScale(cDefaultCameraTop, freeCamera.movingSpeedMultiplier.y * freeCamera.movingDirection.y * deltaTime));
  transform.addTranslation(DirectX::XMVectorScale(cDefaultCameraForward, freeCamera.movingSpeedMultiplier.z * freeCamera.movingDirection.z * deltaTime));
  transform.addRotationAxis(cDefaultCameraTop, freeCamera.rotatingSpeedMultiplier.x * freeCamera.rotatingRel.x * deltaTime);
  transform.addRotationAxis(cDefaultCameraRight, freeCamera.rotatingSpeedMultiplier.y * freeCamera.rotatingRel.y * deltaTime);
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
    case SDL_SCANCODE_W: freeCamera.movingDirection.z += 1; break;
    case SDL_SCANCODE_S: freeCamera.movingDirection.z -= 1; break;
    case SDL_SCANCODE_D: freeCamera.movingDirection.x += 1; break;
    case SDL_SCANCODE_A: freeCamera.movingDirection.x -= 1; break;
    case SDL_SCANCODE_E: freeCamera.movingDirection.y += 1; break;
    case SDL_SCANCODE_Q: freeCamera.movingDirection.y -= 1; break;
    }
    break;
  case SDL_EVENT_KEY_UP:
    switch (inputEvent->key.keysym.scancode)
    {
    case SDL_SCANCODE_W: freeCamera.movingDirection.z -= 1; break;
    case SDL_SCANCODE_S: freeCamera.movingDirection.z += 1; break;
    case SDL_SCANCODE_D: freeCamera.movingDirection.x -= 1; break;
    case SDL_SCANCODE_A: freeCamera.movingDirection.x += 1; break;
    case SDL_SCANCODE_E: freeCamera.movingDirection.y -= 1; break;
    case SDL_SCANCODE_Q: freeCamera.movingDirection.y += 1; break;
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
    freeCamera.rotatingRel.x += inputEvent->motion.xrel;
    freeCamera.rotatingRel.y += inputEvent->motion.yrel;
    break;
  }
}

}