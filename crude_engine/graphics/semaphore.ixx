module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.semaphore;

import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;

class Semaphore : public Vulkan_Object<VkSemaphore>
{
public:
  explicit Semaphore(core::shared_ptr<const Device> device, VkSemaphoreCreateFlags flags = 0u);
  ~Semaphore();
private:
  core::shared_ptr<const Device>  m_device;
};

}
