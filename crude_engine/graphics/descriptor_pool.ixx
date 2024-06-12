module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.descriptor_pool;

import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;

struct Descriptor_Pool_Size final : public VkDescriptorPoolSize
{
  Descriptor_Pool_Size(VkDescriptorType type, core::uint32 descriptorCount)
  {
    this->type = type;
    this->descriptorCount = descriptorCount;
  }
};

class Descriptor_Pool : public Vulkan_Object<VkDescriptorPool>
{
public:
  explicit Descriptor_Pool(core::shared_ptr<const Device>           device,
                           const core::span<Descriptor_Pool_Size>&  poolSizes,
                           bool                                     freeDescriptorSet = false);
  ~Descriptor_Pool();
  bool canFreeDescriptorSet() const;
private:
  const core::shared_ptr<const Device>  m_device;
  const bool                            m_freeDescriptorSet;
};

}
