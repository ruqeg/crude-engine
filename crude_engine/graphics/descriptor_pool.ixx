module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.descriptor_pool;

import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.graphics.vulkan.vulkan_object;

export namespace crude::graphics
{

class Device;

class Descriptor_Pool : public Vulkan_Object<VkDescriptorPool>
{
public:
  explicit Descriptor_Pool(core::Shared_Ptr<const Device>           device,
                           const core::span<VkDescriptorPoolSize>&  poolSizes,
                           core::uint32                             maxSets,
                           bool                                     freeDescriptorSet = false);
  ~Descriptor_Pool();
  bool canFreeDescriptorSet() const;
private:
  const core::Shared_Ptr<const Device>  m_device;
  const bool                            m_freeDescriptorSet;
};

}
