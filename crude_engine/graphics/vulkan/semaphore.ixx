module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.semaphore;

import crude_engine.core.shared_ptr;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;

class Semaphore : public Vulkan_Object<VkSemaphore>
{
public:
  explicit Semaphore(Shared_Ptr<const Device> device, VkSemaphoreCreateFlags flags = 0u);
  ~Semaphore();
private:
  Shared_Ptr<const Device>  m_device;
};

}
