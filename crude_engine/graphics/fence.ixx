module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.fence;

import crude.core.shared_ptr;
import crude.graphics.vulkan.vulkan_object;

export namespace crude::graphics
{

class Device;

class Fence : public Vulkan_Object<VkFence>
{
public:
  explicit Fence(core::Shared_Ptr<const Device> device, VkFenceCreateFlags flags);
  ~Fence();
  bool wait(core::uint64 timeout = UINT64_MAX);
  bool reset();
private:
  core::Shared_Ptr<const Device>  m_device;
};

}
