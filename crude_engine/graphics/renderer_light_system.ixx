module;

#include <vulkan/vulkan.hpp>
#include <DirectXMath.h>
#include <flecs.h>

export module crude.graphics.renderer_light_system;

export import crude.core.std_containers_heap;
export import crude.graphics.renderer_point_shadow_pass_system;

export namespace crude::graphics
{

class Storage_Buffer;

struct Renderer_Light_To_Update_Flag {};

struct Renderer_Light_Ctx
{
  explicit Renderer_Light_Ctx(core::shared_ptr<Renderer_Point_Shadow_Pass_Systen_Ctx> rendererPointShadowPassCtx);
  core::shared_ptr<Renderer_Point_Shadow_Pass_Systen_Ctx>     rendererPointShadowPassCtx;
  core::optional<core::shared_ptr<graphics::Storage_Buffer>>  pointLightsBuffer;
};

// !TODO
// 0 component - scene::Point_Light_CPU
void rendererLightUpdateSystemProcess(flecs::iter& it);

}