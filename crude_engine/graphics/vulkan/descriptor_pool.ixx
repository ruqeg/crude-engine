module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.descriptor_pool;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;

class Descriptor_Pool : public Vulkan_Object<VkDescriptorPool>
{
public:
  explicit Descriptor_Pool(Shared_Ptr<const Device>           device,
                           const span<VkDescriptorPoolSize>&  poolSizes,
                           uint32                             maxSets,
                           bool                               freeDescriptorSet = false);
  ~Descriptor_Pool();
  bool canFreeDescriptorSet() const;
private:
  const Shared_Ptr<const Device>  m_device;
  const bool                      m_freeDescriptorSet;
};

}
