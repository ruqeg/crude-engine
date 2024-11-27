module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.fence;

import crude.core.std_containers_heap;
import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{

class Device;

class Fence : public Vulkan_Object<VkFence>
{
public:
  explicit Fence(core::shared_ptr<const Device> device, VkFenceCreateFlags flags);
  ~Fence();
  bool wait(core::uint64 timeout = UINT64_MAX);
  bool reset();
private:
  core::shared_ptr<const Device>  m_device;
};

}
