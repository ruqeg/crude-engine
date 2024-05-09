module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.command_pool;

import crude_engine.core.shared_ptr;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;

class Command_Pool : public Vulkan_Object<VkCommandPool>
{
public:
  explicit Command_Pool(Shared_Ptr<const Device>  device,
                        VkCommandPoolCreateFlags  flags,
                        uint32                    queueFamilyIndex);
  ~Command_Pool();
private:
  Shared_Ptr<const Device>  m_device;
};

}
