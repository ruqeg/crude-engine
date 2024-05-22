module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.descriptor_set;

import crude.core.shared_ptr;
import crude.core.std_containers_heap;
import crude.graphics.vulkan.vulkan_object;

export namespace crude::graphics
{

class Device;
class Descriptor_Set_Layout;
class Descriptor_Pool;

class Descriptor_Set : public Vulkan_Object<VkDescriptorSet>
{
public:
  explicit Descriptor_Set(core::Shared_Ptr<const Device>                                device,
                          const core::vector<core::Shared_Ptr<Descriptor_Set_Layout>>&  setLayouts,
                          core::Shared_Ptr<Descriptor_Pool>                             pool);
  ~Descriptor_Set();
private:
  core::Shared_Ptr<const Device>                         m_device;
  core::vector<core::Shared_Ptr<Descriptor_Set_Layout>>  m_setLayouts;
  core::Shared_Ptr<Descriptor_Pool>                      m_pool;
};

}
