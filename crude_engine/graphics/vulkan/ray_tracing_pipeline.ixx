module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.ray_tracing_pipeline;

export import crude.gfx.vk.pipeline;

export namespace crude::gfx::vk
{

class Ray_Tracing_Pipeline : public Pipeline
{
public:
  explicit Ray_Tracing_Pipeline();
};

}
