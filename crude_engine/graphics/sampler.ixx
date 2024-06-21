module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.sampler;

export import crude.core.std_containers_heap;
export import crude.graphics.sampler_state;
import crude.graphics.vulkan_object;

export namespace crude::graphics
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
