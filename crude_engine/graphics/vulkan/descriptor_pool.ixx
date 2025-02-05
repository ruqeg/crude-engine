module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.descriptor_pool;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.vk.descriptor_pool_size;
import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{

class Device;

class Descriptor_Pool : public Vulkan_Object<VkDescriptorPool>
{
public:
  explicit Descriptor_Pool(core::shared_ptr<const Device>          device,
                           core::span<const Descriptor_Pool_Size>  poolSizes,
                           core::uint32                            maxSets = 1u,
                           bool                                    freeDescriptorSet = false);
  core::shared_ptr<const Device> getDevice() const;
  ~Descriptor_Pool();
  bool canFreeDescriptorSet() const;
private:
  const core::shared_ptr<const Device>  m_device;
  const bool                            m_freeDescriptorSet;
};

}
