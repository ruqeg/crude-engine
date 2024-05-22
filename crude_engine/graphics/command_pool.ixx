module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.command_pool;

import crude.core.shared_ptr;
import crude.graphics.vulkan.vulkan_object;

export namespace crude::graphics
{

class Device;

class Command_Pool : public Vulkan_Object<VkCommandPool>
{
public:
  explicit Command_Pool(core::Shared_Ptr<const Device>  device,
                        VkCommandPoolCreateFlags        flags,
                        core::uint32                    queueFamilyIndex);
  ~Command_Pool();
private:
  core::Shared_Ptr<const Device>  m_device;
};

}
