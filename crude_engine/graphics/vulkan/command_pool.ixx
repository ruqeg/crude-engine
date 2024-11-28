module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.command_pool;

import crude.core.std_containers_heap;
import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{

class Device;

class Command_Pool : public Vulkan_Object<VkCommandPool>
{
public:
  explicit Command_Pool(core::shared_ptr<const Device>  device,
                        VkCommandPoolCreateFlags        flags,
                        core::uint32                    queueFamilyIndex);
  ~Command_Pool();
  core::shared_ptr<const Device> const getDevice();
  core::uint32 getQueueFamilyIndex() { return m_queueFamilyIndex; }
private:
  core::shared_ptr<const Device>  m_device;
  const core::uint32              m_queueFamilyIndex;
};

}
