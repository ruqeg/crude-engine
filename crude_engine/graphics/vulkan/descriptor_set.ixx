module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.descriptor_set;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_heap;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;
class Descriptor_Set_Layout;
class Descriptor_Pool;

class Descriptor_Set : public Vulkan_Object<VkDescriptorSet>
{
public:
  explicit Descriptor_Set(Shared_Ptr<const Device>                          device,
                          const vector<Shared_Ptr<Descriptor_Set_Layout>>&  setLayouts,
                          Shared_Ptr<Descriptor_Pool>                       pool);
  ~Descriptor_Set();
private:
  Shared_Ptr<const Device>                   m_device;
  vector<Shared_Ptr<Descriptor_Set_Layout>>  m_setLayouts;
  Shared_Ptr<Descriptor_Pool>                m_pool;
};

}
