#include <SDL3/SDL.h>
#include <DirectXMath.h>
#include <flecs.h>

module crude.scene.free_camera_script;

import crude.core.logger;
import crude.platform.input_system;

namespace crude::scene::script
{

void freeCameraUpdateSystemProcess(flecs::iter& it)
{
  while (it.next())
  {
    auto freeCameras = it.field<Free_Camera_Component>(0);
    auto transforms = it.field<Transform>(1);

    for (auto i : it)
    {
      const core::float64 deltaTime = it.delta_time();
      const core::int32 movingForaward = freeCameras[i].movingDirection[0].z - freeCameras[i].movingDirection[1].z;
      const core::int32 movingUp = freeCameras[i].movingDirection[0].y - freeCameras[i].movingDirection[1].y;
      const core::int32 movingRight = freeCameras[i].movingDirection[0].x - freeCameras[i].movingDirection[1].x;

      if (movingRight)
        transforms[i].addTranslation(DirectX::XMVectorScale(transforms[i].getWorldBasisRightVector(), freeCameras[i].movingSpeedMultiplier.x * movingRight * deltaTime));
      if (movingForaward)
        transforms[i].addTranslation(DirectX::XMVectorScale(transforms[i].getWorldBasisForwardVector(), freeCameras[i].movingSpeedMultiplier.z * movingForaward * deltaTime));
      if (movingUp)
        transforms[i].addTranslation(DirectX::XMVectorScale(transforms[i].getWorldBasisUpVector(), freeCameras[i].movingSpeedMultiplier.y * movingUp * deltaTime));

      DirectX::XMVECTOR cameraUp = DirectX::XMVectorGetY(transforms[i].getWorldBasisUpVector()) > 0.0f ? transforms[i].getDefaultBasisUpVector() : DirectX::XMVectorNegate(transforms[i].getDefaultBasisUpVector());
      transforms[i].addRotationAxis(cameraUp, -freeCameras[i].rotatingSpeedMultiplier.x * freeCameras[i].rotatingRel.x * deltaTime);
      transforms[i].addRotationAxis(transforms[i].getWorldBasisRightVector(), -freeCameras[i].rotatingSpeedMultiplier.y * freeCameras[i].rotatingRel.y * deltaTime);
      freeCameras[i].rotatingRel = {};
    }
  }
}

void freeCameraUpdateEventSystemProcess(flecs::iter& it)
{
  const SDL_Event& inputEvent = it.world().get<platform::Input_System_Component>()->inputEvent;

  while (it.next())
  {
    auto freeCameras = it.field<Free_Camera_Component>(0);

    for (auto i : it)
    {
      switch (inputEvent.type)
      {
      case SDL_EVENT_KEY_DOWN:
        switch (inputEvent.key.scancode)
        {
        case SDL_SCANCODE_W: freeCameras[i].movingDirection[0].z = 1; break;
        case SDL_SCANCODE_S: freeCameras[i].movingDirection[1].z = 1; break;
        case SDL_SCANCODE_D: freeCameras[i].movingDirection[0].x = 1; break;
        case SDL_SCANCODE_A: freeCameras[i].movingDirection[1].x = 1; break;
        case SDL_SCANCODE_E: freeCameras[i].movingDirection[0].y = 1; break;
        case SDL_SCANCODE_Q: freeCameras[i].movingDirection[1].y = 1; break;
        }
        break;
      case SDL_EVENT_KEY_UP:
        switch (inputEvent.key.scancode)
        {
        case SDL_SCANCODE_W: freeCameras[i].movingDirection[0].z = 0; break;
        case SDL_SCANCODE_S: freeCameras[i].movingDirection[1].z = 0; break;
        case SDL_SCANCODE_D: freeCameras[i].movingDirection[0].x = 0; break;
        case SDL_SCANCODE_A: freeCameras[i].movingDirection[1].x = 0; break;
        case SDL_SCANCODE_E: freeCameras[i].movingDirection[0].y = 0; break;
        case SDL_SCANCODE_Q: freeCameras[i].movingDirection[1].y = 0; break;
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        switch (inputEvent.button.button)
        {
        case SDL_BUTTON_RIGHT:
          freeCameras[i].rotatingLocked = false;
          break;
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_UP:
        switch (inputEvent.button.button)
        {
        case SDL_BUTTON_RIGHT:
          freeCameras[i].rotatingLocked = true;
          break;
        }
        break;
      case SDL_EVENT_MOUSE_MOTION:
        if (!freeCameras[i].rotatingLocked)
        {
          freeCameras[i].rotatingRel.x += inputEvent.motion.xrel;
          freeCameras[i].rotatingRel.y += inputEvent.motion.yrel;
        }
        break;
      };
    }
  }
}

}