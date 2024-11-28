module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.sampler;

export import crude.core.std_containers_heap;
export import crude.gfx.vk.sampler_state;
import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{

class Device;

class Sampler : public Vulkan_Object<VkSampler>
{
public:
  explicit Sampler(core::shared_ptr<const Device>  device,
                   const Sampler_State&            samplerState);
  ~Sampler();
private:
  core::shared_ptr<const Device>  m_device;
};

}
