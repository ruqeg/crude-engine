#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>

module crude.graphics.renderer_light_system;

import crude.scene.light;
import crude.scene.transform;
import crude.graphics.command_buffer;
import crude.graphics.command_pool;
import crude.graphics.storage_buffer;

namespace crude::graphics
{

Renderer_Light_Ctx::Renderer_Light_Ctx(core::shared_ptr<Renderer_Core_System_Ctx> coreCtx)
  : coreCtx{ coreCtx }
{}

void rendererLightUpdateSystemProcess(flecs::iter& it)
{
  Renderer_Light_Ctx* lightCtx = it.ctx<Renderer_Light_Ctx>();
  
  core::vector<scene::Point_Light_GPU> visiblePointLightsGPU;
  while (it.next())
  {
    auto pointLightsCPU = it.field<scene::Point_Light_CPU>(0);
    for (auto i : it)
    {
      scene::Point_Light_GPU pointLightGPU{};
      pointLightGPU.intensity    = DirectX::XMFLOAT3A(pointLightsCPU[i].intensity.x, pointLightsCPU[i].intensity.y, pointLightsCPU[i].intensity.z);
      pointLightGPU.sourceRadius = pointLightsCPU[i].sourceRadius;
      if (it.entity(i).has<scene::Transform>())
      {
        auto transform = it.entity(i).get_ref<scene::Transform>();
        DirectX::XMStoreFloat3A(&pointLightGPU.position, transform->getTranslationVector());
      }
      visiblePointLightsGPU.push_back(pointLightGPU);
    }
  }

  if (!visiblePointLightsGPU.empty())
  {
    auto commandBuffer = core::allocateShared<graphics::Command_Buffer>(lightCtx->coreCtx->graphicsCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    lightCtx->pointLightsBuffer = core::allocateShared<graphics::Storage_Buffer>(commandBuffer, visiblePointLightsGPU);
  }
}

}