module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.fence;

import crude_engine.core.shared_ptr;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;

class Fence : public Vulkan_Object<VkFence>
{
public:
  explicit Fence(Shared_Ptr<const Device> device, VkFenceCreateFlags flags);
  ~Fence();
  bool wait(uint64 timeout = UINT64_MAX);
  bool reset();
private:
  Shared_Ptr<const Device>  m_device;
};

}
