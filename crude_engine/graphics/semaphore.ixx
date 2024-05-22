module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.semaphore;

import crude.core.shared_ptr;
import crude.graphics.vulkan.vulkan_object;

export namespace crude::graphics
{

class Device;

class Semaphore : public Vulkan_Object<VkSemaphore>
{
public:
  explicit Semaphore(core::Shared_Ptr<const Device> device, VkSemaphoreCreateFlags flags = 0u);
  ~Semaphore();
private:
  core::Shared_Ptr<const Device>  m_device;
};

}
